/*
 * StructVal.h
 *
 *  Created on: 11 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_VALUE_STRUCTVAL_H_
#define SRC_EXECUTION_VALUE_STRUCTVAL_H_

#include "DynVal.h"
#include <map>

class StructVal : public DynVal {
public:
  StructVal(unsigned);
  virtual ~StructVal();
  map<unsigned, shared_ptr<DynVal>> structMap; // offset -> value
  unsigned structSize;
  void addField(unsigned offset, shared_ptr<DynVal> val);
  shared_ptr<DynVal> getFieldAtOffset(unsigned offset) const;

  virtual void print(raw_ostream *os);
};

#endif /* SRC_EXECUTION_VALUE_STRUCTVAL_H_ */
