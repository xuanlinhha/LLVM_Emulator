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

void StructVal::addField(unsigned offset, DynVal *val) {
  assert(!structMap.count(offset) && "StructVal::addField: invalid offset");
  structMap.insert(std::make_pair(offset, std::move(val)));
}

DynVal *StructVal::getFieldAtOffset(unsigned offset) const {
  assert((structMap.size() <= offset) &&
         "StructVal::getFieldAtNum: invalid position");
  return std::next(structMap.begin(), offset)->second;
}

void StructVal::print(raw_ostream *os) {
  errs() << "(SV, {";
  for (map<unsigned, DynVal *>::iterator it = structMap.begin(),
                                         ie = structMap.end();
       it != ie; ++it) {
    it->second->print(os);
    if (std::next(it) != structMap.end()) {
      *os << ", ";
    }
  }
  *os << "})";
}
