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
  static IntVal *simplify(std::map<IntVal *, bool> &pcs, IntVal *&expr);
  static IntVal *simplify1(std::map<IntVal *, bool> &pcs, IntVal *&expr);

  static SimVal *replace(SimVal *&expr,
                         std::map<SimVal *, SimVal *> &equalities);
};

#endif /* SRC_SIMPLIFY_CONDSIMPLIFIER_H_ */
