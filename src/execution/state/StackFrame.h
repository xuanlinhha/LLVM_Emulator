/*
 * StackFrame.h
 *
 *  Created on: 14 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_STATE_STACKFRAME_H_
#define SRC_EXECUTION_STATE_STACKFRAME_H_

#include "execution/value/DynVal.h"
#include <llvm/IR/Function.h>

class StackFrame {
public:
  StackFrame(const Instruction *c, const Function *f);
  StackFrame(const Instruction *c, const Function *f, unsigned als,
             vector<shared_ptr<DynVal>> &ars,
             map<const llvm::Value *, shared_ptr<DynVal>> &rs);
  virtual ~StackFrame();
  const Instruction *caller;
  const Function *function;
  unsigned allocSize;
  vector<shared_ptr<DynVal>> args;
  map<const llvm::Value *, shared_ptr<DynVal>> regs;
  void insertBinding(const llvm::Value *v, shared_ptr<DynVal> val);
  bool hasBinding(const llvm::Value *val) const;
  shared_ptr<DynVal> lookup(const llvm::Value *val);
  unique_ptr<StackFrame> clone();
};

#endif /* SRC_EXECUTION_STATE_STACKFRAME_H_ */
