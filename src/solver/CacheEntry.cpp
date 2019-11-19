/*
 * CacheEntry.cpp
 *
 *  Created on: 19 Nov 2019
 *      Author: xuanlinhha
 */

#include "CacheEntry.h"

CacheEntry::CacheEntry(std::map<shared_ptr<IntVal>, bool> &_pcs,
                       shared_ptr<IntVal> &_q, bool _isTrue, SolverResult _sr)
    : pcs(_pcs), q(_q), isTrue(_isTrue), sr(_sr) {}

CacheEntry::~CacheEntry() {}

string CacheEntry::stringify() {
  stringstream ss;
  for (std::map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(),
                                                    ie = pcs.end();
       it != ie; ++it) {
    ss << it->first->toString() << "->" << it->second << ";";
  }
  ss << q->toString() << "->" << isTrue << ";";
  return ss.str();
}
