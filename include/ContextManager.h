// -*- mode: c++ -*-

#pragma once

#include "llvm/IR/Module.h"

#include <list>
#include <map>
#include <stack>
#include <string>
#include <vector>

class ContextManager {
public:
    enum ElemType { Constant, Int, Pointer, Array, Error };
    ContextManager(llvm::Module *module) : _module(module) { enterBlock(nullptr); }
    void enterBlock(llvm::Function *f);
    void exitBlock();
    
    llvm::Value *get(std::string) const;
    llvm::Value *defConstant(std::string, int);
    llvm::Value *defVariable(std::string);
    llvm::Value *defVariable(std::string, int);
    llvm::Value *defPointer(std::string);
    llvm::Value *defArray(std::string, int);
    llvm::Value *defArray(std::string, int, std::vector<int>);
    ElemType getType(std::string) const;
private:
    std::list<std::map<std::string, std::pair<llvm::Value *, ElemType>>> _sym_table;
    std::stack<llvm::Function *> _func;
    llvm::Module *_module;
};
