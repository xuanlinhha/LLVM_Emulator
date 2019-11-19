/*
 * Z3Solver.h
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_SOLVER_Z3SOLVER_H_
#define SRC_SOLVER_Z3SOLVER_H_

#include "execution/symexe/Statistics.h"
#include "execution/value/IntVal.h"
#include "z3++.h"

using namespace z3;

enum class SolverResult { SAT, UNSAT, UNKNOWN };

class Z3Solver {
public:
  Z3Solver();
  virtual ~Z3Solver();
  context c;
  solver s;
  expr generateZ3Expr(shared_ptr<SimVal> ex, context &c);
  SolverResult check(std::map<shared_ptr<IntVal>, bool> &pcs,
                     shared_ptr<IntVal> &q, bool isTrue);
  void printModel(std::map<shared_ptr<IntVal>, bool> &pcs);
};

#endif /* SRC_SOLVER_Z3SOLVER_H_ */
