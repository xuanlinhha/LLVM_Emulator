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

void PointerVal::print() {
  if (!isSym) {
    errs() << "(PV, " << address << ",";
    if (space == AddressSpace::GLOBAL) {
      errs() << "GLOBAL)";
    } else if (space == AddressSpace::STACK) {
      errs() << "STACK)";
    } else if (space == AddressSpace::HEAP) {
      errs() << "HEAP)";
    } else {
      errs() << "UNDEFINED)";
    }
  } else {
    // TODO: identify space to print
  }
}

string PointerVal::toString() {
  std::stringstream ss;
  if (!isSym) {
    ss << "(PV, " << address << ",";
    if (space == AddressSpace::GLOBAL) {
      ss << "GLOBAL)";
    } else if (space == AddressSpace::STACK) {
      ss << "STACK)";
    } else if (space == AddressSpace::HEAP) {
      ss << "HEAP)";
    } else {
      ss << "UNDEFINED)";
    }
  } else {
    // TODO: identify space to print
  }
  return ss.str();
}
