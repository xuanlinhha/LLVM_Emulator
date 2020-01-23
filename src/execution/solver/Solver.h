/*
 * Solver.h
 *
 *  Created on: 15 Nov 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_SOLVER_SOLVER_H_
#define SRC_SOLVER_SOLVER_H_

#include "CacheSolver.h"
#include "ResultType.h"
#include "Z3Solver.h"
#include "execution/helper/SymValHelper.h"
#include "execution/simplify/PCSSimplifier.h"
#include "execution/symexe/Statistics.h"
#include "execution/value/IntVal.h"

class Solver {
public:
  Solver();
  virtual ~Solver();
  CacheSolver *cacheSolver;
  Z3Solver *z3Solver;
  ValidResult isValid(map<IntVal *, bool> &pcs, IntVal *&q, bool qval);
  bool isPossible(map<IntVal *, bool> &pcs, IntVal *&assume, bool assumeVal);
  void printModel(std::map<IntVal *, bool> &pcs);
};

#endif /* SRC_SOLVER_SOLVER_H_ */
