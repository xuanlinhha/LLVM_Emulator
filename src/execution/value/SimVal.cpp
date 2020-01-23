/*
 * SimVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "SimVal.h"

std::map<SymExprType, string> SimVal::symExprTypeName = {
    {SymExprType::INVALID, "INVALID"},
    {SymExprType::VAR, "VAR"},
    // Arithmetic
    {SymExprType::ADD, "ADD"},
    {SymExprType::SUB, "SUB"},
    {SymExprType::MUL, "MUL"},
    {SymExprType::UDIV, "UDIV"},
    {SymExprType::SDIV, "SDIV"},
    {SymExprType::UREM, "UREM"},
    {SymExprType::SREM, "SREM"},
    // Bit
    {SymExprType::NOT, "NOT"},
    {SymExprType::AND, "AND"},
    {SymExprType::OR, "OR"},
    {SymExprType::XOR, "XOR"},
    {SymExprType::SHL, "SHL"},
    {SymExprType::LSHR, "LSHR"},
    {SymExprType::ASHR, "ASHR"},
    //
    {SymExprType::SELECT, "SELECT"},
    {SymExprType::CONCAT, "CONCAT"},
    {SymExprType::EXTRACT, "EXTRACT"},
    // Compare
    {SymExprType::EQ, "EQ"},
    {SymExprType::NE, "NE"},
    {SymExprType::ULT, "ULT"},
    {SymExprType::ULE, "ULE"},
    {SymExprType::UGT, "UGT"},
    {SymExprType::UGE, "UGE"},
    {SymExprType::SLT, "SLT"},
    {SymExprType::SLE, "SLE"},
    {SymExprType::SGT, "SGT"},
    {SymExprType::SGE, "SGE"},
    // Casting
    {SymExprType::ZEXT, "ZEXT"},
    {SymExprType::SEXT, "SEXT"}};

SimVal::SimVal(DynValType vt)
    : DynVal(vt), isSym(false), symExprType(SymExprType::INVALID) {}

SimVal::SimVal(DynValType vt, SymExprType set, string n)
    : DynVal(vt), isSym(true), symExprType(set), name(n) {}

SimVal::~SimVal() {}

unsigned SimVal::getBitWidth() { return 0; }

SimVal *SimVal::mulMinusOne() { return nullptr; }

SimVal *SimVal::combineAddOnce() { return nullptr; }

void SimVal::print(raw_ostream *os) {
  *os << "(SV, " << dynValTypeName[valType] << ", " << isSym << ")";
}
