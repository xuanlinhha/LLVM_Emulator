/*
 * Z3Solver.h
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_SOLVER_Z3SOLVER_H_
#define SRC_SOLVER_Z3SOLVER_H_

#include "ResultType.h"
#include "execution/symexe/Statistics.h"
#include "execution/value/IntVal.h"
#include "z3++.h"

using namespace z3;

class Z3Solver {
public:
  Z3Solver();
  virtual ~Z3Solver();
  context c;
  solver s;
  expr generateZ3Expr(SimVal *ex, context &c);
  SolverResult check(std::map<IntVal *, bool> &pcs, IntVal *&q, bool isTrue);
  void printModel(std::map<IntVal *, bool> &pcs);

private:
  string getStrResult(SolverResult sr);
};

#endif /* SRC_SOLVER_Z3SOLVER_H_ */
