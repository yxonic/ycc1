#include <iostream>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"

#include "llvm/IR/LegacyPassManager.h"

#include "Driver.h"

using namespace std;
using namespace llvm;

char usage_str[] = "usage: ycc1 [-h] [-S] [-d] [-o file] source      \n\
                                                                     \n\
positional arguments:                                                \n\
  source             source file to compile                          \n\
                                                                     \n\
optional arguments:                                                  \n\
  -h, --help         show this help message and exit                 \n\
  -d, --debug        print debug messages                            \n\
  -o file            target path for the compiled code               \n\
  -S                 only run preprocess and compilation steps";

void print_usage()
{
    cout << usage_str << endl;
}

int main(int argc, char **argv)
{
    LLVMContext &context = getGlobalContext();
    Module *module = new Module("ycc1", context);

    FunctionPassManager *fpm = new FunctionPassManager(module);

    fpm->add(new DataLayoutPass());
    // Provide basic AliasAnalysis support for GVN.
    fpm->add(createBasicAliasAnalysisPass());
    // Promote allocas to registers.
    fpm->add(createPromoteMemoryToRegisterPass());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    fpm->add(createInstructionCombiningPass());
    // Reassociate expressions.
    fpm->add(createReassociatePass());
    // Eliminate Common SubExpressions.
    fpm->add(createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    fpm->add(createCFGSimplificationPass());
    
    fpm->doInitialization();
    
    LLVMCodeGen *codegen = new LLVMCodeGen(module, fpm);

    string input_file;
    string output_file;
    bool emit_llvm = false;
    
    --argc; ++argv;
    while (argc > 0) {
        string arg(*argv);
        if (arg == "-o") {
            --argc; ++argv;
            if (argc <= 0) {
                print_usage();
                return 1;
            }
            output_file = *argv;
        } else if (arg == "-S") {
            emit_llvm = true;
        } else if (arg == "-d") {
            logger.setLevel(LogManager::Debug);
        } else if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (!arg.empty() && arg[0] == '-') {
            print_usage();
            return 1;
        } else {
            input_file = *argv;
        }
        --argc; ++argv;
    }
    
    if (input_file.empty()) {
        cout << "ycc1: no input file" << endl;
        return 1;
    }

    if (output_file.empty())
        output_file = "a.out";
    
    Driver driver(input_file, codegen);

    driver.parse();

    if (emit_llvm)
        driver.codegen(output_file);

    return 0;
}
