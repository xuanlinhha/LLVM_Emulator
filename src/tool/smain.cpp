#include <iostream>
#include <map>
#include <string>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include "execution/symexe/SymExecutor.h"

using namespace std;
using namespace llvm;

cl::opt<string> SearchStrategy("search", cl::desc("<Search strategy>"),
                               cl::value_desc("search_strategy"));

cl::opt<string> PrintPath("print-path", cl::desc("<Print path>"),
                          cl::value_desc("print_path"));

cl::opt<string> AssertFailLimit("assert-fail-limit",
                                cl::desc("<Assertion Fail Limit>"),
                                cl::value_desc("print_path"));

cl::opt<string> InputFile(cl::Positional, cl::desc("<bc file>"), cl::init("-"));

cl::list<string> ProgramParams(cl::ConsumeAfter,
                               cl::desc("<program parameters>..."));

int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv, "A simple LLVM symbolic simulator");

  // prepare module
  LLVMContext Context;
  SMDiagnostic Err;
  unique_ptr<Module> mainModule = parseIRFile(InputFile, Err, Context);
  unique_ptr<SymExecutor> symExecutor = std::make_unique<SymExecutor>();

  // simulator's parameters
  symExecutor->simParams.insert(
      std::make_pair(SimParamType::SEARCH, SearchStrategy.getValue()));
  symExecutor->simParams.insert(
      std::make_pair(SimParamType::PRINT_PATH, PrintPath.getValue()));
  symExecutor->simParams.insert(std::make_pair(
      SimParamType::ASSERTION_FAIL_LIMIT, AssertFailLimit.getValue()));

  // program's parameters
  ProgramParams.insert(ProgramParams.begin(), InputFile);

  // initialize & start
  symExecutor->initialize(mainModule.get(), ProgramParams);
  symExecutor->startSym();

  return 0;
}
