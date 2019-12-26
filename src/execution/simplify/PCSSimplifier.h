/*
 * PCSSimplifier.h
 *
 *  Created on: 26 Dec 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_SIMPLIFY_PCSSIMPLIFIER_H_
#define SRC_EXECUTION_SIMPLIFY_PCSSIMPLIFIER_H_

#include "execution/helper/SymValHelper.h"
#include "execution/value/IntVal.h"

class PCSSimplifier {
public:
  PCSSimplifier();
  virtual ~PCSSimplifier();
  static map<shared_ptr<IntVal>, bool>
  getDependentConstraints(map<shared_ptr<IntVal>, bool> &pcs,
                          shared_ptr<IntVal> &q);
};

#endif /* SRC_EXECUTION_SIMPLIFY_PCSSIMPLIFIER_H_ */
