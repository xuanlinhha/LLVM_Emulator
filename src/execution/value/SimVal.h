/*
 * SimVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_SIMVAL_H_
#define SRC_EXECUTION_VALUE_SIMVAL_H_

#include "DynVal.h"

enum class SymExprType {
  INVALID = -1,
  VAR = 0,
  // Arithmetic
  ADD,
  SUB,
  MUL,
  UDIV,
  SDIV,
  UREM,
  SREM,
  // Bit
  NOT,
  AND,
  OR,
  XOR,
  SHL,
  LSHR,
  ASHR,
  //
  SELECT,
  CONCAT,
  EXTRACT,
  // Compare
  EQ,
  NE,
  ULT,
  ULE,
  UGT,
  UGE,
  SLT,
  SLE,
  SGT,
  SGE,
  // Casting,
  ZEXT,
  SEXT,

};

class SimVal : public DynVal {
public:
  SimVal(DynValType vt, bool sym);
  SimVal(DynValType vt, bool sym, SymExprType set, string n = "");
  virtual ~SimVal();
  bool isSym;
  SymExprType symExprType;
  string name;
  vector<shared_ptr<SimVal>> operands;

  virtual unsigned getBitWidth();
  virtual shared_ptr<SimVal> mulMinusOne();
  virtual shared_ptr<SimVal> combineAddOnce();
};

#endif /* SRC_EXECUTION_VALUE_SIMVAL_H_ */