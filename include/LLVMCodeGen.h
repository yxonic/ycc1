// -*- mode: c++ -*-

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"

#include "AST.h"
#include "ContextManager.h"

class LLVMCodeGen {
    std::unique_ptr<llvm::Module> _module;
    llvm::IRBuilder<> _builder;
    ContextManager _context;
    std::map<std::string, int> _constants;
    
public:
    LLVMCodeGen(std::unique_ptr<llvm::Module> &module) :
        _module(std::move(module)), _builder(llvm::getGlobalContext()), _context(_module.get()) { }
    void codegen(const std::shared_ptr<ast::AST>);
    std::unique_ptr<llvm::Module> &&getModule() { return std::move(_module); }

private:
    llvm::Value *visit(const std::shared_ptr<ast::AST>);
    llvm::Value *visitCompUnit(const ast::CompUnit &);
    llvm::Value *visitDecl(const ast::Decl &);
    llvm::Value *visitConstDef(const ast::ConstDef &);
    llvm::Value *visitVar(const ast::Var &);
    llvm::Value *visitFuncDef(const ast::FuncDef &);
    llvm::Value *visitParam(const ast::Param &);
    llvm::Value *visitBlock(const ast::Block &);
    llvm::Value *visitAsgnStmt(const ast::AsgnStmt &);
    llvm::Value *visitExp(const ast::Exp &);
    llvm::Value *visitLVal(const ast::LVal &);
    llvm::Value *visitNumber(const ast::Number &);
    llvm::Value *visitCond(const ast::Cond &);
    llvm::Value *visitWhileStmt(const ast::WhileStmt &);
    llvm::Value *visitIfStmt(const ast::IfStmt &);
    llvm::Value *visitFuncCall(const ast::FuncCall &);
    llvm::Value *visitRetStmt(const ast::RetStmt &);
};
