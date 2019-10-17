/*
 * Memory.cpp
 *
 *  Created on: 14 Oct 2019
 *      Author: xuanlinhha
 */

#include "Memory.h"

Memory::Memory() : totalSize(DEFAULT_SIZE), usedSize(1), concMem(nullptr) {
  concMem = new unsigned char[DEFAULT_SIZE];
}

Memory::Memory(unsigned ts, unsigned us, unsigned char *cm,
               map<unsigned long, shared_ptr<DynVal>> &sm)
    : totalSize(ts), usedSize(us), concMem(cm), symMem(sm) {}

Memory::~Memory() {}

void Memory::grow() {
  auto newSize = totalSize * 2;
  auto newMem = new unsigned char[newSize];
  std::memcpy(newMem, concMem, totalSize);
  delete[] concMem;
  concMem = newMem;
  totalSize = newSize;
}

bool Memory::isAddressLegal(unsigned long addr) const {
  return (addr < usedSize);
}

unsigned Memory::allocate(unsigned size) {
  while (usedSize + size >= totalSize) {
    grow();
  }
  auto retAddr = usedSize;
  usedSize += size;
  return retAddr;
}

void Memory::deallocate(unsigned size) { usedSize -= size; }

void Memory::free(unsigned long addr, unsigned size) {}

shared_ptr<DynVal> Memory::readAsInt(unsigned long address,
                                     unsigned bitWidth) const {
  assert(bitWidth <= 64 && "No support for >64-bit int read");
  assert(isAddressLegal(address) && "Memory::readAsInt: illegal address");
  // read symbolic
  map<unsigned long, shared_ptr<DynVal>>::const_iterator it =
      symMem.find(address);
  if (it != symMem.end()) {
    assert((it->second->valType == DynValType::INT_VAL) &&
           "Memory::readAsInt: Invalid memory reading!\n");
    return it->second;
  }
  // read concrete
  uint64_t val = 0;
  std::memcpy(&val, concMem + address, bitWidth / 8u);
  return std::make_shared<IntVal>(APInt(bitWidth, val));
}

shared_ptr<DynVal> Memory::readAsFloat(unsigned long address,
                                       bool isDouble) const {
  assert(isAddressLegal(address) && "Memory::readAsFloat: illegal address");
  // read symbolic
  map<unsigned long, shared_ptr<DynVal>>::const_iterator it =
      symMem.find(address);
  if (it != symMem.end()) {
    assert((it->second->valType == DynValType::FLOAT_VAL) &&
           "Memory::readAsFloat: Invalid memory reading!\n");
    return it->second;
  }

  // read concrete
  if (isDouble) {
    double val = 0;
    std::memcpy(&val, concMem + address, sizeof(double));
    return std::make_shared<FloatVal>(val, true);
  } else {
    float val = 0;
    std::memcpy(&val, concMem + address, sizeof(float));
    return std::make_shared<FloatVal>(val, false);
  }
}

shared_ptr<DynVal> Memory::readAsPointer(unsigned long address) const {
  assert(isAddressLegal(address) && "Memory::readAsPointer: illegal address");
  // read symbolic
  map<unsigned long, shared_ptr<DynVal>>::const_iterator it =
      symMem.find(address);
  if (it != symMem.end()) {
    assert((it->second->valType == DynValType::POINTER_VAL) &&
           "Memory::readAsPointer: Invalid memory reading!\n");
    return it->second;
  }

  // read concrete
  unsigned long retAddr = 0;
  std::memcpy(&retAddr, concMem + address, PointerVal::PointerSize);
  auto addrSpace = AddressSpace::GLOBAL;
  switch (retAddr & AddressSpaceMask) {
  case GlobalAddressSpaceTag:
    addrSpace = AddressSpace::GLOBAL;
    break;
  case StackAddressSpaceTag:
    addrSpace = AddressSpace::STACK;
    break;
  case HeapAddressSpaceTag:
    addrSpace = AddressSpace::HEAP;
    break;
  default:
    throw std::runtime_error("readAsPointer() reads illegal pointer tag");
  }
  return std::make_shared<PointerVal>(retAddr & ~AddressSpaceMask, addrSpace);
}

string Memory::readAsString(unsigned long address) {
  std::stringstream ss;
  while (concMem[address] != 0) {
    ss << concMem[address];
    address++;
  }
  return ss.str();
}

void *Memory::getRawPointerAtAddress(unsigned long address) {
  return concMem + address;
}

void Memory::write(unsigned long address, shared_ptr<DynVal> dynVal) {
  // TODO: delete memory in symbolic storage
  //  symMem.erase(address);
  switch (dynVal->valType) {
  case DynValType::INT_VAL: {
    auto intValue = std::static_pointer_cast<IntVal>(dynVal);
    assert(intValue->intVal.getBitWidth() <= 64 &&
           "Memory::write: >64-bit integer is not supported");
    auto rawData = intValue->intVal.getRawData();
    memcpy(concMem + address, rawData, intValue->intVal.getBitWidth() / 8);
    break;
  }
  case DynValType::FLOAT_VAL: {
    auto floatValue = std::static_pointer_cast<FloatVal>(dynVal);
    if (floatValue->isDouble) {
      memcpy(concMem + address, &floatValue->fpVal, sizeof(double));
    } else {
      memcpy(concMem + address, &floatValue->fpVal, sizeof(float));
    }
    break;
  }
  case DynValType::POINTER_VAL: {
    //  errs() << "Writing " << address << " to " << add << "\n";
    auto pointerValue = std::static_pointer_cast<PointerVal>(dynVal);
    auto addressVal = pointerValue->address;
    switch (pointerValue->space) {
    case AddressSpace::GLOBAL:
      addressVal |= Memory::GlobalAddressSpaceTag;
      break;
    case AddressSpace::STACK:
      addressVal |= Memory::StackAddressSpaceTag;
      break;
    case AddressSpace::HEAP:
      addressVal |= Memory::HeapAddressSpaceTag;
      break;
    }
    memcpy(concMem + address, &addressVal, PointerVal::PointerSize);
    break;
  }
  case DynValType::ARRAY_VAL: {
    //  errs() << "Writing ArrayValue to " << add << "\n";
    auto arrayValue = std::static_pointer_cast<ArrayVal>(dynVal);
    for (vector<shared_ptr<DynVal>>::iterator it = arrayValue->array.begin(),
                                              ie = arrayValue->array.end();
         it != ie; ++it) {
      write(address, (*it));
      address += arrayValue->elemSize;
    }
    break;
  }
  case DynValType::STRUCT_VAL: {
    //  errs() << "Writing StructValue to " << add << "\n";
    auto structValue = std::static_pointer_cast<StructVal>(dynVal);
    for (map<unsigned, shared_ptr<DynVal>>::iterator
             it = structValue->structMap.begin(),
             ie = structValue->structMap.end();
         it != ie; ++it) {
      write(address + it->first, it->second);
    }
    break;
  }
  }
}
