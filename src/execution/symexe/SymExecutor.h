/*
 * SymExecutor.h
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_SYMEXE_SYMEXECUTOR_H_
#define SRC_EXECUTION_SYMEXE_SYMEXECUTOR_H_

#include "Statistics.h"
#include "execution/searcher/DFS.h"
#include "solver/Z3Solver.h"
#include <llvm/IR/Module.h>

enum class SimulatorParamType { SEARCH, SOLVER, PRINT_PATH };

class ExecutionState;

class SymExecutor {
public:
  SymExecutor();
  virtual ~SymExecutor();
  map<SimulatorParamType, string> simulatorParams;
  static unique_ptr<Searcher> searcher;
  static shared_ptr<ExecutionState> initialState;
  static unique_ptr<Z3Solver> z3Solver;
  void initialize(Module *m, const vector<string> &programParams);
  unique_ptr<Searcher> createSearcher(string searchStrategy);
  shared_ptr<ExecutionState>
  createInitialState(Module *m, const vector<string> &programParams);
  void startSym();
};

#endif /* SRC_EXECUTION_SYMEXE_SYMEXECUTOR_H_ */
