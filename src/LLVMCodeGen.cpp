#include "LLVMCodeGen.h"

#include <stack>

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"

using namespace llvm;
using namespace ast;

void LLVMCodeGen::codegen(const std::shared_ptr<ast::AST> root)
{
    /// TODO: Add assertion on root. Add error handling.
    visit_recursive(root);
}

Value *LLVMCodeGen::visit_recursive(const std::shared_ptr<ast::AST> x)
{
    return nullptr;
}
