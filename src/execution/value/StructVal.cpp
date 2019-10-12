/*
 * StructVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "StructVal.h"

StructVal::StructVal(unsigned sz)
    : DynVal(DynValType::STRUCT_VAL), structSize(sz) {}

StructVal::~StructVal() {}

void StructVal::addField(unsigned offset, shared_ptr<DynVal> val) {
  assert(!structMap.count(offset) && "StructVal::addField: invalid offset");
  structMap.insert(std::make_pair(offset, std::move(val)));
}

shared_ptr<DynVal> StructVal::getFieldAtPos(unsigned pos) const {
  assert((structMap.size() <= pos) &&
         "StructVal::getFieldAtNum: invalid position");
  return std::next(structMap.begin(), pos)->second;
}
