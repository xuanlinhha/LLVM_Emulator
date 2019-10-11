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
  ArrayVal();
  virtual ~ArrayVal();
};

#endif /* SRC_EXECUTION_VALUE_ARRAYVAL_H_ */
