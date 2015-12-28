#include "ContextManager.h"

#include "llvm/IR/IRBuilder.h"

using namespace llvm;

void ContextManager::enterBlock(Function *f)
{
    _sym_table.emplace_back(std::map<std::string, std::pair<Value *, ElemType>>());
    _func.push(f);
}

void ContextManager::exitBlock()
{
    _sym_table.pop_back();
    _func.pop();
}

Value *ContextManager::get(std::string name) const
{
    for (auto i = _sym_table.rbegin(), e = _sym_table.rend();
         i != e; ++i) {
        auto x = i->find(name);
        if (x != i->end())
            return x->second.first;
    }
    return nullptr;
}

ContextManager::ElemType ContextManager::getType(std::string name) const
{
    for (auto i = _sym_table.rbegin(), e = _sym_table.rend();
         i != e; ++i) {
        auto x = i->find(name);
        if (x != i->end())
            return x->second.second;
    }
    return Error;
}

Value *ContextManager::defConstant(std::string name, int value)
{
    if (_sym_table.size() == 1) {
        GlobalVariable* gvar =
            new GlobalVariable(*_module, IntegerType::get(_module->getContext(), 32),
                               /*isConstant=*/true, GlobalValue::ExternalLinkage, 0, name);
        gvar->setInitializer(ConstantInt::get(getGlobalContext(), APInt(32, value)));
        _sym_table.back()[name] = {gvar, Constant};
        return gvar;
    } else {
        IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                         _func.top()->getEntryBlock().begin());
        AllocaInst *rv = tmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0,
                                           name);
        tmpB.CreateStore(ConstantInt::get(getGlobalContext(), APInt(32, value)), rv);
        _sym_table.back()[name] = {rv, Constant};
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
        _sym_table.back()[name] = {gvar, Int};
        return gvar;
    } else {
        IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                         _func.top()->getEntryBlock().begin());
        AllocaInst *rv = tmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0,
                                           name);
        _sym_table.back()[name] = {rv, Int};
        return rv;
    }
}

Value *ContextManager::defVariable(std::string name, int value)
{
    if (_sym_table.size() == 1) {
        GlobalVariable* gvar =
            new GlobalVariable(*_module, IntegerType::get(getGlobalContext(), 32),
                               /*isConstant=*/false, GlobalValue::ExternalLinkage, 0, name);
        gvar->setInitializer(ConstantInt::get(getGlobalContext(), APInt(32, value)));
        _sym_table.back()[name] = {gvar, Int};
        return gvar;
    } else {
        IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                         _func.top()->getEntryBlock().begin());
        AllocaInst *rv = tmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0,
                                           name);
        tmpB.CreateStore(ConstantInt::get(getGlobalContext(), APInt(32, value)), rv);
        _sym_table.back()[name] = {rv, Int};
        return rv;
    }
}

Value *ContextManager::defPointer(std::string name)
{
    IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                     _func.top()->getEntryBlock().begin());
    AllocaInst *rv = tmpB.CreateAlloca(Type::getInt32PtrTy(getGlobalContext()), 0,
                                       name);
    _sym_table.back()[name] = {rv, Pointer};
    return rv;
}

Value *ContextManager::defArray(std::string name, int size)
{
    if (_sym_table.size() == 1) {
        ArrayType *arrayTy = ArrayType::get(Type::getInt32Ty(getGlobalContext()), size);
        GlobalVariable *gvar =
            new GlobalVariable(*_module, arrayTy,
                               /*isConstant=*/false, GlobalValue::ExternalLinkage, 0, name);
        _sym_table.back()[name] = {gvar, Array};
        ConstantAggregateZero* values = ConstantAggregateZero::get(arrayTy);
        gvar->setInitializer(values);
        return gvar;
    } else {
        IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                         _func.top()->getEntryBlock().begin());
        AllocaInst *rv =
            tmpB.CreateAlloca(ArrayType::get(Type::getInt32Ty(getGlobalContext()), size),
                              0, name);
        _sym_table.back()[name] = {rv, Array};
        return rv;
    }
}

Value *ContextManager::defArray(std::string name, int size, std::vector<int> values)
{
    if (_sym_table.size() == 1) {
        ArrayType *arrayTy = ArrayType::get(Type::getInt32Ty(getGlobalContext()), size);
        GlobalVariable *gvar =
            new GlobalVariable(*_module, arrayTy,
                               /*isConstant=*/false, GlobalValue::ExternalLinkage, 0, name);
        _sym_table.back()[name] = {gvar, Array};
        std::vector<llvm::Constant*> consts;
        for (int v : values)
            consts.push_back(ConstantInt::get(getGlobalContext(), APInt(32, v)));
        gvar->setInitializer(ConstantArray::get(arrayTy, ArrayRef<llvm::Constant*>(consts)));
        return gvar;
    } else {
        IRBuilder<> tmpB(&_func.top()->getEntryBlock(),
                         _func.top()->getEntryBlock().begin());
        AllocaInst *rv =
            tmpB.CreateAlloca(ArrayType::get(Type::getInt32Ty(getGlobalContext()), size),
                              0, name);
        _sym_table.back()[name] = {rv, Array};
        return rv;
    }
}
