/*
 * StackFrame.cpp
 *
 *  Created on: 14 Oct 2019
 *      Author: xuanlinhha
 */

#include "StackFrame.h"

StackFrame::StackFrame(const Instruction *c, const Function *f)
    : caller(c), function(f), allocSize(0) {}

StackFrame::StackFrame(const Instruction *c, const Function *f, unsigned als,
                       vector<shared_ptr<DynVal>> &ars,
                       map<const llvm::Value *, shared_ptr<DynVal>> &rs)
    : caller(c), function(f), allocSize(als), args(ars), regs(rs) {}

StackFrame::~StackFrame() {}

void StackFrame::insertBinding(const llvm::Value *v, shared_ptr<DynVal> val) {
  auto it = regs.find(v);
  if (it == regs.end())
    regs.insert(std::make_pair(v, std::move(val)));
  else
    it->second = std::move(val);
}

bool StackFrame::hasBinding(const llvm::Value *val) const {
  auto itr = regs.find(val);
  return itr != regs.end();
}

shared_ptr<DynVal> StackFrame::lookup(const llvm::Value *val) {
  return regs.at(val);
}

unique_ptr<StackFrame> StackFrame::clone() {
  return std::make_unique<StackFrame>(caller, function, allocSize, args, regs);
}
