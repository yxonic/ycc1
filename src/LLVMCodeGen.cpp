#include "LLVMCodeGen.h"

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"

using namespace llvm;
using namespace ast;

Value *LLVMCodeGen::codegen(const std::shared_ptr<ast::AST>)
{
    return nullptr;
}
