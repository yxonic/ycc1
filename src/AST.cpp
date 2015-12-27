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
        append(d);
    }

    Decl::Decl(shared_ptr<Vars> d)
    {
        production = "Decl -> int VarDefs ;";
        type = VAR;
        append(d);
    }

    void Decl::setLocal()
    {
        isGlobal = 0;
    }

    ConstDefs::ConstDefs()
    {
        production = "ConstDefs -> ConstDefs [ ConstDef ]";
    }

    ConstDef::ConstDef(string id, shared_ptr<Exp> e)
    {
        name = id;
        append(e);
        production = "ConstDef -> ID = Exp";
    }

    ConstDef::ConstDef(string id, shared_ptr<Exp> e, shared_ptr<Exps> el)
    {
        name = id;
        is_array = true;
        append(e);
        append(el);
        production = "ConstDef -> ID [ Exp ] = Exps";
    }

    Vars::Vars()
    {
        production = "Vars -> Vars [ Var ]";
    }

    Var::Var(string id)
    {
        name = id;
        production = "Var -> ID";
    }

    Var::Var(string id, shared_ptr<Exp> e, bool i)
    {
        name = id;
        append(e);
        init = i;
        is_array = !i;
        if (init)
            production = "Var -> ID = Exp";
        else
            production = "Var -> ID [ Exp ]";
    }

    Var::Var(string id, shared_ptr<Exp> e, shared_ptr<Exps> el)
    {
        name = id;
        append(e);
        append(el);
        production = "Var -> ID [ Exp ] = Exps";
    }

    FuncDef::FuncDef(string id, shared_ptr<Params>args, shared_ptr<Block> b)
    {
        name = id;
        append(args);
        append(b);
        production = "FuncDef -> void ID ( ) Block";
    }

    Params::Params()
    {
        production = "Params -> Params [Param]";
    }

    Param::Param(string id, bool b)
    {
        name = id;
        is_array = b;
    }

    Stmt::Stmt()
    {
        production = "Stmt -> ;";
    }

    FuncCall::FuncCall(string id, shared_ptr<Exps> e)
    {
        name = id;
        append(e);
        production = "Stmt -> void ID ( ) ;";
    }

    RetStmt::RetStmt(shared_ptr<Exp> e)
    {
        append(e);
        production = "Stmt -> return exp ;";
    }

    Block::Block()
    {
        production = "Block -> { Decl | Stmt }";
    }

    AsgnStmt::AsgnStmt(shared_ptr<LVal> v, shared_ptr<Exp> e)
    {
        append(v);
        append(e);
        production = "Stmt -> LVal = Exp";
    }

    IfStmt::IfStmt(shared_ptr<Cond> c, shared_ptr<Stmt> s1,
                   shared_ptr<Stmt> s2)
    {
        append(c);
        append(s1);
        production = "Stmt -> if ( Cond ) Stmt";
        if (s2) {
            append(s2);
            production += " else Stmt";
        }
    }

    WhileStmt::WhileStmt(shared_ptr<Cond> c, shared_ptr<Stmt> s)
    {
        append(c);
        append(s);
        production = "Stmt -> while ( Cond ) Stmt";
    }

    Cond::Cond(shared_ptr<Cond> c) : op("!")
    {
        append(c);
        production = "Cond -> ! Cond";
    }

    Cond::Cond(string s, shared_ptr<Cond> c1, shared_ptr<Cond> c2) : op(s)
    {
        append(c1);
        append(c2);
        production = "Cond -> Cond " + op + " Cond";
    }

    Cond::Cond(string s, shared_ptr<Exp> e) : op(s)
    {
        append(e);
        production = "Cond -> " + op + " Exp";
    }

    Cond::Cond(string s, shared_ptr<Exp> e1, shared_ptr<Exp> e2) : op(s)
    {
        append(e1);
        append(e2);
        production = "Cond -> Exp " + op + " Exp";
    }

    Exp::Exp(char c, shared_ptr<Exp> e) : op(c)
    {
        append(e);
        production = "Exp -> UnaryOp Exp";
    }

    Exp::Exp(char c, shared_ptr<Exp> e1, shared_ptr<Exp> e2) : op(c)
    {
        production = "Exp -> Exp " + string(1, op) + " Exp";
        append(e1);
        append(e2);
    }

    int Exp::calc() const {
        return calc(map<std::string, int>());
    }
    
    int Exp::calc(const map<std::string, int> &m) const {
        switch (op) {
        case 'N':
            return -dynamic_cast<Exp&>(*components[0]).calc(m);
        case '+':
            return dynamic_cast<Exp&>(*components[0]).calc(m) +
                dynamic_cast<Exp&>(*components[1]).calc(m);
        case '-':
            return dynamic_cast<Exp&>(*components[0]).calc(m) -
                dynamic_cast<Exp&>(*components[1]).calc(m);
        case '*':
            return dynamic_cast<Exp&>(*components[0]).calc(m) *
                dynamic_cast<Exp&>(*components[1]).calc(m);
        case '/':
            return dynamic_cast<Exp&>(*components[0]).calc(m) /
                dynamic_cast<Exp&>(*components[1]).calc(m);
        case '%':
            return dynamic_cast<Exp&>(*components[0]).calc(m) %
                dynamic_cast<Exp&>(*components[1]).calc(m);
        default:
            return 0;
        }
    }

    LVal::LVal(string id, shared_ptr<Exp> e)
    {
        name = id;
        production = "LVal -> ident";
        if (e) {
            is_array = true;
            production += " [ Exp ]";
            append(e);
        }
    }

    int LVal::calc(const map<string, int> &m) const {
        return m.at(name);
    }

    int Number::calc(const map<string, int> &m) const {
        return value;
    }

    Exps::Exps()
    {
        production = "Exps -> Exps [ Exp ]";
    }
    
}
