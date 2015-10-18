%skeleton "lalr1.cc"

%defines
%define parser_class_name {Parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
#include <string>
#include "Utils.h"
#include "AST.h"
class ParsingDriver;
}

%param { ParsingDriver &driver }
%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = &driver.filename;
}

%define parse.trace
%define parse.error verbose

%code
{
#include "ParsingDriver.h"
}

%define api.token.prefix {TOK_}

%token                  END 0           "end of file"
                        ASSIGN          "="
                        PLUS            "+"
                        MINUS           "-"
                        STAR            "*"
                        SLASH           "/"
                        MOD             "%"
                        LPAREN          "("
                        RPAREN          ")"
                        LBRACKET        "["
                        RBRACKET        "]"
                        LBRACE          "{"
                        RBRACE          "}"
                        EQ              "=="
                        NE              "!="
                        LT              "<"
                        LE              "<="
                        GT              ">"
                        GE              ">="
                        NOT             "!"
                        OR              "or"
                        AND             "and"
                        ODD             "odd"
                        SEMI            ";"
                        COMMA           ","
                        IF              "if"
                        ELSE            "else"
                        WHILE           "while"
                        CONST           "const"
                        INT             "int"
                        VOID            "void"
     ;

%token  <std::string> ID
%token  <int> NUMBER

%left "and"
%left "or"
%left "+" "-"
%left "*" "/" "%"
%precedence UNARY

%type   <ast::Exp *> exp
%type   <ast::LVal *> lval
%type   <ast::Cond *> cond
%type   <ast::Stmt *> stmt
%type   <ast::Stmt *> matched
%type   <ast::Stmt *> unmatched
%type   <ast::Stmt *> otherstmt
%type   <ast::Block *> block
%type   <ast::Block *> blockitem
%type   <ast::Decl *> decl
%type   <ast::ConstDefs *> constdefs
%type   <ast::ConstDef *> constdef
%type   <ast::Vars *> vars
%type   <ast::Var *> var
%type   <ast::Exps *> exps
%type   <ast::CompUnit *> compunit
%type   <ast::FuncDef *> funcdef

%%

compunit:       %empty
                {
                    $$ = new ast::CompUnit();
                    INFO("%s", $$->production.c_str());
                    driver.root = $$;
                }
        |       compunit funcdef
                {
                    $$ = $1;
                    $$->append($2);
                }
        |       compunit decl
                {
                    $$ = $1;
                    $$->append($2);
                }
        ;

funcdef:        "void" ID "(" ")" block
                {
                    $$ = new ast::FuncDef(new ast::Ident($2), $5);
                    INFO("%s", $$->production.c_str());
                }
        ;

stmt:           matched
        |       unmatched
        ;

matched:        "if" "(" cond ")" matched "else" matched
                {
                    $$ = new ast::IfStmt($3, $5, $7);
                    INFO("%s", $$->production.c_str());
                }
        |       otherstmt
                {
                    $$ = $1;
                }
        ;
unmatched:      "if" "(" cond ")" stmt
                {
                    $$ = new ast::IfStmt($3, $5);
                    INFO("%s", $$->production.c_str());
                }
        |       "if" "(" cond ")" matched "else" unmatched
                {
                    $$ = new ast::IfStmt($3, $5, $7);
                    INFO("%s", $$->production.c_str());
                }
        ;

