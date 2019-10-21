/*
 * ExecutionStateMemory.cpp
 *
 *  Created on: 19 Oct 2019
 *      Author: xuanlinhha
 */

#include "ExecutionState.h"

unsigned ExecutionState::allocateStackMem(unsigned size) {
  frames.back()->allocSize += size;
  return stackMem->allocate(size);
}

void ExecutionState::writeToPointer(const shared_ptr<PointerVal> ptr,
                                    const shared_ptr<DynVal> val) {
  switch (ptr->space) {
  case AddressSpace::GLOBAL: {
    globalMem->write(ptr->address, val);
    break;
  }
  case AddressSpace::STACK: {
    stackMem->write(ptr->address, val);
    break;
  }
  case AddressSpace::HEAP: {
    heapMem->write(ptr->address, val);
    break;
  }
  }
}

shared_ptr<DynVal>
ExecutionState::readFromPointer(const shared_ptr<PointerVal> ptr,
                                Type *loadType) {
  switch (ptr->space) {
  case AddressSpace::GLOBAL:
    return loadValue(globalMem, ptr->address, loadType);
  case AddressSpace::STACK:
    return loadValue(stackMem, ptr->address, loadType);
  case AddressSpace::HEAP:
    return loadValue(heapMem, ptr->address, loadType);
  default:
    llvm_unreachable(
        "SymExecutor::readFromPointer: Pointer type not supported");
  }
  return std::make_shared<DynVal>(DynValType::ERROR);
}

shared_ptr<DynVal> ExecutionState::loadValue(unique_ptr<Memory> &mem,
                                             unsigned addr, Type *loadType) {
  // look up in symbolic memory
  if (mem->symMem.find(addr) != mem->symMem.end()) {
    return mem->symMem[addr];
  }

  // look up in concrete memory
  if (auto intType = dyn_cast<IntegerType>(loadType)) {
    return mem->readAsInt(addr, intType->getBitWidth());
  } else if (loadType->isPointerTy()) {
    return mem->readAsPointer(addr);
  } else if (loadType->isDoubleTy() || loadType->isFloatTy()) {
    return mem->readAsFloat(addr, loadType->isDoubleTy());
  } else if (auto stType = dyn_cast<StructType>(loadType)) {
    auto stLayout = dataLayout->getStructLayout(stType);
    auto retVal =
        std::make_shared<StructVal>(dataLayout->getTypeAllocSize(stType));
    for (auto i = 0u, e = stType->getNumElements(); i < e; ++i) {
      auto elemType = stType->getElementType(i);
      auto offset = stLayout->getElementOffset(i);
      retVal->addField(offset, loadValue(mem, addr + offset, elemType));
    }
    return retVal;
  } else if (auto arrayType = dyn_cast<ArrayType>(loadType)) {
    auto elemType = arrayType->getElementType();
    auto elemSize = dataLayout->getTypeAllocSize(elemType);
    auto arraySize = arrayType->getNumElements();

    auto retVal = std::make_shared<ArrayVal>(elemSize, arraySize);
    for (unsigned i = 0; i < arraySize; ++i) {
      if (elemType->isAggregateType())
        retVal->setElementAtIndex(
            i, loadValue(mem, addr + i * elemSize, elemType));
    }
    return retVal;
  } else {
    llvm_unreachable("Load type not supported");
    return std::make_shared<DynVal>(DynValType::ERROR);
  }
};

string ExecutionState::readStringFromPointer(const shared_ptr<PointerVal> ptr) {
  switch (ptr->space) {
  case AddressSpace::GLOBAL: {
    return globalMem->readAsString(ptr->address);
  }
  case AddressSpace::STACK:
    return stackMem->readAsString(ptr->address);
  case AddressSpace::HEAP:
    return heapMem->readAsString(ptr->address);
  default:
    llvm_unreachable(
        "SymExecutor::readStringFromPointer: Pointer type not supported");
  }
  return "";
}
