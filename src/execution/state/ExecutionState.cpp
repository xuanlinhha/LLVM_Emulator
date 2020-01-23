/*
 * ExecutionState.cpp
 *
 *  Created on: 14 Oct 2019
 *      Author: xuanlinhha
 */

#include "ExecutionState.h"
#include "execution/symexe/SymExecutor.h"

Module *ExecutionState::module = nullptr;
DataLayout *ExecutionState::dataLayout = nullptr;
vector<DynVal *> ExecutionState::mainArgs;

ExecutionState::ExecutionState()
    : globalMem(new Memory()), stackMem(new Memory()), heapMem(new Memory()),
      currentInst(nullptr), incomingBB(nullptr), isError(false) {}

ExecutionState::~ExecutionState() {}

DynVal *ExecutionState::run() {
  while (!frames.empty()) {
    // Skip debug instructions
    if (isa<DbgInfoIntrinsic>(currentInst)) {
      currentInst = currentInst->getNextNode();
      continue;
    }

    switch (currentInst->getOpcode()) {
    case Instruction::Br: {
      incomingBB = currentInst->getParent();
      auto brInst = cast<BranchInst>(currentInst);
      // un-conditional branch
      if (!brInst->isConditional()) {
        currentInst = &brInst->getSuccessor(0)->front();
        break;
      }

      // conditional branch
      auto cond = (IntVal *)evalOperand(brInst->getCondition());
      //      cond->print(&errs());
      //      errs() << "\n";

      // concrete condition
      if (!cond->isSym) {
        ++Statistics::concCondCounter;
        if (cond->intVal.getBoolValue()) {
          currentInst = &brInst->getSuccessor(0)->front();
        } else {
          currentInst = &brInst->getSuccessor(1)->front();
        }
        break;
      }

      // simplify symbolic condition
      ++Statistics::symCondCounter;
      auto simplifiedPCS = PCSSimplifier::getDependentConstraints(pcs, cond);
      auto simpCond = CondSimplifier::simplify(simplifiedPCS, cond);

      if (!simpCond->isSym) {
        if (simpCond->intVal.getBoolValue()) {
          currentInst = &brInst->getSuccessor(0)->front();
        } else {
          currentInst = &brInst->getSuccessor(1)->front();
        }
        break;
      }

      // symbolic condition
      ValidResult trueValid =
          SymExecutor::solver->isValid(simplifiedPCS, simpCond, true);
      if (trueValid == ValidResult::UNKNOWN) {
        ++Statistics::trueUnknownCounter;
        // cannot prove validity or not
        // enable error
        isError = true;
        WithColor(errs(), HighlightColor::Error)
            << "solver cannot prove validity or not!\n";
      } else if (trueValid == ValidResult::YES) {
        ++Statistics::trueValidCounter;
        // continue with true branch
        currentInst = &brInst->getSuccessor(0)->front();

      } else {
        ++Statistics::trueNotValidCounter;
        ValidResult falseValid =
            SymExecutor::solver->isValid(simplifiedPCS, simpCond, false);
        if (trueValid == ValidResult::UNKNOWN) {
          ++Statistics::falseUnknownCounter;
          // enable error
          isError = true;
          WithColor(errs(), HighlightColor::Error)
              << "solver cannot prove validity or not!\n";
        } else if (falseValid == ValidResult::YES) {
          ++Statistics::falseValidCounter;
          // continue with false branch
          currentInst = &brInst->getSuccessor(1)->front();
        } else {
          ++Statistics::falseNotValidCounter;
          // add false state to work list
          ExecutionState *falseState = this->clone();
          falseState->pcs[simpCond] = false;
          falseState->currentInst = &brInst->getSuccessor(1)->front();
          SymExecutor::searcher->insertState(falseState);

          // add new constraint & continue with true state
          pcs[simpCond] = true;
          currentInst = &brInst->getSuccessor(0)->front();
        }
      }
      break;
    }
    case Instruction::Switch: {
      auto switchInst = cast<SwitchInst>(currentInst);
      auto condVal = evalOperand(switchInst->getCondition());
      auto const &condInt = ((IntVal *)condVal)->intVal;
      auto const *destBB = switchInst->getDefaultDest();
      for (auto &caseItr : switchInst->cases()) {
        auto const &caseInt = caseItr.getCaseValue()->getValue();
        if (condInt == caseInt) {
          destBB = caseItr.getCaseSuccessor();
          break;
        }
      }
      incomingBB = currentInst->getParent();
      currentInst = &destBB->front();
      break;
    }
    case Instruction::Ret: {
      auto retInst = cast<ReturnInst>(currentInst);
      DynVal *retVal = nullptr;
      if (auto value = retInst->getReturnValue())
        retVal = evalOperand(value);
      popStack();
      return retVal; // Return to function's caller
    }
    default: {
      executeNonTerminator();
    }
    }

    // check if error happen
    if (isError) {
      // TODO: Print info
      break;
    }
  }

  return new DynVal(DynValType::ERROR);
}

DynVal *ExecutionState::evalOperand(const llvm::Value *v) {
  if (auto cv = dyn_cast<Constant>(v)) {
    return evalConst(cv);
  } else {
    return frames.back()->lookup(v);
  }
}

Value *ExecutionState::findBasePointer(const Value *val) {
  if (auto itp = cast<IntToPtrInst>(val)) {
    Value *srcValue = nullptr;
    auto op = itp->getOperand(0);
    if (PatternMatch::match(
            op, PatternMatch::m_PtrToInt(PatternMatch::m_Value(srcValue)))) {
      return srcValue->stripPointerCasts();
    } else if (PatternMatch::match(
                   op, PatternMatch::m_Add(PatternMatch::m_PtrToInt(
                                               PatternMatch::m_Value(srcValue)),
                                           PatternMatch::m_Value()))) {
      return srcValue->stripPointerCasts();
    }
  }

  llvm_unreachable("Unhandled case for findBasePointer()");
  return nullptr;
}

void ExecutionState::popStack() {
  stackMem->deallocate(frames.back()->allocSize);
  assert(!frames.empty());
  frames.pop_back();
}

void ExecutionState::emptyStack() {
  while (!frames.empty()) {
    frames.pop_back();
  }
}

ExecutionState *ExecutionState::clone() {
  ExecutionState *res = new ExecutionState();
  res->globalAddresses = globalAddresses;
  res->addressToFunc = addressToFunc;
  res->currentInst = currentInst;
  res->incomingBB = incomingBB;
  res->globalMem = globalMem->clone();
  res->stackMem = stackMem->clone();
  res->heapMem = heapMem->clone();
  for (vector<StackFrame *>::iterator it = frames.begin(), ie = frames.end();
       it != ie; ++it) {
    res->frames.push_back((*it)->clone());
  }
  res->pcs = pcs;
  res->isError = isError;
  return res;
}
