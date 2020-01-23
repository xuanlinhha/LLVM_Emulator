/*
 * ArrayVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_ARRAYVAL_H_
#define SRC_EXECUTION_VALUE_ARRAYVAL_H_

#include "DynVal.h"

class ArrayVal : public DynVal {
public:
  ArrayVal(unsigned es, unsigned as);
  virtual ~ArrayVal();
  unsigned elemSize;
  unsigned arraySize;
  vector<DynVal *> array;
  void setElementAtIndex(unsigned idx, DynVal *val);
  DynVal *getElementAtIndex(unsigned idx) const;

  virtual void print(raw_ostream *os);
};

#endif /* SRC_EXECUTION_VALUE_ARRAYVAL_H_ */
