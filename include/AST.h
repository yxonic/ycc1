#pragma once

#include <string>
#include <vector>

#include "ASTDump.h"

namespace ast {

    // Abstract class for ASTs
    class AST {
    public:
        AST() = default;
        virtual void dump(ASTDump &);
        std::string production;
        void append(AST *e) { components.push_back(e); }
    protected:
        std::vector<AST *> components;
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
        Decl(ConstDefs *);
        Decl(Vars *);
    };

    class ConstDefs : public AST {
    public:
        ConstDefs();
    };

    class ConstDef : public AST {
        bool is_array = false;
    public:
        ConstDef(Ident *, Exp *);
        ConstDef(Ident *, Exp *, Exps *);
    };

    class Vars : public AST {
    public:
        Vars();
    };

    class Var : public AST {
        bool is_array = false;
        bool init = false;
    public:
        Var(Ident *);
        Var(Ident *, Exp *, bool = false);
        Var(Ident *, Exp *, Exps *);
    };

    class FuncDef : public AST {
    public:
        FuncDef(Ident *, Block *);
    };
    
    class Stmt : public AST {
    public:
        Stmt();
    };
    
    class FuncCall : public Stmt {
    public:
        FuncCall(Ident *);
    };
    
    class Block : public Stmt {
    public:
        Block();
    };

    class AsgnStmt : public Stmt {
    public:
        AsgnStmt(LVal *, Exp *);
    };

    class IfStmt : public Stmt {
    public:
        IfStmt(Cond *, Stmt *, Stmt * = nullptr);
    };

    class WhileStmt : public Stmt {
    public:
        WhileStmt(Cond *, Stmt *);
    };

    class Cond : public AST {
        std::string op;
    public:
        Cond(Cond *);
        Cond(std::string, Cond *, Cond *);
        Cond(std::string, Exp *);
        Cond(std::string, Exp *, Exp *);
    };
    
    class Exp : public AST {
        // "()" for paren-expr, "P" for unary plus, "N" for unary
        // minus, "V" for LVal, and "Num" for number.
        std::string op;
    public:
        Exp() = default;
        Exp(std::string, Exp *);
        Exp(std::string, Exp *, Exp *);
        virtual int calc();
    };

    class Ident : public Exp {
        std::string name;
    public:
        Ident(std::string n) : name(n) { }
        int calc() override { return 0; }
    };

    class Number : public Exp {
        int value;
    public:
        Number(int n) : value(n) { }
        int calc() override { return value; }
    };

    class LVal : public Exp {
        Ident *id;
        bool is_array = false;
    public:
        LVal(Ident *, Exp * = nullptr);
        int calc() override { return id->calc(); }
    };

    class Exps : public AST {
    public:
        Exps();
    };
    
}
