/*
 * Solver.h
 *
 *  Created on: 15 Nov 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_SOLVER_SOLVER_H_
#define SRC_SOLVER_SOLVER_H_

//#include "CacheSolver.h"
#include "Z3Solver.h"
#include "execution/symexe/Statistics.h"
#include "execution/value/IntVal.h"

enum class ValidResult { YES, NO, UNKNOWN };

class Solver {
public:
  Solver();
  virtual ~Solver();
  //  unique_ptr<CacheSolver> cacheSolver;
  unique_ptr<Z3Solver> z3Solver;
  ValidResult isValid(std::map<shared_ptr<IntVal>, bool> &pcs,
                      shared_ptr<IntVal> &q, bool isTrue);
  void printModel(std::map<shared_ptr<IntVal>, bool> &pcs);
};

#endif /* SRC_SOLVER_SOLVER_H_ */
