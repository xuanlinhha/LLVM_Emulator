/*
 * CacheSolver.h
 *
 *  Created on: 15 Nov 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_SOLVER_CACHESOLVER_H_
#define SRC_SOLVER_CACHESOLVER_H_

#include "CacheEntry.h"
#include "ResultType.h"
#include "execution/value/IntVal.h"

using namespace std;

class CacheSolver {
public:
  CacheSolver();
  virtual ~CacheSolver();
  map<CacheEntry *, SolverResult> data;
  map<string, SolverResult> stringCache;
  void addEntry(std::map<IntVal *, bool> &pcs, IntVal *&q, bool qval,
                SolverResult sr);
  bool getEntryResult(std::map<IntVal *, bool> &pcs, IntVal *&q, bool qval,
                      SolverResult &sr);
};

#endif /* SRC_SOLVER_CACHESOLVER_H_ */
