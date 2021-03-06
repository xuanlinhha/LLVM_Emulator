/*
 * CacheEntry.cpp
 *
 *  Created on: 19 Nov 2019
 *      Author: xuanlinhha
 */

#include "CacheEntry.h"

CacheEntry::CacheEntry(std::map<shared_ptr<IntVal>, bool> &_pcs,
                       shared_ptr<IntVal> &_q, bool _qval)
    : pcs(_pcs), q(_q), qval(_qval) {}

CacheEntry::~CacheEntry() {}

string CacheEntry::stringify() {
  string res;
  raw_string_ostream ss(res);
  for (std::map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(),
                                                    ie = pcs.end();
       it != ie; ++it) {
    it->first->print(&ss);
    ss << "->" << it->second << ";";
  }
  q->print(&ss);
  ss << "->" << qval << ";";
  ss.flush();
  return res;
}
