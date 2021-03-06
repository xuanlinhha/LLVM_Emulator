/*
 * DFS.h
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_SEARCHER_DFS_H_
#define SRC_EXECUTION_SEARCHER_DFS_H_

#include "Searcher.h"

class DFS : public Searcher {
public:
  DFS();
  virtual ~DFS();
  vector<shared_ptr<ExecutionState>> worklist;
  virtual void insertState(shared_ptr<ExecutionState> state);
  virtual shared_ptr<ExecutionState> getNextState();
  virtual bool isEmpty();
};

#endif /* SRC_EXECUTION_SEARCHER_DFS_H_ */
