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

shared_ptr<SimVal> IntVal::mulMinusOne() { return nullptr; }

shared_ptr<SimVal> IntVal::combineAddOnce() { return nullptr; }

void IntVal::print() {
  if (!isSym) {
    errs() << "(IV, " << bitWidth << ", " << intVal.getSExtValue() << ")";
  } else {
    errs() << "(SIV, " << bitWidth << ", "
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

string IntVal::toString() {
  stringstream ss;
  if (isSym) {
    ss << "(IV, " << bitWidth << ", " << intVal.getSExtValue() << ")";
  } else {
    ss << "(SIV, " << bitWidth << ", " << SimVal::symExprTypeName[symExprType]
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
