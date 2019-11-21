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
  string es;
  raw_string_ostream ess(es);
  expr->print(&ess);
  ess.flush();
  for (std::map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(),
                                                    ie = pcs.end();
       it != ie; ++it) {
    string ts;
    raw_string_ostream tss(ts);
    it->first->print(&tss);
    tss.flush();
    if (ts == es) {
      errs() << "Simplify Success: " << es << " -> " << it->second << "\n";
      ++Statistics::succSimplify;
      return std::make_shared<IntVal>(APInt(1, it->second));
    }
  }
  //  errs() << "Simplify Fail!\n";
  return expr;
}

shared_ptr<IntVal>
CondSimplifier::simplify1(std::map<shared_ptr<IntVal>, bool> &pcs,
                          shared_ptr<IntVal> &expr) {
  if (!expr->isSym) {
    return expr;
  }

  //  std::map<shared_ptr<SimVal>, shared_ptr<SimVal>> equalities;
  //
  //  // collect equalities
  //  errs() << "====START====\n";
  //  for (std::map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(),
  //                                                    ie = pcs.end();
  //       it != ie; ++it) {
  //    errs() << it->first->toString() << "->";
  //    errs() << it->second << "\n";
  //
  //    if ((it->second && it->first->symExprType == SymExprType::EQ) ||
  //        (!it->second && it->first->symExprType == SymExprType::NE)) {
  //      equalities.insert(
  //          std::make_pair(it->first->operands[0], it->first->operands[1]));
  //      errs() << it->first->operands[0]->toString() << "->";
  //      errs() << it->first->operands[1]->toString() << "\n";
  //      errs() << "++++++++\n";
  //    }
  //    equalities.insert(std::make_pair(
  //        it->first, std::make_shared<IntVal>(APInt(1, it->second))));
  //  }
  //
  //  errs() << "====MIDDLE====\n";
  //  for (std::map<shared_ptr<SimVal>, shared_ptr<SimVal>>::iterator
  //           it = equalities.begin(),
  //           ie = equalities.end();
  //       it != ie; ++it) {
  //    errs() << it->first->toString() << "->";
  //    errs() << it->second->toString() << "\n";
  //  }
  //  errs() << "--------\n";
  //  errs() << expr->toString() << "\n";
  //  errs() << "====END====\n";
  return nullptr;
}

shared_ptr<SimVal> CondSimplifier::replace(
    shared_ptr<SimVal> &expr,
    std::map<shared_ptr<SimVal>, shared_ptr<SimVal>> &equalities) {
  return nullptr;
}
