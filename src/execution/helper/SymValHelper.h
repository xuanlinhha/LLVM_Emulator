/*
 * SymValHelper.h
 *
 *  Created on: 23 Dec 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_HELPER_SYMVALHELPER_H_
#define SRC_EXECUTION_HELPER_SYMVALHELPER_H_

#include "execution/value/IntVal.h"
#include <set>
#include <string>

using namespace std;

class SymValHelper {
public:
  SymValHelper();
  virtual ~SymValHelper();
  static void getVars(shared_ptr<SimVal> &si, set<string> &result);
  static bool isIntersect(set<string> &ss1, set<string> &ss2);
  static vector<shared_ptr<IntVal>>
  getDependentEles(vector<shared_ptr<IntVal>> &list, shared_ptr<IntVal> &ele);
};

#endif /* SRC_EXECUTION_HELPER_SYMVALHELPER_H_ */
