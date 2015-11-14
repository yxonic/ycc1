// -*- mode: c++ -*-

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "location.hh"
#include "Utils.h"
#include "ASTDump.h"

namespace ast {

    /// AST - Abstract class for ASTs
    class AST {
    public:
        // Default constructor
        AST() = default;

        /// Dump AST structure using a dumper.
        virtual int dump(ASTDump &dumper) const;

        /// Production rule that generates this node.
        std::string production;

        /// Position information of the unit.
        yy::location loc;

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
    class Block;
    class Stmt;
    class LVal;
    class Cond;
    class Exp;
    class Ident;
    class Number;
    class Exps;

    // Definitions

    class CompUnit : public AST {
    public:
        CompUnit();
    };

    class Decl : public AST {
    public:
        enum Type { CONST, VAR };
        Type type;
        Decl(std::shared_ptr<ConstDefs>);
        Decl(std::shared_ptr<Vars>);
    };

    class ConstDefs : public AST {
    public:
        ConstDefs();
    };

    class ConstDef : public AST {
    public:
        bool is_array = false;
        ConstDef(std::shared_ptr<Ident>, std::shared_ptr<Exp>);
        ConstDef(std::shared_ptr<Ident>, std::shared_ptr<Exp>,
                 std::shared_ptr<Exps>);
    };

    class Vars : public AST {
    public:
        Vars();
    };

    class Var : public AST {
    public:
        bool is_array = false;
        bool init = false;
        Var(std::shared_ptr<Ident>);
        Var(std::shared_ptr<Ident>, std::shared_ptr<Exp>, bool = false);
        Var(std::shared_ptr<Ident>, std::shared_ptr<Exp>,
            std::shared_ptr<Exps>);
    };

    class FuncDef : public AST {
    public:
        FuncDef(std::shared_ptr<Ident>, std::shared_ptr<Block>);
    };

    class Stmt : public AST {
    public:
        Stmt();
    };

    class FuncCall : public Stmt {
    public:
        FuncCall(std::shared_ptr<Ident>);
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
        // "()" for paren-expr, "P" for unary plus, "N" for unary
        // minus, "V" for LVal, and "Num" for number.
        std::string op;
        Exp() = default;
        Exp(std::string, std::shared_ptr<Exp>);
        Exp(std::string, std::shared_ptr<Exp>, std::shared_ptr<Exp>);
        virtual int calc();
    };

    class Ident : public Exp {
    public:
        std::string name;
        Ident(std::string n) : name(n) {
            production = n;
        }
        int calc() override { return 0; }
    };

    class Number : public Exp {
    public:
        int value;
        Number(int n) : value(n) {
            production = std::to_string(n);
        }
        int calc() override { return value; }
    };

    class LVal : public Exp {
    public:
        bool is_array = false;
        LVal(std::shared_ptr<Ident>, std::shared_ptr<Exp> = nullptr);
        int calc() override { return 0; }
    };

    class Exps : public AST {
    public:
        Exps();
    };

}
