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
#include "execution/simplify/CondSimplifier.h"
#include "execution/simplify/PCSSimplifier.h"
#include "execution/solver/ResultType.h"
#include "execution/solver/Solver.h"
#include "llvm/Support/WithColor.h"
#include <boost/format.hpp>
#include <llvm/IR/CallSite.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <llvm/IR/DebugLoc.h>
#include <llvm/IR/GetElementPtrTypeIterator.h>
#include <llvm/IR/GlobalAlias.h>
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
  static DataLayout *dataLayout;
  static vector<DynVal *> mainArgs;

  //
  std::map<const GlobalValue *, unsigned> globalAddresses;
  std::map<unsigned, const llvm::Function *> addressToFunc;
  vector<StackFrame *> frames;
  Memory *globalMem;
  Memory *stackMem;
  Memory *heapMem;
  const Instruction *currentInst;
  const BasicBlock *incomingBB;
  std::map<IntVal *, bool> pcs;

  //
  bool isError;
  DynVal *run();

  // symbolic execution
  void executeNonTerminator();
  DynVal *evalOperand(const Value *v);
  unsigned allocateStackMem(unsigned size);
  void writeToPointer(const PointerVal *ptr, const DynVal *val);
  DynVal *readFromPointer(const PointerVal *ptr, Type *loadType);
  DynVal *loadValue(Memory *&mem, unsigned addr, Type *loadType);
  DynVal *callExternalFunction(ImmutableCallSite cs, const Function *f,
                               vector<DynVal *> argValues);
  string readStringFromPointer(const PointerVal *ptr);

  // evaluate constant values
  DynVal *evalConst(const Constant *cv);
  DynVal *evalConstExpr(const ConstantExpr *cexpr);
  Value *findBasePointer(const Value *val);

  // stack
  void emptyStack();
  void popStack();
  ExecutionState *clone();
};

#endif /* SRC_EXECUTION_STATE_EXECUTIONSTATE_H_ */
