/*
 * PointerVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "PointerVal.h"

unsigned PointerVal::PointerSize = 8u;

PointerVal::PointerVal(unsigned a, AddressSpace s)
    : SimVal(DynValType::POINTER_VAL), address(a), space(s) {}

PointerVal::PointerVal(SymExprType set, string n)
    : SimVal(DynValType::POINTER_VAL, set, n), address(0),
      space(AddressSpace::UNDEFINED) {}

PointerVal::~PointerVal() {}

unsigned PointerVal::getBitWidth() { return PointerSize; }

shared_ptr<SimVal> PointerVal::mulMinusOne() { return nullptr; }

shared_ptr<SimVal> PointerVal::combineAddOnce() { return nullptr; }
