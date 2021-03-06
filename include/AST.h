// -*- mode: c++ -*-

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "llvm/IR/Module.h"

#include "location.hh"
#include "Utils.h"
#include "ASTDump.h"

namespace ast {

    /// AST - Abstract class for ASTs
    class AST {
    public:
        /// Default constructor.
        AST() = default;

        /// Dump AST structure using a dumper.
        virtual int dump(ASTDump &dumper) const;

        /// Production rule that generates this node.
        std::string production;

        /// Components (childs) of this node.
        std::vector<std::shared_ptr<AST>> components;

        /// Append a child.
        void append(std::shared_ptr<AST> e) { components.push_back(e); }
    };

    // AST declarations for specific grammar elements

    class CompUnit;
    class Decl;
    class ConstDefs;
    class ConstDef;
    class Vars;
    class Var;
    class FuncDef;
    class ExtFunc;
    class Params;
    class Param;
    class Block;
    class Stmt;
    class FuncCall;
    class RetStmt;
    class LVal;
    class Cond;
    class Exp;
    class CallExp;
    class Number;
    class Exps;

    // Definitions

    class CompUnit : public AST {
    public:
        CompUnit();
    };

    class Decl : public AST {
    public:
        enum Type { CONST, VAR } type;
        Decl() = default;
        Decl(std::shared_ptr<ConstDefs>);
        Decl(std::shared_ptr<Vars>);
        bool isGlobal = 1;
        void setLocal();
    };

    class ConstDefs : public AST {
    public:
        ConstDefs();
    };

    class ConstDef : public Decl {
    public:
        std::string name;
        bool isArray = false;
        ConstDef(std::string, std::shared_ptr<Exp>);
        ConstDef(std::string, std::shared_ptr<Exp>,
                 std::shared_ptr<Exps>);
    };

    class Vars : public AST {
    public:
        Vars();
    };

    class Var : public Decl {
    public:
        std::string name;
        bool isArray = false;
        bool init = false;
        Var(std::string);
        Var(std::string, std::shared_ptr<Exp>, bool = false);
        Var(std::string, std::shared_ptr<Exp>,
            std::shared_ptr<Exps>);
    };

    class Params : public AST {
    public:
        Params();
    };

    class Param : public AST {
    public:
        std::string name;
        bool isArray = false;
        Param(std::string, bool = false);
    };
    
    class FuncDef : public AST {
    public:
        std::string name;
        FuncDef(std::string, std::shared_ptr<Params>,
                std::shared_ptr<Block>);
    };

    class ExtFunc : public AST {
    public:
        std::string name;
        ExtFunc(std::string, std::shared_ptr<Params>);
    };

    class Stmt : public AST {
    public:
        Stmt();
    };

    class FuncCall : public Stmt {
    public:
        std::string name;
        FuncCall(std::string, std::shared_ptr<Exps>);
    };

    class RetStmt : public Stmt {
    public:
        RetStmt(std::shared_ptr<Exp>);
    };

    class Block : public Stmt {
    public:
        Block();
    };

    class AsgnStmt : public Stmt {
    public:
        AsgnStmt(std::shared_ptr<LVal>, std::shared_ptr<Exp>);
    };

    class IfStmt : public Stmt {
    public:
        IfStmt(std::shared_ptr<Cond>, std::shared_ptr<Stmt>,
               std::shared_ptr<Stmt> = nullptr);
    };

    class WhileStmt : public Stmt {
    public:
        WhileStmt(std::shared_ptr<Cond>, std::shared_ptr<Stmt>);
    };

    class Cond : public AST {
    public:
        std::string op;
        Cond(std::shared_ptr<Cond>);
        Cond(std::string, std::shared_ptr<Cond>, std::shared_ptr<Cond>);
        Cond(std::string, std::shared_ptr<Exp>);
        Cond(std::string, std::shared_ptr<Exp>, std::shared_ptr<Exp>);
    };

    class Exp : public AST {
    public:
        char op;
        Exp() = default;
        Exp(char, std::shared_ptr<Exp>);
        Exp(char, std::shared_ptr<Exp>, std::shared_ptr<Exp>);
        virtual int calc() const;
        virtual int calc(const std::map<std::string, int> &) const;
    };

    class Number : public Exp {
    public:
        int value;
        Number(int n) : value(n) {
            production = std::to_string(n);
        }
        virtual int calc(const std::map<std::string, int> &) const;
    };

    class LVal : public Exp {
    public:
        std::string name;
        bool isArray = false;
        LVal(std::string, std::shared_ptr<Exp> = nullptr);
        virtual int calc(const std::map<std::string, int> &) const;
    };

    class CallExp : public Exp {
    public:
        std::string name;
        CallExp(std::string, std::shared_ptr<Exps>);
    };

    class Exps : public AST {
    public:
        Exps();
    };

}
