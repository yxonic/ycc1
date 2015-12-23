#include "AST.h"
#include <sstream>

using namespace std;

namespace ast {

    int AST::dump(ASTDump &dumper) const
    {
        ostringstream os;
        os << production;
        int s = dumper.newNode(os.str());
        vector<int> childs;
        for (auto c : components)
            childs.push_back(c->dump(dumper));
        for (int t : childs)
            dumper.drawLine(s, t);
        return s;
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

    ConstDef::ConstDef(shared_ptr<Ident> id, shared_ptr<Exp> e,
                       shared_ptr<Exps> el)
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

    IfStmt::IfStmt(shared_ptr<Cond> c, shared_ptr<Stmt> s1,
                   shared_ptr<Stmt> s2)
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

    Exp::Exp(char c, shared_ptr<Exp> e) : op(c)
    {
        components.push_back(e);
        production = "Exp -> UnaryOp Exp";
    }

    Exp::Exp(char c, shared_ptr<Exp> e1, shared_ptr<Exp> e2) : op(c)
    {
        production = "Exp -> Exp " + string(1, op) + " Exp";
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
    
}
