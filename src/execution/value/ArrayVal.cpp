/*
 * ArrayVal.cpp
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#include "ArrayVal.h"

ArrayVal::ArrayVal(unsigned es, unsigned as)
    : DynVal(DynValType::ARRAY_VAL), elemSize(es), arraySize(as) {}

ArrayVal::~ArrayVal() {}

void ArrayVal::setElementAtIndex(unsigned idx, shared_ptr<DynVal> val) {
  assert(idx < array.size());
  array[idx] = std::move(val);
}

shared_ptr<DynVal> ArrayVal::getElementAtIndex(unsigned idx) const {
  return array.at(idx);
}

void ArrayVal::print(raw_ostream *os) {
  *os << "(AV, [";
  for (vector<shared_ptr<DynVal>>::iterator it = array.begin(),
                                            ie = array.end();
       it != ie; ++it) {
    (*it)->print(os);
    if (std::next(it) != array.end()) {
      *os << ", ";
    }
  }
  *os << "])";
}
