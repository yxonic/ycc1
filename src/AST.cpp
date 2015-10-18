#include "AST.h"

using namespace std;

namespace ast {

    void AST::dump(ASTDump &)
    {
    }
    
    CompUnit::CompUnit()
    {
        production = "CompUnit -> CompUnit [Decl | FuncDef]";
    }

    Decl::Decl(shared_ptr<ConstDefs> d)
    {
        production = "Decl -> const int ConstDefs ;";
        type = CONST;
        components.push_back(d);
    }

    Decl::Decl(shared_ptr<Vars> d)
    {
        production = "Decl -> int VarDefs ;";
        type = VAR;
        components.push_back(d);
    }

    ConstDefs::ConstDefs()
    {
        production = "ConstDefs -> ConstDefs [ ConstDef ]";
    }

    ConstDef::ConstDef(shared_ptr<Ident> id, shared_ptr<Exp> e)
    {
        components.push_back(id);
        components.push_back(e);
        production = "ConstDef -> ID = Exp";
    }

    ConstDef::ConstDef(shared_ptr<Ident> id, shared_ptr<Exp> e, shared_ptr<Exps> el)
    {
        is_array = true;
        components.push_back(id);
        components.push_back(e);
        components.push_back(el);
        production = "ConstDef -> ID [ Exp ] = Exps";

    }

    Vars::Vars()
    {
        production = "Vars -> Vars [ Var ]";
    }

    Var::Var(shared_ptr<Ident> id)
    {
        components.push_back(id);
        production = "Var -> ID";
    }

    Var::Var(shared_ptr<Ident> id, shared_ptr<Exp> e, bool i)
    {
        components.push_back(id);
        components.push_back(e);
        init = i;
        is_array = !i;
        if (init)
            production = "Var -> ID = Exp";
        else
            production = "Var -> ID [ Exp ]";
    }

    Var::Var(shared_ptr<Ident> id, shared_ptr<Exp> e, shared_ptr<Exps> el)
    {
        components.push_back(id);
        components.push_back(e);
        components.push_back(el);
        production = "Var -> ID [ Exp ] = Exps";
    }

    FuncDef::FuncDef(shared_ptr<Ident> id, shared_ptr<Block> b)
    {
        components.push_back(id);
        components.push_back(b);
        production = "FuncDef -> void ID ( ) Block";
    }
    
    Stmt::Stmt()
    {
        production = "Stmt -> ;";
    }

    FuncCall::FuncCall(shared_ptr<Ident> id)
    {
        components.push_back(id);
        production = "Stmt -> void ID ( ) ;";
    }

    Block::Block()
    {
        production = "Block -> { Decl | Stmt }";
    }    

    AsgnStmt::AsgnStmt(shared_ptr<LVal> v, shared_ptr<Exp> e)
    {
        components.push_back(v);
        components.push_back(e);
        production = "Stmt -> LVal = Exp"; 
    }

    IfStmt::IfStmt(shared_ptr<Cond> c, shared_ptr<Stmt> s1, shared_ptr<Stmt> s2)
    {
        components.push_back(c);
        components.push_back(s1);
        production = "Stmt -> if ( Cond ) Stmt";
        if (s2) {
            components.push_back(s2);
            production += " else Stmt";
        }
    }

    WhileStmt::WhileStmt(shared_ptr<Cond> c, shared_ptr<Stmt> s)
    {
        components.push_back(c);
        components.push_back(s);
        production = "Stmt -> while ( Cond ) Stmt";
    }
    
    Cond::Cond(shared_ptr<Cond> c) : op("!")
    {
        components.push_back(c);
        production = "Cond -> ! Cond";
    }

    Cond::Cond(string s, shared_ptr<Cond> c1, shared_ptr<Cond> c2) : op(s)
    {
        components.push_back(c1);
        components.push_back(c2);
        production = "Cond -> Cond " + op + " Cond";
    }

    Cond::Cond(string s, shared_ptr<Exp> e) : op(s)
    {
        components.push_back(e);
        production = "Cond -> " + op + " Exp";
    }

    Cond::Cond(string s, shared_ptr<Exp> e1, shared_ptr<Exp> e2) : op(s)
    {
        components.push_back(e1);
        components.push_back(e2);
        production = "Cond -> Exp " + op + " Exp";
    }
    
    Exp::Exp(string s, shared_ptr<Exp> e) : op(s)
    {
        components.push_back(e);
        if (s == "()")
            production = "Exp -> (Exp)";
        else if (s == "V")
            production = "Exp -> LVal";
        else if (s == "Num")
            production = "Exp -> Number";
        else
            production = "Exp -> UnaryOp Exp";
    }
    
    Exp::Exp(string s, shared_ptr<Exp> e1, shared_ptr<Exp> e2) : op(s)
    {
        production = "Exp -> Exp " + op + " Exp";
        components.push_back(e1);
        components.push_back(e2);
    }    

    LVal::LVal(shared_ptr<Ident> i, shared_ptr<Exp> e)
    {
        production = "LVal -> ident";
        if (e) {
            is_array = true;
            production += " [ Exp ]";
            components.push_back(e);
        }
    }

    Exps::Exps()
    {
        production = "Exps -> Exps [ Exp ]";
    }

    int Exp::calc()
    {
        if (op == "()" || op == "P")
            return dynamic_cast<Exp *>(components[0].get())->calc();
        if (op == "N")
            return -dynamic_cast<Exp *>(components[0].get())->calc();
        if (op == "+")
            return dynamic_cast<Exp *>(components[0].get())->calc() +
                dynamic_cast<Exp *>(components[1].get())->calc();
        if (op == "-")
            return dynamic_cast<Exp *>(components[0].get())->calc() -
                dynamic_cast<Exp *>(components[1].get())->calc();
        if (op == "*")
            return dynamic_cast<Exp *>(components[0].get())->calc() *
                dynamic_cast<Exp *>(components[1].get())->calc();
        if (op == "/")
            return dynamic_cast<Exp *>(components[0].get())->calc() /
                dynamic_cast<Exp *>(components[1].get())->calc();
        if (op == "%")
            return dynamic_cast<Exp *>(components[0].get())->calc() %
                dynamic_cast<Exp *>(components[1].get())->calc();
        if (op == "Num")
            return dynamic_cast<Exp *>(components[0].get())->calc();
        else
            return 0;
    }
    
}
