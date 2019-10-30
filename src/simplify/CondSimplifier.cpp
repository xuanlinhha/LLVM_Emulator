/*
 * CondSimplifier.cpp
 *
 *  Created on: 21 Oct 2019
 *      Author: xuanlinhha
 */

#include "CondSimplifier.h"

CondSimplifier::CondSimplifier() {
  // TODO Auto-generated constructor stub
}

CondSimplifier::~CondSimplifier() {
  // TODO Auto-generated destructor stub
}

shared_ptr<IntVal>
CondSimplifier::simplify(std::map<shared_ptr<IntVal>, bool> &pcs,
                         shared_ptr<IntVal> &expr) {

  //  errs() << "Simplify Start!\n";
  if (!expr->isSym) {
    return expr;
  }
  for (std::map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(),
                                                    ie = pcs.end();
       it != ie; ++it) {
    if (it->first->toString() == expr->toString()) {
      //      errs() << "Simplify Success: " << expr->toString() << " -> " <<
      //      it->second
      //             << "\n";
      ++Statistics::succSimplify;
      return std::make_shared<IntVal>(APInt(1, it->second));
    }
  }
  //  errs() << "Simplify Fail!\n";
  return expr;
}
