#include "LLVMCodeGen.h"

#include <string>
#include <stack>

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;
using namespace ast;

#define dc dynamic_cast

void LLVMCodeGen::codegen(const std::shared_ptr<ast::AST> root)
{
    /// TODO: Add assertion on root. Add error handling.
    visit(root);
}

Value *error(std::string errstr)
{
    logger.error << errstr;
    return nullptr;
}

Value *LLVMCodeGen::visit(const std::shared_ptr<ast::AST> x)
{
    assert(x && "Can't codegen for nullptr");
    const auto &type = typeid(*x);
    if (type == typeid(CompUnit))
        return visitCompUnit(dc<const CompUnit&>(*x));
    else if (type == typeid(Decl))
        return visitDecl(dc<const Decl&>(*x));
    else if (type == typeid(FuncDef))
        return visitFuncDef(dc<const FuncDef&>(*x));
    else if (type == typeid(ConstDef))
        return visitConstDef(dc<const ConstDef&>(*x));
    else if (type == typeid(Param))
        return visitParam(dc<const Param&>(*x));
    else if (type == typeid(Var))
        return visitVar(dc<const Var&>(*x));
    else if (type == typeid(Block))
        return visitBlock(dc<const Block&>(*x));
    else if (type == typeid(AsgnStmt))
        return visitAsgnStmt(dc<const AsgnStmt&>(*x));
    else if (type == typeid(LVal))
        return visitLVal(dc<const LVal&>(*x));
    else if (type == typeid(Number))
        return visitNumber(dc<const Number&>(*x));
    else if (type == typeid(Exp))
        return visitExp(dc<const Exp&>(*x));
    else if (type == typeid(Cond))
        return visitCond(dc<const Cond&>(*x));
    else if (type == typeid(WhileStmt))
        return visitWhileStmt(dc<const WhileStmt&>(*x));
    else if (type == typeid(IfStmt))
        return visitIfStmt(dc<const IfStmt&>(*x));
    else if (type == typeid(FuncCall))
        return visitFuncCall(dc<const FuncCall&>(*x));
    else if (type == typeid(RetStmt))
        return visitRetStmt(dc<const RetStmt&>(*x));
    else
        return error(std::string("No code for AST node ") + type.name());
}

Value *LLVMCodeGen::visitCompUnit(const CompUnit &x)
{
    for (auto c : x.components)
        visit(c);
    return nullptr;
}

Value *LLVMCodeGen::visitDecl(const Decl &x)
{
    for (auto c : x.components[0]->components)
        visit(c);
    return nullptr;
}

Value *LLVMCodeGen::visitConstDef(const ConstDef &x)
{
    if (!x.is_array) {
        const Exp &e = dc<const Exp &>(*x.components[0]);
        int val = e.calc(_constants);
        _constants[x.name] = val;
        return _context.defConstant(x.name, val);
    } else {
        return nullptr;
    }
}

Value *LLVMCodeGen::visitVar(const Var &x)
{
    if (!x.is_array) {
        if (x.init) {
            const Exp &e = dc<const Exp &>(*x.components[0]);
            return _context.defVariable(x.name, e.calc(_constants));
        } else
            return _context.defVariable(x.name);
    } else {
        return nullptr;
    }
}

Value *LLVMCodeGen::visitFuncDef(const FuncDef &x)
{
    std::vector<Type*> args(x.components[0]->components.size(),
                            Type::getInt32Ty(getGlobalContext()));
    FunctionType *ft =
        FunctionType::get(Type::getInt32Ty(getGlobalContext()),
                          args, false);
    Function *f =
        Function::Create(ft, GlobalValue::ExternalLinkage,
                         x.name, _module.get());
    BasicBlock *EntryBB =
        BasicBlock::Create(_module->getContext(), "", f, 0);
    _builder.SetInsertPoint(EntryBB);

    _context.enterBlock(f);
    Function::arg_iterator ai = f->arg_begin();
    for (auto c : x.components[0]->components) {
        _builder.CreateStore(ai, visit(c));
        ++ai;
    }
    visit(x.components[1]);
    _context.exitBlock();
    
    verifyFunction(*f);
    return f;
}

Value *LLVMCodeGen::visitParam(const Param &x)
{
    if (!x.is_array) {
        return _context.defVariable(x.name);
    } else {
        return nullptr;
    }

}

Value *LLVMCodeGen::visitBlock(const Block &x)
{
    for (auto c : x.components)
        visit(c);
    return nullptr;
}

