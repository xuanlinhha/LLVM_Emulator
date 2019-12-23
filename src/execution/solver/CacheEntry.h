/*
 * CacheEntry.h
 *
 *  Created on: 19 Nov 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_SOLVER_CACHEENTRY_H_
#define SRC_SOLVER_CACHEENTRY_H_

#include "execution/value/IntVal.h"

using namespace std;

class CacheEntry {
public:
  CacheEntry(std::map<shared_ptr<IntVal>, bool> &_pcs, shared_ptr<IntVal> &_q,
             bool _qval);
  virtual ~CacheEntry();
  std::map<shared_ptr<IntVal>, bool> pcs;
  shared_ptr<IntVal> q;
  bool qval;
  string stringify();
};

#endif /* SRC_SOLVER_CACHEENTRY_H_ */
