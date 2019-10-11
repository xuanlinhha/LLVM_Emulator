/*
 * DynVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_DYNVAL_H_
#define SRC_EXECUTION_VALUE_DYNVAL_H_

#include <llvm/Support/raw_ostream.h>
#include <string>

using namespace std;
using namespace llvm;

enum class DynValType {
  // concrete
  INT_VAL,
  FLOAT_VAL,
  POINTER_VAL,
  SIM_VAL,
  ARRAY_VAL,
  STRUCT_VAL,
  // error & undefined
  ERROR,
  UNDEF_VALUE
};

class DynVal {
public:
  DynVal(DynValType valType);
  virtual ~DynVal();
  DynValType valType;
  virtual void print();
  virtual string toString();
};

#endif /* SRC_EXECUTION_VALUE_DYNVAL_H_ */
