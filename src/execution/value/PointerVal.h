/*
 * PointerVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_POINTERVAL_H_
#define SRC_EXECUTION_VALUE_POINTERVAL_H_

#include "SimVal.h"

class PointerVal : public SimVal {
public:
  PointerVal();
  virtual ~PointerVal();
};

#endif /* SRC_EXECUTION_VALUE_POINTERVAL_H_ */
