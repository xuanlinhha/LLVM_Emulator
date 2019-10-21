/*
 * ExecutionStateExternal.cpp
 *
 *  Created on: 20 Oct 2019
 *      Author: xuanlinhha
 */

#include "ExecutionState.h"

enum class ExternalCallType {
  NOOP,
  PRINTF,
  MEMCPY,
  MEMSET,
  MALLOC,
  FREE,
  SYMBOLIC_CHAR,
  SYMBOLIC_INT,
  SYMBOLIC_FLOAT,
  SYMBOLIC_POINTER
};

shared_ptr<DynVal>
ExecutionState::callExternalFunction(ImmutableCallSite cs, const Function *f,
                                     vector<shared_ptr<DynVal>> argValues) {
  static unordered_map<string, ExternalCallType> externalFuncMap = {
      {"printf", ExternalCallType::PRINTF},
      {"memcpy", ExternalCallType::MEMCPY},
      {"memmove", ExternalCallType::MEMCPY},
      {"llvm.memcpy.p0i8.p0i8.i32", ExternalCallType::MEMCPY},
      {"llvm.memcpy.p0i8.p0i8.i64", ExternalCallType::MEMCPY},
      {"memset", ExternalCallType::MEMSET},
      {"llvm.memset.p0i8.i32", ExternalCallType::MEMSET},
      {"llvm.memset.p0i8.i64", ExternalCallType::MEMSET},
      {"malloc", ExternalCallType::MALLOC},
      {"free", ExternalCallType::FREE},
      // symbolic & checking
      {"symbolic_char", ExternalCallType::SYMBOLIC_CHAR},
      {"symbolic_int", ExternalCallType::SYMBOLIC_INT},
      {"symbolic_float", ExternalCallType::SYMBOLIC_FLOAT},
      {"symbolic_pointer", ExternalCallType::SYMBOLIC_POINTER}};

  auto getRawPointer = [this](const shared_ptr<PointerVal> ptr) {
    switch (ptr->space) {
    case AddressSpace::GLOBAL:
      return globalMem->getRawPointerAtAddress(ptr->address);
    case AddressSpace::STACK:
      return stackMem->getRawPointerAtAddress(ptr->address);
    case AddressSpace::HEAP:
      return heapMem->getRawPointerAtAddress(ptr->address);
    }
  };

  //  errs() << "START EXTERNAL: " << f->getName() << "\n";

  auto itr = externalFuncMap.find(f->getName());
  if (itr == externalFuncMap.end()) {
    errs() << "Unknown external function: " << f->getName() << "\n";
    llvm_unreachable("");
  }

  //  errs() << "itr->second = " << itr->first << "\n";

  switch (itr->second) {
  case ExternalCallType::NOOP:
    return std::make_shared<DynVal>(DynValType::UNDEF_VAL);
  case ExternalCallType::PRINTF: {
    assert(argValues.size() >= 1);
    string fmtString = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    auto fmt = boost::format(fmtString);
    for (auto i = std::size_t(1), e = argValues.size(); i < e; ++i) {
      auto argVal = argValues[i];
      if (argVal->valType == DynValType::UNDEF_VAL) {
        llvm_unreachable("Passing undef value into printf?");
      } else if (argVal->valType == DynValType::INT_VAL) {
        fmt % std::static_pointer_cast<IntVal>(argVal)->intVal.getZExtValue();
      } else if (argVal->valType == DynValType::FLOAT_VAL) {
        fmt % std::static_pointer_cast<FloatVal>(argVal)->fpVal;
      } else if (argVal->valType == DynValType::POINTER_VAL) {
        // TODO: Will be a bug in case of printing a pointer value
        string tmp =
            readStringFromPointer(std::static_pointer_cast<PointerVal>(argVal));
        fmt % tmp;
      } else {
        llvm_unreachable("Passing an array or struct to printf?");
      }
    }

    errs() << fmt.str();
    return std::make_shared<IntVal>(APInt(32, fmt.size()));
  }
  case ExternalCallType::MEMCPY: {
    assert(argValues.size() >= 3);

    auto destPtr = std::static_pointer_cast<PointerVal>(argValues.at(0));
    auto srcPtr = std::static_pointer_cast<PointerVal>(argValues.at(1));
    auto size = std::static_pointer_cast<IntVal>(argValues.at(2))
                    ->intVal.getZExtValue();
    std::memcpy(getRawPointer(destPtr), getRawPointer(srcPtr), size);
    return std::make_shared<DynVal>(DynValType::UNDEF_VAL);
  }
  case ExternalCallType::MEMSET: {
    assert(argValues.size() >= 3);
    auto destPtr = std::static_pointer_cast<PointerVal>(argValues.at(0));
    auto fillInt = std::static_pointer_cast<IntVal>(argValues.at(1))
                       ->intVal.getZExtValue();
    auto size = std::static_pointer_cast<IntVal>(argValues.at(2))
                    ->intVal.getZExtValue();
    std::memset(getRawPointer(destPtr), fillInt, size);
    return std::make_shared<DynVal>(DynValType::UNDEF_VAL);
  }
  case ExternalCallType::MALLOC: {
    assert(argValues.size() >= 1);
    auto mallocSize = std::static_pointer_cast<IntVal>(argValues.at(0))
                          ->intVal.getZExtValue();
    auto retAddr = heapMem->allocate(mallocSize);
    return std::make_shared<PointerVal>(retAddr, AddressSpace::HEAP);
  }
  case ExternalCallType::FREE: {
    assert(argValues.size() >= 1);
    auto ptrVal = std::static_pointer_cast<PointerVal>(argValues.at(0));
    if (ptrVal->space != AddressSpace::HEAP)
      llvm_unreachable("Trying to free a non-heap pointer?");
    // currentState->heapMem->free(ptrVal->address());
    // TODO: Free memory on heap memory
    return std::make_shared<DynVal>(DynValType::UNDEF_VAL);
  }

  // make symbolic
  case ExternalCallType::SYMBOLIC_CHAR: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<IntVal>(8, SymExprType::VAR, symName.c_str());
  }
  case ExternalCallType::SYMBOLIC_INT: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<IntVal>(32, SymExprType::VAR, symName.c_str());
  }
  case ExternalCallType::SYMBOLIC_FLOAT: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<FloatVal>(SymExprType::VAR, symName.c_str(), false);
  }
  case ExternalCallType::SYMBOLIC_POINTER: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<PointerVal>(SymExprType::VAR, symName.c_str());
  }
  }
  llvm_unreachable("Should not reach here");
  return std::make_shared<DynVal>(DynValType::ERROR);
}
