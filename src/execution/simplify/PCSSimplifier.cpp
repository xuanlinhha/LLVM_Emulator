/*
 * PCSSimplifier.cpp
 *
 *  Created on: 26 Dec 2019
 *      Author: xuanlinhha
 */

#include "PCSSimplifier.h"

PCSSimplifier::PCSSimplifier() {
  // TODO Auto-generated constructor stub
}

PCSSimplifier::~PCSSimplifier() {
  // TODO Auto-generated destructor stub
}

map<IntVal *, bool>
PCSSimplifier::getDependentConstraints(map<IntVal *, bool> &pcs, IntVal *&q) {

  // extract dependent constraints
  vector<IntVal *> worklist;
  for (map<IntVal *, bool>::iterator it = pcs.begin(), ie = pcs.end(); it != ie;
       ++it) {
    worklist.push_back(it->first);
  }
  vector<IntVal *> dependentEles = SymValHelper::getDependentEles(worklist, q);
  map<IntVal *, bool> result;
  for (vector<IntVal *>::iterator it = dependentEles.begin(),
                                  ie = dependentEles.end();
       it != ie; ++it) {
    result[*it] = pcs[*it];
  }
  return result;
}
