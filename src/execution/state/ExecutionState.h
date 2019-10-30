/*
 * ExecutionState.h
 *
 *  Created on: 14 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_STATE_EXECUTIONSTATE_H_
#define SRC_EXECUTION_STATE_EXECUTIONSTATE_H_

#include "Memory.h"
#include "StackFrame.h"
#include "execution/searcher/Searcher.h"
#include "execution/symexe/SymExecutor.h"
#include "simplify/CondSimplifier.h"
#include <boost/format.hpp>
#include <llvm/IR/CallSite.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GetElementPtrTypeIterator.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/PatternMatch.h>
#include <unordered_map>

using namespace std;
using namespace llvm;

class ExecutionState {
public:
  ExecutionState();
  virtual ~ExecutionState();

  // static data
  static Module *module;
  static shared_ptr<DataLayout> dataLayout;
  static vector<shared_ptr<DynVal>> mainArgs;

  //
  std::map<const GlobalValue *, unsigned> globalAddresses;
  std::map<unsigned, const llvm::Function *> addressToFunc;
  vector<unique_ptr<StackFrame>> frames;
  unique_ptr<Memory> globalMem;
  unique_ptr<Memory> stackMem;
  unique_ptr<Memory> heapMem;
  const Instruction *currentInst;
  const BasicBlock *incomingBB;
  std::map<shared_ptr<IntVal>, bool> pcs;

  //
  bool isError;
  shared_ptr<DynVal> run();

  // symbolic execution
  void executeNonTerminator();
  shared_ptr<DynVal> evalOperand(const Value *v);
  unsigned allocateStackMem(unsigned size);
  void writeToPointer(const shared_ptr<PointerVal> ptr,
                      const shared_ptr<DynVal> val);
  shared_ptr<DynVal> readFromPointer(const shared_ptr<PointerVal> ptr,
                                     Type *loadType);
  shared_ptr<DynVal> loadValue(unique_ptr<Memory> &mem, unsigned addr,
                               Type *loadType);
  shared_ptr<DynVal> callExternalFunction(ImmutableCallSite cs,
                                          const Function *f,
                                          vector<shared_ptr<DynVal>> argValues);
  string readStringFromPointer(const shared_ptr<PointerVal> ptr);

  // evaluate constant values
  shared_ptr<DynVal> evalConst(const Constant *cv);
  shared_ptr<DynVal> evalConstExpr(const ConstantExpr *cexpr);
  Value *findBasePointer(const Value *val);

  // stack
  void emptyStack();
  void popStack();
  shared_ptr<ExecutionState> clone();
};

#endif /* SRC_EXECUTION_STATE_EXECUTIONSTATE_H_ */
