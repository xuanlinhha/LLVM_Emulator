/*
 * IntVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "IntVal.h"

IntVal::IntVal(const APInt &iv)
    : SimVal(DynValType::INT_VAL), intVal(iv), bitWidth(iv.getBitWidth()) {}

IntVal::IntVal(unsigned bw, SymExprType set, string n)
    : SimVal(DynValType::INT_VAL, set, n), bitWidth(bw) {}

IntVal::~IntVal() {}

unsigned IntVal::getBitWidth() { return bitWidth; }

SimVal *IntVal::mulMinusOne() { return nullptr; }

SimVal *IntVal::combineAddOnce() { return nullptr; }

void IntVal::print(raw_ostream *os) {
  if (!isSym) {
    *os << "(IV, " << bitWidth << ", " << intVal.getSExtValue() << ")";
  } else {
    *os << "(SIV, " << bitWidth << ", " << SimVal::symExprTypeName[symExprType]
        << "(";
    if (symExprType == SymExprType::VAR) {
      *os << name << ")";
    } else if (!operands.empty()) {
      for (vector<SimVal *>::iterator it = operands.begin(),
                                      ie = operands.end();
           it != ie; ++it) {
        (*it)->print(os);
        if (std::next(it) != operands.end()) {
          *os << ", ";
        }
      }
      *os << ")";
    }
    *os << ")";
  }
}
