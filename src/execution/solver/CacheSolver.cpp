/*
 * CacheSolver.cpp
 *
 *  Created on: 15 Nov 2019
 *      Author: xuanlinhha
 */

#include "CacheSolver.h"

CacheSolver::CacheSolver() {}

CacheSolver::~CacheSolver() {}

void CacheSolver::addEntry(std::map<shared_ptr<IntVal>, bool> &pcs,
                           shared_ptr<IntVal> &q, bool qval, SolverResult sr) {
  unique_ptr<CacheEntry> entry = std::make_unique<CacheEntry>(pcs, q, qval);
  string key = entry->stringify();
  data.insert(std::make_pair(std::move(entry), sr));
  stringCache.insert(std::make_pair(key, sr));
  //  errs() << "Insert Entry with pcs size = " << pcs.size() << "\n";
}

bool CacheSolver::getEntryResult(std::map<shared_ptr<IntVal>, bool> &pcs,
                                 shared_ptr<IntVal> &q, bool qval,
                                 SolverResult &sr) {
  unique_ptr<CacheEntry> entry = std::make_unique<CacheEntry>(pcs, q, qval);
  map<string, SolverResult>::iterator it = stringCache.find(entry->stringify());
  if (it != stringCache.end()) {
    sr = it->second;
    return true;
  } else {
    //  errs() << "Get Entry Fail with pcs size = " << pcs.size() << "\n";
    return false;
  }
}
