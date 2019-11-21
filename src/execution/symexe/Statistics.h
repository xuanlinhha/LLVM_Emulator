/*
 * Statistics.h
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_EXECUTION_SYMEXE_STATISTICS_H_
#define SRC_EXECUTION_SYMEXE_STATISTICS_H_

#include <chrono>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

class Statistics {
public:
  Statistics();
  virtual ~Statistics();
  static std::chrono::duration<double> runningTime;
  static unsigned cacheHitCounter;
  static unsigned solverCheckingCounter;
  static std::chrono::duration<double> solverCheckingTime;
  static unsigned noOfpath;
  static unsigned succSimplify;
  static void printInfo();
};

#endif /* SRC_EXECUTION_SYMEXE_STATISTICS_H_ */
