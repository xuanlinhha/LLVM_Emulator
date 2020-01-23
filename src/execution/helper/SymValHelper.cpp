/*
 * SymValHelper.cpp
 *
 *  Created on: 23 Dec 2019
 *      Author: xuanlinhha
 */

#include "SymValHelper.h"

SymValHelper::SymValHelper() {}

SymValHelper::~SymValHelper() {}

void SymValHelper::getVars(SimVal *&si, set<string> &result) {
  if (si->isSym) {
    if (si->symExprType == SymExprType::INVALID) {
      return;
    } else if (si->symExprType == SymExprType::VAR) {
      result.insert(si->name);
    } else {
      for (unsigned i = 0; i < si->operands.size(); ++i) {
        getVars(si->operands[i], result);
      }
    }
  }
}

bool SymValHelper::isIntersect(set<string> &ss1, set<string> &ss2) {
  for (set<string>::iterator it = ss1.begin(), ie = ss1.end(); it != ie; ++it) {
    if (ss2.find(*it) != ss2.end()) {
      return true;
    }
  }
  return false;
}

vector<IntVal *> SymValHelper::getDependentEles(vector<IntVal *> &worklist,
                                                IntVal *&q) {
  // set of variables in query
  set<string> vars;
  SimVal *t = (SimVal *)q;
  SymValHelper::getVars(t, vars);

  vector<IntVal *> result;
  bool stop = false;
  while (!stop) {
    stop = true;
    vector<IntVal *> newWorklist;
    for (vector<IntVal *>::iterator it = worklist.begin(), ie = worklist.end();
         it != ie; ++it) {
      // get variables set of the current element
      set<string> tmp;
      SimVal *t = (SimVal *)*it;
      SymValHelper::getVars(t, tmp);
      if (SymValHelper::isIntersect(vars, tmp)) {
        vars.insert(tmp.begin(), tmp.end());
        result.push_back(*it);
        stop = false;
      } else {
        newWorklist.push_back(*it);
      }
    }

    if (stop) {
      break;
    } else {
      worklist.swap(newWorklist);
    }
  }
  return result;
}
