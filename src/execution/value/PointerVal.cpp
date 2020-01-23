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

SimVal *PointerVal::mulMinusOne() { return nullptr; }

SimVal *PointerVal::combineAddOnce() { return nullptr; }

void PointerVal::print(raw_ostream *os) {
  if (!isSym) {
    *os << "(PV, " << address << ",";
    if (space == AddressSpace::GLOBAL) {
      *os << "GLOBAL)";
    } else if (space == AddressSpace::STACK) {
      *os << "STACK)";
    } else if (space == AddressSpace::HEAP) {
      *os << "HEAP)";
    } else {
      *os << "UNDEFINED)";
    }
  } else {
    // TODO: identify space to print
  }
}
