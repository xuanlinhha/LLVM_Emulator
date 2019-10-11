#include <iostream>
#include <map>
#include <string>

#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/raw_ostream.h>

using namespace std;
using namespace llvm;

cl::opt<string> SearchStrategy("search", cl::desc("<Search strategy>"),
                               cl::value_desc("search_strategy"));

cl::opt<string> PrintPath("print-path", cl::desc("<Print path>"),
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

  return 0;
}
