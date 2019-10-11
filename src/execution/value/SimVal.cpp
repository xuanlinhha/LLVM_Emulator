/*
 * SimVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "SimVal.h"

SimVal::SimVal(DynValType vt, bool sym)
    : DynVal(vt), isSym(sym), symExprType(SymExprType::INVALID) {}

SimVal::SimVal(DynValType vt, bool sym, SymExprType set, string n)
    : DynVal(vt), isSym(sym), symExprType(set), name(n) {}

SimVal::~SimVal() {
  // TODO Auto-generated destructor stub
}