Value *LLVMCodeGen::visitAsgnStmt(const AsgnStmt &x)
{
    const LVal &v = dc<const LVal&>(*x.components[0]);
    Value *e = visit(x.components[1]);
    _builder.CreateStore(e, _context.getVariable(v.name));
    return e;
}

Value *LLVMCodeGen::visitExp(const Exp &x)
{
    if (x.op == 'N')
        return _builder.CreateNeg(visit(x.components[0]));

    Value *l = visit(x.components[0]);
    Value *r = visit(x.components[1]);
    switch (x.op) {
    case '+':
        return _builder.CreateAdd(l, r);
    case '-':
        return _builder.CreateSub(l, r);
    case '*':
        return _builder.CreateMul(l, r);
    case '/':
        return _builder.CreateSDiv(l, r);
    default:
        return error("Unknown operator");
    }
}

Value *LLVMCodeGen::visitLVal(const LVal &x)
{
    return _builder.CreateLoad(_context.get(x.name));
}

Value *LLVMCodeGen::visitNumber(const Number &x)
{
    return ConstantInt::get(getGlobalContext(), APInt(32, x.value));
}

Value *LLVMCodeGen::visitCond(const Cond &x)
{
    if (x.op == "!")
        return _builder.CreateNot(visit(x.components[0]));
    if (x.op == "and")
        return _builder.CreateAnd(visit(x.components[0]), visit(x.components[1]));
    if (x.op == "or")
        return _builder.CreateAnd(visit(x.components[0]), visit(x.components[1]));
    if (x.op == "==")
        return _builder.CreateICmpEQ(visit(x.components[0]), visit(x.components[1]));
    if (x.op == "!=")
        return _builder.CreateICmpNE(visit(x.components[0]), visit(x.components[1]));
    if (x.op == "<")
        return _builder.CreateICmpSLT(visit(x.components[0]), visit(x.components[1]));
    if (x.op == "<=")
        return _builder.CreateICmpSLE(visit(x.components[0]), visit(x.components[1]));
    if (x.op == ">")
        return _builder.CreateICmpSGT(visit(x.components[0]), visit(x.components[1]));
    if (x.op == ">=")
        return _builder.CreateICmpSGE(visit(x.components[0]), visit(x.components[1]));
    return error("Unknown condition.");
}

Value *LLVMCodeGen::visitWhileStmt(const WhileStmt &x)
{
    Function *func = _builder.GetInsertBlock()->getParent();
    BasicBlock *loop = BasicBlock::Create(getGlobalContext(), "loop", func);
    BasicBlock *after = BasicBlock::Create(getGlobalContext(), "afterloop", func);

    Value *cond = visit(x.components[0]);
    _builder.CreateCondBr(cond, loop, after);
    _builder.SetInsertPoint(loop);
    visit(x.components[1]);
    cond = visit(x.components[0]);
    _builder.CreateCondBr(cond, loop, after);
    _builder.SetInsertPoint(after);
    return nullptr;
}

Value *LLVMCodeGen::visitIfStmt(const IfStmt &x)
{
    Function *func = _builder.GetInsertBlock()->getParent();
    BasicBlock *thenBB = BasicBlock::Create(getGlobalContext(), "then", func);
    BasicBlock *elseBB = BasicBlock::Create(getGlobalContext(), "else", func);
    BasicBlock *mergeBB = BasicBlock::Create(getGlobalContext(), "merge", func);
    Value *cond = visit(x.components[0]);
    _builder.CreateCondBr(cond, thenBB, elseBB);
    _builder.SetInsertPoint(thenBB);
    visit(x.components[0]);
    _builder.CreateBr(mergeBB);
    if (x.components.size() > 1) {
        _builder.SetInsertPoint(elseBB);
        visit(x.components[1]);
        _builder.CreateBr(mergeBB);
    }
    _builder.SetInsertPoint(mergeBB);
    return nullptr;
}

Value *LLVMCodeGen::visitFuncCall(const FuncCall &x)
{
    Function *callee = _module->getFunction(x.name);
    std::vector<Value *> args;
    for (auto c : x.components[0]->components)
        args.push_back(visit(c));
    return _builder.CreateCall(callee, args);
}

Value *LLVMCodeGen::visitRetStmt(const RetStmt &x)
{
    return _builder.CreateRet(visit(x.components[0]));
}
