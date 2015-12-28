#include <iostream>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "Driver.h"

using namespace std;
using namespace llvm;

char usage_str[] = "usage: ycc1 [-h] [-S] [-d] [-o file] source      \n\
                                                                                \n\
positional arguments:                                                           \n\
  source             source file to compile                                     \n\
                                                                                \n\
optional arguments:                                                             \n\
  -h, --help         show this help message and exit                            \n\
  -d, --debug        print debug messages                                       \n\
  -o file            target path for the compiled code                          \n\
  -S                 only run preprocess and compilation steps";

void print_usage()
{
    cout << usage_str << endl;
}

int main(int argc, char **argv)
{
    LLVMContext &context = getGlobalContext();
    Module *module = new Module("ycc1", context);
    LLVMCodeGen *codegen = new LLVMCodeGen(module);

    string input_file;
    string output_file;
    bool emit_llvm = false;
    
    --argc; ++argv;
    while (argc > 0) {
        string arg(*argv);
        if (arg == "-o") {
            --argc; ++argv;
            if (argc < 0) {
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
    if (output_file.empty())
        output_file = "a.out";
    
    Driver driver(input_file, codegen);

    driver.parse();

    if (emit_llvm)
        driver.codegen(output_file);

    return 0;
}
