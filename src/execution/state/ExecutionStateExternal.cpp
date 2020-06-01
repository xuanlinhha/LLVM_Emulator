/*
 * ExecutionStateExternal.cpp
 *
 *  Created on: 20 Oct 2019
 *      Author: xuanlinhha
 */

#include "ExecutionState.h"
#include "execution/symexe/SymExecutor.h"

enum class ExternalCallType {
  NOOP,
  PRINTF,
  MEMCPY,
  MEMSET,
  MALLOC,
  FREE,
  RAND,
  // make symbolic
  SYMBOLIC_BOOL,
  SYMBOLIC_CHAR,
  SYMBOLIC_INT,
  SYMBOLIC_FLOAT,
  SYMBOLIC_DOUBLE,
  SYMBOLIC_POINTER,
  SYMBOLIC_SHORT,
  SYMBOLIC_LONG,
  //
  TOOL_ASSERT,
  TOOL_ASSUME
};

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
    {"rand", ExternalCallType::RAND},
    // symbolic & checking
    {"symbolic_bool", ExternalCallType::SYMBOLIC_BOOL},
    {"symbolic_char", ExternalCallType::SYMBOLIC_CHAR},
    {"symbolic_int", ExternalCallType::SYMBOLIC_INT},
    {"symbolic_float", ExternalCallType::SYMBOLIC_FLOAT},
    {"symbolic_double", ExternalCallType::SYMBOLIC_DOUBLE},
    {"symbolic_pointer", ExternalCallType::SYMBOLIC_POINTER},
    {"symbolic_short", ExternalCallType::SYMBOLIC_SHORT},
    {"symbolic_long", ExternalCallType::SYMBOLIC_LONG},
    //
    {"tool_assert", ExternalCallType::TOOL_ASSERT},
    {"tool_assume", ExternalCallType::TOOL_ASSUME}};

shared_ptr<DynVal>
ExecutionState::callExternalFunction(ImmutableCallSite cs, const Function *f,
                                     vector<shared_ptr<DynVal>> argValues) {
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

  // ignore intrinsic function
  if (f->isIntrinsic()) {
    return std::make_shared<IntVal>(APInt(32, 0));
  }

  // handle common functions
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
  case ExternalCallType::RAND: {
    int r = rand();
    return std::make_shared<IntVal>(APInt(32, r));
  }
  // make symbolic
  case ExternalCallType::SYMBOLIC_BOOL: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<IntVal>(1, SymExprType::VAR, symName.c_str());
  }
  case ExternalCallType::SYMBOLIC_CHAR: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<IntVal>(sizeof(char) * 8, SymExprType::VAR,
                                    symName.c_str());
  }
  case ExternalCallType::SYMBOLIC_INT: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    std::shared_ptr<IntVal> r = std::make_shared<IntVal>(
        sizeof(int) * 8, SymExprType::VAR, symName.c_str());
    return std::move(r);
  }
  case ExternalCallType::SYMBOLIC_FLOAT: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<FloatVal>(SymExprType::VAR, symName.c_str(), false);
  }
  case ExternalCallType::SYMBOLIC_DOUBLE: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<FloatVal>(SymExprType::VAR, symName.c_str(), true);
  }
  case ExternalCallType::SYMBOLIC_POINTER: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    return std::make_shared<PointerVal>(SymExprType::VAR, symName.c_str());
  }
  case ExternalCallType::SYMBOLIC_SHORT: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    std::shared_ptr<IntVal> r = std::make_shared<IntVal>(
        sizeof(short) * 8, SymExprType::VAR, symName.c_str());
    return std::move(r);
  }
  case ExternalCallType::SYMBOLIC_LONG: {
    string symName = readStringFromPointer(
        std::static_pointer_cast<PointerVal>(argValues.at(0)));
    std::shared_ptr<IntVal> r = std::make_shared<IntVal>(
        sizeof(long) * 8, SymExprType::VAR, symName.c_str());
    return std::move(r);
  }
  //
  case ExternalCallType::TOOL_ASSUME: {
    assert(argValues.size() >= 1);
    auto assumeExpr = std::static_pointer_cast<IntVal>(argValues[0]);
    auto simplifiedPCS =
        PCSSimplifier::getDependentConstraints(pcs, assumeExpr);

    assumeExpr->print(&errs());
    errs() << "\n";

    // check for possibility of adding assume condition
    if (!SymExecutor::solver->isPossible(simplifiedPCS, assumeExpr, true)) {
      // terminate current state
      isError = true;
      ++Statistics::errorPathCounter;

      // print message
      const llvm::DebugLoc &debugInfo = currentInst->getDebugLoc();
      std::string filePath = debugInfo->getFilename();
      int line = debugInfo->getLine();
      int column = debugInfo->getColumn();
      WithColor(errs(), HighlightColor::Error)
          << "INVAID ASSUME: in " << filePath << " at line " << line
          << ", colume " << column << "\n";
    } else {
      pcs.insert(std::make_pair(assumeExpr, true));
    }

    return std::make_shared<DynVal>(DynValType::UNDEF_VAL);
  }
  case ExternalCallType::TOOL_ASSERT: {
    assert(argValues.size() >= 1);
    auto assertExpr = std::static_pointer_cast<IntVal>(argValues[0]);
    if (!assertExpr->intVal.getBoolValue()) {
      // terminate current state
      isError = true;
      ++Statistics::errorPathCounter;

      // print message
      const llvm::DebugLoc &debugInfo = currentInst->getDebugLoc();
      std::string filePath = debugInfo->getFilename();
      int line = debugInfo->getLine();
      int column = debugInfo->getColumn();
      WithColor(errs(), HighlightColor::Error)
          << "ASSERTION FAIL: in " << filePath << " at line " << line
          << ", colume " << column << "\n";

      // increase fail counter
      ++SymExecutor::assertFailCounter;
      if (SymExecutor::assertFailCounter == SymExecutor::assertFailLimit) {
        SymExecutor::isStop = true;
      }
    }
    return std::make_shared<DynVal>(DynValType::UNDEF_VAL);
  }
  }
  llvm_unreachable("Should not reach here");
  return std::make_shared<DynVal>(DynValType::ERROR);
}
