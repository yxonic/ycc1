#include <iostream>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "Driver.h"

using namespace std;
using namespace llvm;

int main(int argc, char **argv)
{
    LLVMContext &context = getGlobalContext();
    unique_ptr<Module> module(new Module("main", context));
    LLVMCodeGen *codegen = new LLVMCodeGen(module);
    Driver driver(argv[1], codegen);

    driver.parse();

    logger.setLevel(LogManager::Debug);
    driver.codegen("a.out");

    module = codegen->getModule();
    module->dump();
    
    return 0;
}
