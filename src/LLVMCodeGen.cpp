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
    else if (type == typeid(Var))
        return visitVar(dc<const Var&>(*x));
    else if (type == typeid(Block))
        return visitBlock(dc<const Block&>(*x));
    else if (type == typeid(AsgnStmt))
        return visitAsgnStmt(dc<const AsgnStmt&>(*x));
    else if (type == typeid(LVal))
        return visitLVal(dc<const LVal&>(*x));
    else if (type == typeid(Ident))
        return visitIdent(dc<const Ident&>(*x));
    else if (type == typeid(Number))
        return visitNumber(dc<const Number&>(*x));
    else if (type == typeid(Exp))
        return visitExp(dc<const Exp&>(*x));
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
        const Exp &e = dc<const Exp &>(*x.components[1]);
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
            const Exp &e = dc<const Exp &>(*x.components[1]);
            return _context.defVariable(x.name, e.calc(_constants));
        } else
            return _context.defVariable(x.name);
    } else {
        return nullptr;
    }
}

Value *LLVMCodeGen::visitFuncDef(const FuncDef &x)
{
    std::vector<Type*> args;
    FunctionType *ft =
        FunctionType::get(IntegerType::get(_module->getContext(), 32),
                          args, false);
    Function *f =
        Function::Create(ft, GlobalValue::ExternalLinkage,
                         x.name, _module.get());
    BasicBlock *EntryBB =
        BasicBlock::Create(_module->getContext(), "", f, 0);
    _builder.SetInsertPoint(EntryBB);

    _context.enterBlock(f);
    visit(x.components[1]);
    _context.exitBlock();
    
    ConstantInt *rv = ConstantInt::get(getGlobalContext(), APInt(32, 0));
    _builder.CreateRet(rv);

    verifyFunction(*f);
    return f;
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

Value *LLVMCodeGen::visitIdent(const Ident &x)
{
    return _context.get(x.name);
}

Value *LLVMCodeGen::visitNumber(const Number &x)
{
    return ConstantInt::get(getGlobalContext(), APInt(32, x.value));
}

