/*
 * Statistics.cpp
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#include "Statistics.h"

std::chrono::duration<double> Statistics::runningTime =
    std::chrono::duration<double>(0.0);
unsigned Statistics::solverCheckingCounter = 0;
unsigned Statistics::cacheHitCounter = 0;
std::chrono::duration<double> Statistics::solverCheckingTime =
    std::chrono::duration<double>(0.0);
unsigned Statistics::noOfpath = 0;
unsigned Statistics::succSimplify = 0;

Statistics::Statistics() {}

Statistics::~Statistics() {}

void Statistics::printInfo() {
  int order = 0;
  errs() << "\n\n\n";
  errs() << "*********** RESULT ***********\n";
  errs() << ++order << ". No of Paths: " << noOfpath << "\n";
  errs() << ++order << ". Running Time: " << runningTime.count() << "\n";
  errs() << ++order << ". Success Simplify Counter: " << succSimplify << "\n";
  errs() << ++order << ". Cache Hit Counter: " << cacheHitCounter << "\n";
  errs() << ++order << ". Solver Checking Counter: " << solverCheckingCounter
         << "\n";
  errs() << ++order << ". Solver Checking Time: " << solverCheckingTime.count()
         << "\n";
  errs() << "********* END RESULT *********\n";
}
