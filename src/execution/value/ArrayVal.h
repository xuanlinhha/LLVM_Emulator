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
  vector<shared_ptr<DynVal>> array;
  void setElementAtIndex(unsigned idx, shared_ptr<DynVal> val);
  shared_ptr<DynVal> getElementAtIndex(unsigned idx) const;

  virtual void print();
  virtual string toString();
};

#endif /* SRC_EXECUTION_VALUE_ARRAYVAL_H_ */
