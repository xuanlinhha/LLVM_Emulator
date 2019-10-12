/*
 * DynVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "DynVal.h"

std::map<DynValType, string> DynVal::dynValTypeName = {
    {DynValType::INT_VAL, "INT_VAL"},
    {DynValType::FLOAT_VAL, "FLOAT_VAL"},
    {DynValType::POINTER_VAL, "POINTER_VAL"},
    {DynValType::SIM_VAL, "SIM_VAL"},
    {DynValType::ARRAY_VAL, "ARRAY_VAL"},
    {DynValType::STRUCT_VAL, "STRUCT_VAL"},
    {DynValType::ERROR, "ERROR"},
    {DynValType::UNDEF_VAL, "UNDEF_VAL"},
};

DynVal::DynVal(DynValType vt) : valType(vt) {}

DynVal::~DynVal() {}

void DynVal::print() { errs() << "(DV, " << dynValTypeName[valType] << ")"; };

string DynVal::toString() {
  std::stringstream ss;
  ss << "(DV, " << dynValTypeName[valType] << ")";
  return ss.str();
}
