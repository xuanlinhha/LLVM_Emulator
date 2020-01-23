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
  CacheEntry(std::map<IntVal *, bool> &_pcs, IntVal *&_q, bool _qval);
  virtual ~CacheEntry();
  std::map<IntVal *, bool> pcs;
  IntVal *q;
  bool qval;
  string stringify();
};

#endif /* SRC_SOLVER_CACHEENTRY_H_ */
