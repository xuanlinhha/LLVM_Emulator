/*
 * Statistics.h
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_SYMEXE_STATISTICS_H_
#define SRC_EXECUTION_SYMEXE_STATISTICS_H_

#include "llvm/Support/WithColor.h"
#include <chrono>
#include <llvm/Support/raw_ostream.h>
using namespace llvm;

class Statistics {
public:
  Statistics();
  virtual ~Statistics();
  // common
  static std::chrono::duration<double> runningTime;
  static unsigned pathCounter;
  static unsigned errorPathCounter;

  // simplification
  static unsigned concCondCounter;
  static unsigned symCondCounter;
  static unsigned succSimpCounter;

  // solver
  static unsigned cacheHitCounter;
  static unsigned solverCheckCounter;
  static std::chrono::duration<double> solverTime;

  static unsigned trueValidCounter;
  static unsigned trueNotValidCounter;
  static unsigned trueUnknownCounter;

  static unsigned falseValidCounter;
  static unsigned falseNotValidCounter;
  static unsigned falseUnknownCounter;

  static void printInfo();
};

#endif /* SRC_EXECUTION_SYMEXE_STATISTICS_H_ */
