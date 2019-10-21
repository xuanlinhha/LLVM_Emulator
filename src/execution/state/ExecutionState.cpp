/*
 * ExecutionState.cpp
 *
 *  Created on: 14 Oct 2019
 *      Author: xuanlinhha
 */

#include "ExecutionState.h"

Module *ExecutionState::module = nullptr;
shared_ptr<DataLayout> ExecutionState::dataLayout = nullptr;

ExecutionState::ExecutionState()
    : globalMem(new Memory()), stackMem(new Memory()), heapMem(new Memory()),
      currentInst(nullptr), incomingBB(nullptr), isError(false) {}

ExecutionState::~ExecutionState() {}

shared_ptr<DynVal> ExecutionState::run() {}

shared_ptr<ExecutionState> ExecutionState::clone() {
  shared_ptr<ExecutionState> res = std::make_shared<ExecutionState>();
  res->globalAddresses = globalAddresses;
  res->addressToFunc = addressToFunc;
  res->currentInst = currentInst;
  res->incomingBB = incomingBB;
  res->globalMem = globalMem->clone();
  res->stackMem = stackMem->clone();
  res->heapMem = heapMem->clone();
  for (vector<unique_ptr<StackFrame>>::iterator it = frames.begin(),
                                                ie = frames.end();
       it != ie; ++it) {
    res->frames.push_back((*it)->clone());
  }
  res->pcs = pcs;
  res->isError = isError;
  return std::move(res);
}

void ExecutionState::popStack() {
  stackMem->deallocate(frames.back()->allocSize);
  assert(!frames.empty());
  frames.pop_back();
}

void ExecutionState::emptyStack() {
  while (!frames.empty()) {
    frames.pop_back();
  }
}
