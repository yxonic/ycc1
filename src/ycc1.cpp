#include <iostream>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace std;
using namespace llvm;

int main()
{
    LLVMContext &context = getGlobalContext();
    Module *module = new Module("main", context);
    module->dump();
    return 0;
}
