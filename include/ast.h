#ifndef _AST_H_
#define _AST_H_

#include "llvm/IR/IRBuilder.h"
#include <cctype>
#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include "dumpdot.h"

using namespace llvm;

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//
namespace ast {
/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual Value *Codegen() = 0;
    virtual int dumpdot(DumpDOT *dumper) = 0;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double val) : Val(val) {}
    virtual Value *Codegen();
    int dumpdot(DumpDOT *dumper);
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string &name) : Name(name) {}
    virtual Value *Codegen();
    int dumpdot(DumpDOT *dumper);
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    ExprAST *LHS, *RHS;

public:
    BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
        : Op(op), LHS(lhs), RHS(rhs) {}
    virtual Value *Codegen();
    int dumpdot(DumpDOT *dumper);
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<ExprAST *> Args;

public:
    CallExprAST(const std::string &callee, std::vector<ExprAST *> &args)
        : Callee(callee), Args(args) {}
    virtual Value *Codegen();
    int dumpdot(DumpDOT *dumper);
};

/// IfExprAST - Expression class for if/then/else.
class IfExprAST : public ExprAST {
    ExprAST *Cond, *Then, *Else;

public:
    IfExprAST(ExprAST *cond, ExprAST *then, ExprAST *_else)
        : Cond(cond), Then(then), Else(_else) {}
    virtual Value *Codegen();
    int dumpdot(DumpDOT *dumper);
};

/// ForExprAST - Expression class for for/in.
class ForExprAST : public ExprAST {
    std::string VarName;
    ExprAST *Start, *End, *Step, *Body;

public:
    ForExprAST(const std::string &varname, ExprAST *start, ExprAST *end,
            ExprAST *step, ExprAST *body)
        : VarName(varname), Start(start), End(end), Step(step), Body(body) {}
    virtual Value *Codegen();
    int dumpdot(DumpDOT *dumper);
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &name, const std::vector<std::string> &args)
        : Name(name), Args(args) {}

    Function *Codegen();
    int dumpdot(DumpDOT *dumper);
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    PrototypeAST *Proto;
    ExprAST *Body;

public:
    FunctionAST(PrototypeAST *proto, ExprAST *body) : Proto(proto), Body(body) {}

    Function *Codegen();
    int dumpdot(DumpDOT *dumper);
};
} // end anonymous namespace

#endif
