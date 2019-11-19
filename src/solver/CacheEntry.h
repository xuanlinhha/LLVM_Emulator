/*
 * CacheEntry.h
 *
 *  Created on: 19 Nov 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_SOLVER_CACHEENTRY_H_
#define SRC_SOLVER_CACHEENTRY_H_

#include "Solver.h"
#include <memory>

using namespace std;

class CacheEntry {
public:
  CacheEntry(std::map<shared_ptr<IntVal>, bool> &_pcs, shared_ptr<IntVal> &_q,
             bool _isTrue, SolverResult sr);
  virtual ~CacheEntry();
  std::map<shared_ptr<IntVal>, bool> pcs;
  shared_ptr<IntVal> q;
  bool isTrue;
  SolverResult sr;
  string stringify();
};

#endif /* SRC_SOLVER_CACHEENTRY_H_ */
