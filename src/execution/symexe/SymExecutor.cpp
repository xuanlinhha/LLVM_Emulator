/*
 * SymExecutor.cpp
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#include "SymExecutor.h"

SymExecutor::SymExecutor() {}

SymExecutor::~SymExecutor() {}

unique_ptr<Searcher> SymExecutor::searcher = nullptr;
shared_ptr<ExecutionState> SymExecutor::initialState = nullptr;
unique_ptr<Z3Solver> SymExecutor::z3Solver = nullptr;

void SymExecutor::initialize(Module *m,
                             const std::vector<std::string> &programParams) {
  searcher = createSearcher(simulatorParams[SimulatorParamType::SEARCH]);
  initialState = createInitialState(m, programParams);
  z3Solver = std::make_unique<Z3Solver>();
}

unique_ptr<Searcher> SymExecutor::createSearcher(string searchStrategy) {
  if (searchStrategy == "DFS") {
    errs() << "Using DFS Searcher\n";
    return std::make_unique<DFS>();
  } else {
    errs() << "Using Default Searcher\n";
    return std::make_unique<DFS>();
  }
}

shared_ptr<ExecutionState>
SymExecutor::createInitialState(Module *m,
                                const std::vector<std::string> &programParams) {
  return nullptr;
}
