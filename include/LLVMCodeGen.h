// -*- mode: c++ -*-

#pragma once

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"

#include "AST.h"
#include "ContextManager.h"
#include "Driver.h"

class LLVMCodeGen {
    const Driver *_driver;
    std::unique_ptr<llvm::Module> _module;
    llvm::IRBuilder<> _builder;
    ContextManager _context;
    std::map<std::string, int> _constants;
    
public:
    LLVMCodeGen(llvm::Module *module) :
        _module(module), _builder(llvm::getGlobalContext()), _context(module) { }
    void codegen(const Driver &, std::string);
    std::unique_ptr<llvm::Module> &&getModule() { return std::move(_module); }

private:
    llvm::Value *error(std::string);
    llvm::Value *visit(const std::shared_ptr<ast::AST>);
    llvm::Value *visitCompUnit(const ast::CompUnit &);
    llvm::Value *visitDecl(const ast::Decl &);
    llvm::Value *visitConstDef(const ast::ConstDef &);
    llvm::Value *visitVar(const ast::Var &);
    llvm::Value *visitFuncDef(const ast::FuncDef &);
    llvm::Value *visitExtFunc(const ast::ExtFunc &);
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
    llvm::Value *visitCallExp(const ast::CallExp &);
    llvm::Value *visitRetStmt(const ast::RetStmt &);
};
