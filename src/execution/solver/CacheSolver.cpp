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
                           shared_ptr<IntVal> &q, bool isTrue,
                           SolverResult sr) {
  unique_ptr<CacheEntry> entry =
      std::make_unique<CacheEntry>(pcs, q, isTrue, sr);
  stringCache.insert(std::make_pair(entry->stringify(), entry->sr));
  data.push_back(std::move(entry));
}

bool CacheSolver::getEntryResult(std::map<shared_ptr<IntVal>, bool> &pcs,
                                 shared_ptr<IntVal> &q, bool isTrue,
                                 SolverResult &sr) {
  unique_ptr<CacheEntry> entry =
      std::make_unique<CacheEntry>(pcs, q, isTrue, SolverResult::UNKNOWN);
  map<string, SolverResult>::iterator it = stringCache.find(entry->stringify());
  if (it != stringCache.end()) {
    sr = it->second;
    return true;
  } else {
    return false;
  }
}
