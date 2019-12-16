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
                            shared_ptr<IntVal> &q, bool isTrue) {
  SolverResult cr;
  // check in cache first, if not then call Z3 solver & add result to cache
  bool isInCache = cacheSolver->getEntryResult(pcs, q, !isTrue, cr);
  if (!isInCache) {
    cr = z3Solver->check(pcs, q, !isTrue);
    cacheSolver->addEntry(pcs, q, !isTrue, cr);
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

void Solver::printModel(std::map<shared_ptr<IntVal>, bool> &pcs) {
  z3Solver->printModel(pcs);
}
