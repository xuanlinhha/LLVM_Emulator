/*
 * PointerVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_POINTERVAL_H_
#define SRC_EXECUTION_VALUE_POINTERVAL_H_

#include "SimVal.h"

enum class AddressSpace { GLOBAL = 0, STACK, HEAP, UNDEFINED };

class PointerVal : public SimVal {
public:
  PointerVal(unsigned a, AddressSpace s); // concrete
  PointerVal(SymExprType set, string n);  // symbolic
  virtual ~PointerVal();
  static unsigned PointerSize;
  unsigned long address;
  AddressSpace space;

  virtual unsigned getBitWidth();
  virtual SimVal *mulMinusOne();
  virtual SimVal *combineAddOnce();

  virtual void print(raw_ostream *os);
};

#endif /* SRC_EXECUTION_VALUE_POINTERVAL_H_ */
