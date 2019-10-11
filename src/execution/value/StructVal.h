/*
 * StructVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_STRUCTVAL_H_
#define SRC_EXECUTION_VALUE_STRUCTVAL_H_

#include "DynVal.h"

class StructVal : public DynVal {
public:
  StructVal();
  virtual ~StructVal();
};

#endif /* SRC_EXECUTION_VALUE_STRUCTVAL_H_ */
