// -*- mode: c++ -*-

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"

#include "AST.h"

class LLVMCodeGen {
    std::unique_ptr<llvm::Module> _module;
    llvm::IRBuilder<> _builder;
    std::map<std::string, llvm::AllocaInst *> _sym_table;
    
public:
    LLVMCodeGen(std::unique_ptr<llvm::Module> module) :
        _module(std::move(module)), _builder(llvm::getGlobalContext()) { }
    llvm::Value *codegen(const std::shared_ptr<ast::AST>);
    
protected:
};
