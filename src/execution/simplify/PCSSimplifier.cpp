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

map<shared_ptr<IntVal>, bool>
PCSSimplifier::getDependentConstraints(map<shared_ptr<IntVal>, bool> &pcs,
                                       shared_ptr<IntVal> &q) {

  // extract dependent constraints
  vector<shared_ptr<IntVal>> worklist;
  for (map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(), ie = pcs.end();
       it != ie; ++it) {
    worklist.push_back(it->first);
  }
  vector<shared_ptr<IntVal>> dependentEles =
      SymValHelper::getDependentEles(worklist, q);
  map<shared_ptr<IntVal>, bool> result;
  for (vector<shared_ptr<IntVal>>::iterator it = dependentEles.begin(),
                                            ie = dependentEles.end();
       it != ie; ++it) {
    result[*it] = pcs[*it];
  }
  return result;
}
