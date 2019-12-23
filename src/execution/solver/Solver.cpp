/*
 * Solver.cpp
 *
 *  Created on: 15 Nov 2019
 *      Author: xuanlinhha
 */

#include "Solver.h"

Solver::Solver() {
  z3Solver = std::make_unique<Z3Solver>();
  cacheSolver = std::make_unique<CacheSolver>();
}

Solver::~Solver() {}

// Check for all X: C(X) => q(X) is True
// <=> exist X: C(X) && !q(X) is False
// <=> C(X) && !q(X) is UNSAT
ValidResult Solver::isValid(std::map<shared_ptr<IntVal>, bool> &pcs,
                            shared_ptr<IntVal> &q, bool qval) {
  map<shared_ptr<IntVal>, bool> newPcs = getDependentConstraints(pcs, q);

  SolverResult cr;
  // check in cache first, if not then call Z3 solver & add result to cache
  bool isInCache = cacheSolver->getEntryResult(newPcs, q, !qval, cr);
  if (!isInCache) {
    cr = z3Solver->check(newPcs, q, !qval);
    cacheSolver->addEntry(newPcs, q, !qval, cr);
  } else {
    ++Statistics::cacheHitCounter;
  }

  if (cr == SolverResult::UNKNOWN) {
    return ValidResult::UNKNOWN;
  } else if (cr == SolverResult::UNSAT) {
    return ValidResult::YES;
  } else {
    return ValidResult::NO;
  }
}

map<shared_ptr<IntVal>, bool>
Solver::getDependentConstraints(map<shared_ptr<IntVal>, bool> &pcs,
                                shared_ptr<IntVal> &q) {

  // extract dependent constraints
  vector<shared_ptr<IntVal>> worklist;
  for (map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(), ie = pcs.end();
       it != ie; ++it) {
    worklist.push_back(it->first);
  }
  vector<shared_ptr<IntVal>> dependentEles =
      SymValHelper::getDependentEles(worklist, q);
  map<shared_ptr<IntVal>, bool> result;
  for (vector<shared_ptr<IntVal>>::iterator it = dependentEles.begin(),
                                            ie = dependentEles.end();
       it != ie; ++it) {
    result[*it] = pcs[*it];
  }
  return result;
}

void Solver::printModel(std::map<shared_ptr<IntVal>, bool> &pcs) {
  z3Solver->printModel(pcs);
}
