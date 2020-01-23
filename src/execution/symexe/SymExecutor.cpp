/*
 * SymExecutor.cpp
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#include "SymExecutor.h"

SymExecutor::SymExecutor() {}

SymExecutor::~SymExecutor() {}

ExecutionState *SymExecutor::initialState = nullptr;
Searcher *SymExecutor::searcher = nullptr;
Solver *SymExecutor::solver = nullptr;
unsigned SymExecutor::assertFailLimit = 0;
unsigned SymExecutor::assertFailCounter = 0;
bool SymExecutor::isStop = false;

void SymExecutor::initialize(Module *m,
                             const std::vector<std::string> &programParams) {
  searcher = createSearcher(simParams[SimParamType::SEARCH]);
  initialState = createInitialState(m, programParams);
  solver = new Solver();
  if (!simParams[SimParamType::ASSERTION_FAIL_LIMIT].empty()) {
    assertFailLimit =
        std::stoul(simParams[SimParamType::ASSERTION_FAIL_LIMIT], nullptr, 0);
  }
}

Searcher *SymExecutor::createSearcher(string searchStrategy) {
  if (searchStrategy == "DFS") {
    errs() << "Using DFS Searcher\n";
    return new DFS();
  } else {
    errs() << "Using Default Searcher\n";
    return new DFS();
  }
}

ExecutionState *
SymExecutor::createInitialState(Module *m,
                                const std::vector<std::string> &programParams) {
  ExecutionState *state = new ExecutionState();

  // module & layout
  ExecutionState::module = m;
  ExecutionState::dataLayout = new DataLayout(m);

  // static data
  PointerVal::PointerSize = ExecutionState::dataLayout->getPointerSize();

  // read globals
  for (GlobalVariable &globalVal : ExecutionState::module->getGlobalList()) {
    unsigned size = ExecutionState::dataLayout->getTypeAllocSize(
        globalVal.getType()->getElementType());
    // allocate & write data
    unsigned globalAddr = state->globalMem->allocate(size);
    if (globalVal.hasInitializer()) {
      DynVal *dynVal = state->evalConst(globalVal.getInitializer());
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
  state->mainArgs.push_back(new IntVal(APInt(32, programParams.size())));

  // argv & argv[i] are pointer values
  // so we need to write their data to memory
  ArrayVal *argvData = new ArrayVal(
      ExecutionState::dataLayout->getPointerSize(), programParams.size());
  for (auto const &argStr : programParams) {
    // write argv[i] data to memory & keep its pointer
    auto argvi = state->globalMem->allocate(argStr.size() + 1);
    argvData->array.push_back(
        new PointerVal(argvi, AddressSpace::GLOBAL)); // keep pointer
    for (auto const argChar : argStr) {               // write data of argv[i]
      state->globalMem->write(argvi, new IntVal(APInt(8, argChar)));
      ++argvi;
    }
    state->globalMem->write(
        argvi, new IntVal(APInt(8, 0))); // 0 for string termination
  }

  // write argv data to memory & keep its pointer in main arguments
  auto argv = state->globalMem->allocate(
      ExecutionState::dataLayout->getPointerSize() * programParams.size());
  state->globalMem->write(argv, argvData);
  // add argv to main arguments
  state->mainArgs.push_back(new PointerVal(argv, AddressSpace::GLOBAL));
  return std::move(state);
}

void SymExecutor::startSym() {
  auto start = std::chrono::high_resolution_clock::now();

  // prepare 1st stack frame for main
  auto mainFn = ExecutionState::module->getFunction("main");
  initialState->frames.push_back(new StackFrame(nullptr, mainFn));
  StackFrame *calleeFrame = initialState->frames.back();
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
    ExecutionState *state = searcher->getNextState();
    DynVal *retVal = state->run();

    ++Statistics::pathCounter;

    if (simParams[SimParamType::PRINT_PATH] == "1") {
      errs() << "=== Finished Path ===\n";
      errs() << "Return value from main():\n";
      if (retVal == nullptr) {
        errs() << "void\n";
      } else {
        retVal->print(&errs());
        errs() << "\n";
      }
      solver->printModel(state->pcs);
      errs() << "======================\n";
    }

    // stop condition
    if (isStop) {
      break;
    }
  }

  auto finish = std::chrono::high_resolution_clock::now();
  Statistics::runningTime = finish - start;

  // print statistics info
  Statistics::printInfo();
}
