/*
 * FloatVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "FloatVal.h"

FloatVal::FloatVal(double fv, bool isd)
    : SimVal(DynValType::FLOAT_VAL), fpVal(fv), isDouble(isd) {}

FloatVal::FloatVal(SymExprType set, string n, bool isd)
    : SimVal(DynValType::FLOAT_VAL, set, n), fpVal(0.0), isDouble(isd) {}

FloatVal::~FloatVal() {}

unsigned FloatVal::getBitWidth() {
  if (isDouble) {
    return sizeof(double);
  } else {
    return sizeof(float);
  }
}

SimVal *FloatVal::mulMinusOne() { return nullptr; }

SimVal *FloatVal::combineAddOnce() { return nullptr; }

void FloatVal::print(raw_ostream *os) {
  if (!isSym) {
    *os << "(FV, " << isDouble << ", " << fpVal << ")";
  } else {
    *os << "(SFV, " << isDouble << ", " << SimVal::symExprTypeName[symExprType]
        << "(";
    if (symExprType == SymExprType::VAR) {
      *os << name << ")";
      ;
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
