/*
 * Searcher.h
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_SEARCHER_SEARCHER_H_
#define SRC_EXECUTION_SEARCHER_SEARCHER_H_

#include "execution/state/ExecutionState.h"
#include <memory>
#include <vector>

using namespace std;

class Searcher {
public:
  Searcher();
  virtual ~Searcher();
  virtual void insertState(shared_ptr<ExecutionState> state) = 0;
  virtual shared_ptr<ExecutionState> getNextState() = 0;
  virtual bool isEmpty() = 0;
};

#endif /* SRC_EXECUTION_SEARCHER_SEARCHER_H_ */
