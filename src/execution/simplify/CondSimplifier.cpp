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

IntVal *CondSimplifier::simplify(std::map<IntVal *, bool> &pcs, IntVal *&expr) {
  //  errs() << "Simplify Start!\n";
  if (!expr->isSym) {
    return expr;
  }
  string es;
  raw_string_ostream ess(es);
  expr->print(&ess);
  ess.flush();
  for (std::map<IntVal *, bool>::iterator it = pcs.begin(), ie = pcs.end();
       it != ie; ++it) {
    string ts;
    raw_string_ostream tss(ts);
    it->first->print(&tss);
    tss.flush();
    if (ts == es) {
      //      errs() << "Simplify Success: " << es << " -> " << it->second <<
      //      "\n";
      ++Statistics::succSimpCounter;
      return new IntVal(APInt(1, it->second));
    }
  }
  //  errs() << "Simplify Fail!\n";
  return expr;
}

IntVal *CondSimplifier::simplify1(std::map<IntVal *, bool> &pcs,
                                  IntVal *&expr) {
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

SimVal *CondSimplifier::replace(SimVal *&expr,
                                std::map<SimVal *, SimVal *> &equalities) {
  return nullptr;
}
