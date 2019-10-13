/*
 * IntVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_INTVAL_H_
#define SRC_EXECUTION_VALUE_INTVAL_H_

#include "SimVal.h"
#include "llvm/ADT/APInt.h"

class IntVal : public SimVal {
public:
  IntVal(const APInt &iv);                        // concrete
  IntVal(unsigned bw, SymExprType set, string n); // symbolic
  virtual ~IntVal();
  unsigned bitWidth;
  APInt intVal;

  virtual unsigned getBitWidth();
  virtual shared_ptr<SimVal> mulMinusOne();
  virtual shared_ptr<SimVal> combineAddOnce();

  virtual void print();
  virtual string toString();
};

#endif /* SRC_EXECUTION_VALUE_INTVAL_H_ */
