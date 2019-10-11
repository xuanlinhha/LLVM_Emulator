/*
 * IntVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_INTVAL_H_
#define SRC_EXECUTION_VALUE_INTVAL_H_

#include "SimVal.h"

class IntVal : public SimVal {
public:
  IntVal();
  virtual ~IntVal();
  APInt intVal;
};

#endif /* SRC_EXECUTION_VALUE_INTVAL_H_ */
