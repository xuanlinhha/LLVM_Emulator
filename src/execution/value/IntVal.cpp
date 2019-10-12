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
