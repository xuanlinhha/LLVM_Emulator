/*
 * CondSimplifier.h
 *
 *  Created on: 21 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_SIMPLIFY_CONDSIMPLIFIER_H_
#define SRC_SIMPLIFY_CONDSIMPLIFIER_H_

#include "execution/symexe/Statistics.h"
#include "execution/value/IntVal.h"

class CondSimplifier {
public:
  CondSimplifier();
  virtual ~CondSimplifier();
  static shared_ptr<IntVal> simplify(std::map<shared_ptr<IntVal>, bool> &pcs,
                                     shared_ptr<IntVal> &expr);
  static shared_ptr<IntVal> simplify1(std::map<shared_ptr<IntVal>, bool> &pcs,
                                      shared_ptr<IntVal> &expr);

  static shared_ptr<SimVal>
  replace(shared_ptr<SimVal> &expr,
          std::map<shared_ptr<SimVal>, shared_ptr<SimVal>> &equalities);
};

#endif /* SRC_SIMPLIFY_CONDSIMPLIFIER_H_ */
