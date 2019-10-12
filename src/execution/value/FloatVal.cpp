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
