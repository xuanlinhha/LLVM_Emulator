/*
 * DFS.cpp
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#include "DFS.h"

DFS::DFS() {}

DFS::~DFS() {}

void DFS::insertState(shared_ptr<ExecutionState> state) {
  worklist.push_back(std::move(state));
}

shared_ptr<ExecutionState> DFS::getNextState() {
  shared_ptr<ExecutionState> ret = worklist.back();
  worklist.pop_back();
  return std::move(ret);
}

bool DFS::isEmpty() { return worklist.empty(); }
