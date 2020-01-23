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
             vector<DynVal *> &ars, map<const llvm::Value *, DynVal *> &rs);
  virtual ~StackFrame();
  const Instruction *caller;
  const Function *function;
  unsigned allocSize;
  vector<DynVal *> args;
  map<const llvm::Value *, DynVal *> regs;
  void insertBinding(const llvm::Value *v, DynVal *val);
  bool hasBinding(const llvm::Value *val) const;
  DynVal *lookup(const llvm::Value *val);
  StackFrame *clone();
};

#endif /* SRC_EXECUTION_STATE_STACKFRAME_H_ */
