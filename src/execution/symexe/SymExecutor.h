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
#include "execution/solver/Solver.h"
#include "execution/state/ExecutionState.h"
#include <llvm/IR/Module.h>

enum class SimParamType { SEARCH, SOLVER, PRINT_PATH, ASSERTION_FAIL_LIMIT };

class SymExecutor {
public:
  SymExecutor();
  virtual ~SymExecutor();
  map<SimParamType, string> simParams;
  static ExecutionState *initialState;
  static Searcher *searcher;
  static Solver *solver;
  static unsigned assertFailLimit;
  static unsigned assertFailCounter;
  static bool isStop;
  void initialize(Module *m, const vector<string> &programParams);
  Searcher *createSearcher(string searchStrategy);
  ExecutionState *createInitialState(Module *m,
                                     const vector<string> &programParams);
  void startSym();
};

#endif /* SRC_EXECUTION_SYMEXE_SYMEXECUTOR_H_ */
