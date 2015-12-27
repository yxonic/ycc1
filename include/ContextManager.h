// -*- mode: c++ -*-

#pragma once

#include "llvm/IR/Module.h"

#include <list>
#include <map>
#include <stack>
#include <string>

class ContextManager {
public:
    ContextManager(llvm::Module *module) : _module(module) { enterBlock(nullptr); }
    void enterBlock(llvm::Function *f);
    void exitBlock();
    
    llvm::Value *get(std::string);
    llvm::Value *getConstant(std::string);
    llvm::Value *getVariable(std::string);
    llvm::Value *defConstant(std::string, int);
    llvm::Value *defVariable(std::string);
    llvm::Value *defVariable(std::string, int);
private:
    std::list<std::map<std::string, std::pair<llvm::Value *, bool>>> _sym_table;
    std::stack<llvm::Function *> _func;
    llvm::Module *_module;
};
