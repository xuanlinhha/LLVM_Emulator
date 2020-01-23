/*
 * DFS.cpp
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#include "DFS.h"

DFS::DFS() {}

DFS::~DFS() {}

void DFS::insertState(ExecutionState *state) {
  worklist.push_back(std::move(state));
}

ExecutionState *DFS::getNextState() {
  ExecutionState *ret = worklist.back();
  worklist.pop_back();
  return ret;
}

bool DFS::isEmpty() { return worklist.empty(); }
