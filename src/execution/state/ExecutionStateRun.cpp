/*
 * ExecutionStateRun.cpp
 *
 *  Created on: 19 Oct 2019
 *      Author: xuanlinhha
 */

#include "ExecutionState.h"

void ExecutionState::executeNonTerminator() {}

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
