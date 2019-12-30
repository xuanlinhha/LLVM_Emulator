/*
 * Statistics.cpp
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#include "Statistics.h"

std::chrono::duration<double> Statistics::runningTime =
    std::chrono::duration<double>(0.0);
unsigned Statistics::pathCounter = 0;

unsigned Statistics::concCondCounter = 0;
unsigned Statistics::symCondCounter = 0;
unsigned Statistics::succSimpCounter = 0;

unsigned Statistics::cacheHitCounter = 0;
unsigned Statistics::solverCheckCounter = 0;
std::chrono::duration<double> Statistics::solverTime =
    std::chrono::duration<double>(0.0);

unsigned Statistics::trueUnknownCounter = 0;
unsigned Statistics::trueValidCounter = 0;
unsigned Statistics::trueNotValidCounter = 0;

unsigned Statistics::falseUnknownCounter = 0;
unsigned Statistics::falseValidCounter = 0;
unsigned Statistics::falseNotValidCounter = 0;

Statistics::Statistics() {}

Statistics::~Statistics() {}

void Statistics::printInfo() {
  int order = 0;
  errs() << "\n\n\n";
  errs() << "*********** RESULT ***********\n";
  errs() << ++order << ". Running Time: " << runningTime.count() << "\n";
  errs() << ++order << ". Paths: " << pathCounter << "\n";

  errs() << ++order << ". Concrete Condition: " << concCondCounter << "\n";
  errs() << ++order << ". Symbolic Condition: " << symCondCounter << "\n";
  errs() << ++order << ". Success Simplification: " << succSimpCounter << "\n";

  errs() << ++order << ". Cache Hit: " << cacheHitCounter << "\n";
  errs() << ++order << ". Solver Checking: " << solverCheckCounter << "\n";
  errs() << ++order << ". Solver Checking Time: " << solverTime.count() << "\n";

  errs() << ++order << ". True Unknown Condition: " << trueUnknownCounter
         << "\n";
  errs() << ++order << ". True Valid Condition: " << trueValidCounter << "\n";
  errs() << ++order << ". True Invalid Condition: " << trueNotValidCounter
         << "\n";

  errs() << ++order << ". False Unknown Condition: " << falseUnknownCounter
         << "\n";
  errs() << ++order << ". False Valid Condition: " << falseValidCounter << "\n";
  errs() << ++order << ". False Invalid Condition: " << falseNotValidCounter
         << "\n";

  errs() << "********* END RESULT *********\n";
}
