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
unique_ptr<Solver> SymExecutor::solver = nullptr;

void SymExecutor::initialize(Module *m,
                             const std::vector<std::string> &programParams) {
  searcher = createSearcher(simParams[SimParamType::SEARCH]);
  initialState = createInitialState(m, programParams);
  solver = std::make_unique<Solver>();
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
  shared_ptr<ExecutionState> state = std::make_shared<ExecutionState>();

  // module & layout
  ExecutionState::module = m;
  ExecutionState::dataLayout = std::make_shared<DataLayout>(m);

  // static data
  PointerVal::PointerSize = ExecutionState::dataLayout->getPointerSize();

  // read globals
  for (GlobalVariable &globalVal : ExecutionState::module->getGlobalList()) {
    unsigned size = ExecutionState::dataLayout->getTypeAllocSize(
        globalVal.getType()->getElementType());
    // allocate & write data
    unsigned globalAddr = state->globalMem->allocate(size);
    if (globalVal.hasInitializer()) {
      shared_ptr<DynVal> dynVal = state->evalConst(globalVal.getInitializer());
      state->globalMem->write(globalAddr, dynVal);
    }
    state->globalAddresses.insert(std::make_pair(&globalVal, globalAddr));
  }

  // read functions
  for (Function &f : ExecutionState::module->getFunctionList()) {
    auto size = ExecutionState::dataLayout->getTypeAllocSize(f.getType());
    auto funAddr = state->globalMem->allocate(size);
    state->globalAddresses.insert(std::make_pair(&f, funAddr));
    state->addressToFunc.insert(std::make_pair(funAddr, &f));
  }

  // main arguments include 2 things: argc & argv
  // argc is a value, don't need to write to memory
  state->mainArgs.push_back(
      std::make_shared<IntVal>(APInt(32, programParams.size())));

  // argv & argv[i] are pointer values
  // so we need to write their data to memory
  shared_ptr<ArrayVal> argvData = std::make_shared<ArrayVal>(
      ExecutionState::dataLayout->getPointerSize(), programParams.size());
  for (auto const &argStr : programParams) {
    // write argv[i] data to memory & keep its pointer
    auto argvi = state->globalMem->allocate(argStr.size() + 1);
    argvData->array.push_back(std::make_shared<PointerVal>(
        argvi, AddressSpace::GLOBAL));  // keep pointer
    for (auto const argChar : argStr) { // write data of argv[i]
      state->globalMem->write(argvi,
                              std::make_shared<IntVal>(APInt(8, argChar)));
      ++argvi;
    }
    state->globalMem->write(argvi, std::make_shared<IntVal>(APInt(
                                       8, 0))); // 0 for string termination
  }

  // write argv data to memory & keep its pointer in main arguments
  auto argv = state->globalMem->allocate(
      ExecutionState::dataLayout->getPointerSize() * programParams.size());
  state->globalMem->write(argv, argvData);
  // add argv to main arguments
  state->mainArgs.push_back(
      std::make_shared<PointerVal>(argv, AddressSpace::GLOBAL));
  return std::move(state);
}

void SymExecutor::startSym() {
  auto start = std::chrono::high_resolution_clock::now();

  // prepare 1st stack frame for main
  auto mainFn = ExecutionState::module->getFunction("main");
  initialState->frames.push_back(std::make_unique<StackFrame>(nullptr, mainFn));
  StackFrame *calleeFrame = initialState->frames.back().get();
  unsigned i = 0;
  for (auto it = mainFn->arg_begin(), ie = mainFn->arg_end(); it != ie;
       ++it, ++i) {
    calleeFrame->insertBinding(it, initialState->mainArgs[i]);
  }
  initialState->currentInst = mainFn->getEntryBlock().getFirstNonPHIOrDbg();

  // move initial state to the work list
  searcher->insertState(initialState);

  while (!searcher->isEmpty()) {
    // pop 1 state from the work list & run
    shared_ptr<ExecutionState> state = searcher->getNextState();
    shared_ptr<DynVal> retVal = state->run();

    ++Statistics::noOfpath;

    if (simParams[SimParamType::PRINT_PATH] == "1") {
      errs() << "=== Finished Path ===\n";
      errs() << "Return value from main():\n";
      if (retVal == nullptr) {
        errs() << "void\n";
      } else {
        retVal->print();
        errs() << "\n";
      }
      solver->printModel(state->pcs);
      errs() << "======================\n";
    }
  }

  auto finish = std::chrono::high_resolution_clock::now();
  Statistics::runningTime = finish - start;

  // print statistics info
  Statistics::printInfo();
}
