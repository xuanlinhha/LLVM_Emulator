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
unsigned Statistics::pathCounter = 0;
unsigned Statistics::succSimplificationCounter = 0;
unsigned Statistics::concreteCondCounter = 0;
unsigned Statistics::symbolicCondCounter = 0;

Statistics::Statistics() {}

Statistics::~Statistics() {}

void Statistics::printInfo() {
  int order = 0;
  errs() << "\n\n\n";
  errs() << "*********** RESULT ***********\n";
  errs() << ++order << ". Running Time: " << runningTime.count() << "\n";
  errs() << ++order << ". No of Paths: " << pathCounter << "\n";
  errs() << ++order << ". No. of Cache Hit: " << cacheHitCounter << "\n";
  errs() << ++order << ". No. of Solver Checking: " << solverCheckingCounter
         << "\n";
  errs() << ++order << ". Solver Checking Time: " << solverCheckingTime.count()
         << "\n";
  errs() << ++order << ". No. of Concrete Condition: " << concreteCondCounter
         << "\n";
  errs() << ++order << ". No. of Symbolic Condition: " << symbolicCondCounter
         << "\n";
  errs() << ++order
         << ". No. of Success Simplification: " << succSimplificationCounter
         << "\n";
  errs() << "********* END RESULT *********\n";
}
