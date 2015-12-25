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
    LLVMCodeGen(llvm::Module *module) :
        _module(module), _builder(llvm::getGlobalContext()) { }
    void codegen(const std::shared_ptr<ast::AST>);
    llvm::Module *getModule() { return _module.get(); }
    
private:
    llvm::Value *visit_recursive(const std::shared_ptr<ast::AST>);
};
