/*
 * Memory.h
 *
 *  Created on: 14 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_STATE_MEMORY_H_
#define SRC_EXECUTION_STATE_MEMORY_H_

#include "execution/value/ArrayVal.h"
#include "execution/value/DynVal.h"
#include "execution/value/FloatVal.h"
#include "execution/value/IntVal.h"
#include "execution/value/PointerVal.h"
#include "execution/value/StructVal.h"
#include <string>

class Memory {
public:
  Memory();
  Memory(unsigned ts, unsigned us, unsigned char *m,
         map<unsigned long, DynVal *> &sm);
  virtual ~Memory();
  static const unsigned DEFAULT_SIZE = 0x100000;
  // 11 - Undefined
  static const unsigned long AddressSpaceMask = 0xC000000000000000;
  // 00 - Global
  static const unsigned long GlobalAddressSpaceTag = 0;
  // 01 - Stack
  static const unsigned long StackAddressSpaceTag = 0x4000000000000000;
  // 10 - Heap
  static const unsigned long HeapAddressSpaceTag = 0x8000000000000000;

  // storage
  unsigned totalSize, usedSize;
  unsigned char *concMem;
  map<unsigned long, DynVal *> symMem;

  // operations
  void grow();
  bool isAddressLegal(unsigned long address) const;
  unsigned allocate(unsigned size);
  void deallocate(unsigned size);
  void free(unsigned long address, unsigned size);
  DynVal *readAsInt(unsigned long address, unsigned bitWidth) const;
  DynVal *readAsFloat(unsigned long address, bool isDouble = true) const;
  DynVal *readAsPointer(unsigned long address) const;
  string readAsString(unsigned long address);
  void *getRawPointerAtAddress(unsigned long addr);
  void write(unsigned long address, const DynVal *dynVal);
  Memory *clone();
};

#endif /* SRC_EXECUTION_STATE_MEMORY_H_ */
