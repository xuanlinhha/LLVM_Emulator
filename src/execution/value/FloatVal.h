/*
 * FloatVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_FLOATVAL_H_
#define SRC_EXECUTION_VALUE_FLOATVAL_H_

#include "SimVal.h"

class FloatVal : public SimVal {
public:
  FloatVal(double fv, bool isd);                 // concrete
  FloatVal(SymExprType set, string n, bool isd); // symbolic
  virtual ~FloatVal();
  double fpVal;
  bool isDouble;

  virtual unsigned getBitWidth();
  virtual SimVal *mulMinusOne();
  virtual SimVal *combineAddOnce();

  virtual void print(raw_ostream *os);
};

#endif /* SRC_EXECUTION_VALUE_FLOATVAL_H_ */
