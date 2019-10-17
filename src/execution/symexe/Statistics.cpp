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
std::chrono::duration<double> Statistics::solverCheckingTime =
    std::chrono::duration<double>(0.0);
unsigned Statistics::noOfpath = 0;
unsigned Statistics::succSimplify = 0;

Statistics::Statistics() {}

Statistics::~Statistics() {}

void Statistics::printInfo() {
  errs() << "No of Paths: " << noOfpath << "\n";
  errs() << "Running Time: " << runningTime.count() << "\n";
  errs() << "Success Simplify Counter: " << succSimplify << "\n";
  errs() << "Solver Checking Counter: " << solverCheckingCounter << "\n";
  errs() << "Solver Checking Time: " << solverCheckingTime.count() << "\n";
}
