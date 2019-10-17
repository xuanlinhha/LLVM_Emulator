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
