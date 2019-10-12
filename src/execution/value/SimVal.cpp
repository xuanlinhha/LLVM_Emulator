/*
 * SimVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "SimVal.h"

SimVal::SimVal(DynValType vt)
    : DynVal(vt), isSym(false), symExprType(SymExprType::INVALID) {}

SimVal::SimVal(DynValType vt, SymExprType set, string n)
    : DynVal(vt), isSym(true), symExprType(set), name(n) {}

SimVal::~SimVal() {}
