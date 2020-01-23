/*
 * Solver.cpp
 *
 *  Created on: 15 Nov 2019
 *      Author: xuanlinhha
 */

#include "Solver.h"

Solver::Solver() {
  z3Solver = new Z3Solver();
  cacheSolver = new CacheSolver();
}

Solver::~Solver() {}

// Check for all X: C(X) => q(X) is True
// <=> exist X: C(X) && !q(X) is False
// <=> C(X) && !q(X) is UNSAT
ValidResult Solver::isValid(std::map<IntVal *, bool> &pcs, IntVal *&q,
                            bool qval) {
  SolverResult cr;
  // check in cache first, if not then call Z3 solver & add result to cache
  bool isInCache = cacheSolver->getEntryResult(pcs, q, !qval, cr);
  if (!isInCache) {
    cr = z3Solver->check(pcs, q, !qval);
    cacheSolver->addEntry(pcs, q, !qval, cr);
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

bool Solver::isPossible(map<IntVal *, bool> &pcs, IntVal *&assume,
                        bool assumeVal) {
  SolverResult cr;
  // check in cache first, if not then call Z3 solver & add result to cache
  bool isInCache = cacheSolver->getEntryResult(pcs, assume, assumeVal, cr);
  if (!isInCache) {
    cr = z3Solver->check(pcs, assume, assumeVal);
    cacheSolver->addEntry(pcs, assume, assumeVal, cr);
  } else {
    ++Statistics::cacheHitCounter;
  }

  if (cr == SolverResult::UNSAT) {
    return false;
  } else {
    return true;
  }
}

void Solver::printModel(std::map<IntVal *, bool> &pcs) {
  z3Solver->printModel(pcs);
}