otherstmt:      lval "=" exp ";"
                {
                    $$ = new ast::AsgnStmt($1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       "while" "(" cond ")" otherstmt
                {
                    $$ = new ast::WhileStmt($3, $5);
                    INFO("%s", $$->production.c_str());
                }
        |       ";"
                {
                    $$ = new ast::Stmt();
                    INFO("%s", $$->production.c_str());
                }
        |       ID "(" ")" ";"
                {
                    $$ = new ast::FuncCall(new ast::Ident($1));
                    INFO("%s", $$->production.c_str());
                }
        |       block
                {
                    $$ = $1;
                }
        ;

block:          "{" blockitem "}"
                {
                    $$ = $2;
                }
        ;

blockitem:      %empty
                {
                    $$ = new ast::Block();
                    INFO("%s", $$->production.c_str());
                }
        |       blockitem stmt
                {
                    $$ = $1;
                    $$->append($2);
                }
        |       blockitem decl
                {
                    $$ = $1;
                    $$->append($2);
                }
        ;

decl:           "const" "int" constdefs ";"
                {
                    $$ = new ast::Decl($3);
                    INFO("%s", $$->production.c_str());
                }
        |       "int" vars ";"
                {
                    $$ = new ast::Decl($2);
                    INFO("%s", $$->production.c_str());
                }
        ;

constdefs:      constdef
                {
                    $$ = new ast::ConstDefs();
                    INFO("%s", $$->production.c_str());
                    $$->append($1);
                }
        |       constdefs "," constdef
                {
                    $$ = $1;
                    $$->append($3);
                }
        ;

constdef:       ID "=" exp
                {
                    $$ = new ast::ConstDef(new ast::Ident($1), $3);
                    INFO("%s", $$->production.c_str());
                }
        |       ID "[" exp "]" "=" "{" exps "}"
                {
                    $$ = new ast::ConstDef(new ast::Ident($1), $3, $7);
                    INFO("%s", $$->production.c_str());
                }
        ;

vars:           var
                {
                    $$ = new ast::Vars();
                    INFO("%s", $$->production.c_str());
                    $$->append($1);
                }
        |       vars "," var
                {
                    $$ = $1;
                    $$->append($3);
                }
        ;

var:            ID
                {
                    $$ = new ast::Var(new ast::Ident($1));
                    INFO("%s", $$->production.c_str());
                }
        |       ID "[" exp "]"
                {
                    $$ = new ast::Var(new ast::Ident($1), $3);
                    INFO("%s", $$->production.c_str());
                }
        |       ID "=" exp
                {
                    // with initialization
                    $$ = new ast::Var(new ast::Ident($1), $3, true);
                    INFO("%s", $$->production.c_str());
                }
        |       ID "[" exp "]" "=" "{" exps "}"
                {
                    $$ = new ast::Var(new ast::Ident($1), $3, $7);
                    INFO("%s", $$->production.c_str());
                }
        ;

exps:           exp
                {
                    $$ = new ast::Exps();
                    INFO("%s", $$->production.c_str());
                    $$->append($1);
                }
        |       exps "," exp
                {
                    $$ = $1;
                    $$->append($3);
                }
        ;

exp:            NUMBER
                {
                    $$ = new ast::Exp("Num", new ast::Number($1));
                    INFO("%s", $$->production.c_str());
                }
        |       lval
                {
                    $$ = new ast::Exp("V", $1);
                    INFO("%s", $$->production.c_str());
                }
        |       "-"exp %prec UNARY
                {
                    $$ = new ast::Exp("N", $2);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "+" exp
                {
                    $$ = new ast::Exp("+", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "-" exp
                {
                    $$ = new ast::Exp("-", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "*" exp
                {
                    $$ = new ast::Exp("*", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "/" exp
                {
                    if ($3 == 0)
                        ERROR("Can't be devided by zero.");
                    $$ = new ast::Exp("/", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "%" exp
                {
                    $$ = new ast::Exp("%", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       "(" exp ")"
                {
                    $$ = new ast::Exp("()", $2);
                    INFO("%s", $$->production.c_str());
                }
        ;

lval:           ID
                {
                    $$ = new ast::LVal(new ast::Ident($1));
                    INFO("%s", $$->production.c_str());
                }
        |       ID "[" exp "]"
                {
                    $$ = new ast::LVal(new ast::Ident($1), $3);
                    INFO("%s", $$->production.c_str());
                }
        ;

cond:           "odd" exp
                {
                    $$ = new ast::Cond("odd", $2);
                    INFO("%s", $$->production.c_str());
                }
        |       "!" cond %prec UNARY
                {
                    $$ = new ast::Cond($2);
                    INFO("%s", $$->production.c_str());
                }
        |       cond "and" cond
                {
                    $$ = new ast::Cond("and", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       cond "or" cond
                {
                    $$ = new ast::Cond("or", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "==" exp
                {
                    $$ = new ast::Cond("==", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "!=" exp
                {
                    $$ = new ast::Cond("!=", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "<" exp
                {
                    $$ = new ast::Cond("<", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "<=" exp
                {
                    $$ = new ast::Cond("<=", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp ">" exp
                {
                    $$ = new ast::Cond(">", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp ">=" exp
                {
                    $$ = new ast::Cond(">=", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        ;
%%
void yy::Parser::error(const location_type &l, const std::string& m)
{
    ERROR("Error occured!");
}
