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

shared_ptr<SimVal> FloatVal::mulMinusOne() { return nullptr; }

shared_ptr<SimVal> FloatVal::combineAddOnce() { return nullptr; }

void FloatVal::print() {
  if (!isSym) {
    errs() << "(FV, " << isDouble << ", " << fpVal << ")";
  } else {
    errs() << "(SFV, " << isDouble << ", "
           << SimVal::symExprTypeName[symExprType] << "(";
    if (symExprType == SymExprType::VAR) {
      errs() << name;
    } else if (!operands.empty()) {
      for (vector<shared_ptr<SimVal>>::iterator it = operands.begin(),
                                                ie = operands.end();
           it != ie; ++it) {
        (*it)->print();
        if (std::next(it) != operands.end()) {
          errs() << ", ";
        }
      }
    }
    errs() << ")";
  }
}

string FloatVal::toString() {
  stringstream ss;
  if (!isSym) {
    ss << "(FV, " << isDouble << ", " << fpVal << ")";
  } else {
    ss << "(SFV, " << isDouble << ", " << SimVal::symExprTypeName[symExprType]
       << "(";
    if (symExprType == SymExprType::VAR) {
      ss << name;
    } else if (!operands.empty()) {
      for (vector<shared_ptr<SimVal>>::iterator it = operands.begin(),
                                                ie = operands.end();
           it != ie; ++it) {
        ss << (*it)->toString();
        if (std::next(it) != operands.end()) {
          ss << ", ";
        }
      }
    }
    ss << ")";
  }
  return ss.str();
}
