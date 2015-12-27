#include "ContextManager.h"

#include "llvm/IR/IRBuilder.h"

using namespace llvm;

void ContextManager::enterBlock(Function *f)
{
    _sym_table.emplace_back(std::map<std::string, std::pair<Value *, bool>>());
    _func.push(f);
}

void ContextManager::exitBlock()
{
    _sym_table.pop_back();
    _func.pop();
}

Value *ContextManager::get(std::string name)
{
    for (auto i = _sym_table.rbegin(), e = _sym_table.rend();
         i != e; ++i) {
        auto x = i->find(name);
        if (x != i->end())
            return x->second.first;
    }
    return nullptr;
}

Value *ContextManager::getConstant(std::string name)
{
    for (auto i = _sym_table.rbegin(), e = _sym_table.rend();
         i != e; ++i) {
        auto x = i->find(name);
        if (x != i->end()) {
            if (x->second.second)
                return x->second.first;
            else
                return nullptr;
        }
    }
    return nullptr;
}

Value *ContextManager::getVariable(std::string name)
{
    for (auto i = _sym_table.rbegin(), e = _sym_table.rend();
         i != e; ++i) {
        auto x = i->find(name);
        if (x != i->end()) {
            if (x->second.second)
                return nullptr;
            else
                return x->second.first;
        }
    }
    return nullptr;
}

Value *ContextManager::defConstant(std::string name, int value)
{
    if (_sym_table.size() == 1) {
        GlobalVariable* gvar =
            new GlobalVariable(*_module, IntegerType::get(_module->getContext(), 32),
                               /*isConstant=*/true, GlobalValue::ExternalLinkage, 0, name);
        gvar->setInitializer(ConstantInt::get(getGlobalContext(), APInt(32, value)));
        _sym_table.back()[name] = {gvar, true};
        return gvar;
    } else {
        IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                         _func.top()->getEntryBlock().begin());
        AllocaInst *rv = tmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0,
                                           name);
        tmpB.CreateStore(ConstantInt::get(getGlobalContext(), APInt(32, value)), rv);
        _sym_table.back()[name] = {rv, true};
        return rv;
    }
}

Value *ContextManager::defVariable(std::string name)
{
    if (_sym_table.size() == 1) {
        GlobalVariable* gvar =
            new GlobalVariable(*_module, IntegerType::get(_module->getContext(), 32),
                               /*isConstant=*/false, GlobalValue::ExternalLinkage, 0, name);
        gvar->setInitializer(ConstantInt::get(getGlobalContext(), APInt(32, 0)));
        _sym_table.back()[name] = {gvar, false};
        return gvar;
    } else {
        IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                         _func.top()->getEntryBlock().begin());
        AllocaInst *rv = tmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0,
                                           name);
        _sym_table.back()[name] = {rv, false};
        return rv;
    }
}

Value *ContextManager::defVariable(std::string name, int value)
{
    if (_sym_table.size() == 1) {
        GlobalVariable* gvar =
            new GlobalVariable(*_module, IntegerType::get(_module->getContext(), 32),
                               /*isConstant=*/false, GlobalValue::CommonLinkage, 0, name);
        gvar->setInitializer(ConstantInt::get(getGlobalContext(), APInt(32, value)));
        _sym_table.back()[name] = {gvar, false};
        return gvar;
    } else {
        IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                         _func.top()->getEntryBlock().begin());
        AllocaInst *rv = tmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0,
                                           name);
        tmpB.CreateStore(ConstantInt::get(getGlobalContext(), APInt(32, value)), rv);
        _sym_table.back()[name] = {rv, false};
        return rv;
    }
}
