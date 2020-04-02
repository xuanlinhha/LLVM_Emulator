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
unsigned Statistics::errorPathCounter = 0;
unsigned Statistics::instructionsCounter = 0;

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

  WithColor(errs(), HighlightColor::String) << "\n\n\n";
  WithColor(errs(), HighlightColor::String)
      << "*********** RESULT ***********\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". Running Time: " << runningTime.count() << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". Paths: " << pathCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". Error Paths: " << errorPathCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". Total Instructions : " << instructionsCounter << "\n";

  WithColor(errs(), HighlightColor::String)
      << ++order << ". Concrete Condition: " << concCondCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". Symbolic Condition: " << symCondCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". Success Simplification: " << succSimpCounter << "\n";

  WithColor(errs(), HighlightColor::String)
      << ++order << ". Cache Hit: " << cacheHitCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". Solver Checking: " << solverCheckCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". Solver Checking Time: " << solverTime.count() << "\n";

  WithColor(errs(), HighlightColor::String)
      << ++order << ". True Unknown Condition: " << trueUnknownCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". True Valid Condition: " << trueValidCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". True Invalid Condition: " << trueNotValidCounter << "\n";

  WithColor(errs(), HighlightColor::String)
      << ++order << ". False Unknown Condition: " << falseUnknownCounter
      << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". False Valid Condition: " << falseValidCounter << "\n";
  WithColor(errs(), HighlightColor::String)
      << ++order << ". False Invalid Condition: " << falseNotValidCounter
      << "\n";

  WithColor(errs(), HighlightColor::String)
      << "********* END RESULT *********\n";
}
