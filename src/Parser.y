%skeleton "lalr1.cc"

%defines
%define parser_class_name {Parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
#include <iostream>
#include <string>
#include <memory>
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
#include "Lexer.h"
#undef yylex
#define yylex driver.lexer->lex
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

%type   <std::shared_ptr<ast::Exp>> exp
%type   <std::shared_ptr<ast::LVal>> lval
%type   <std::shared_ptr<ast::Cond>> cond
%type   <std::shared_ptr<ast::Stmt>> stmt
%type   <std::shared_ptr<ast::Stmt>> matched
%type   <std::shared_ptr<ast::Stmt>> unmatched
%type   <std::shared_ptr<ast::Stmt>> otherstmt
%type   <std::shared_ptr<ast::Block>> block
%type   <std::shared_ptr<ast::Block>> blockitem
%type   <std::shared_ptr<ast::Decl>> decl
%type   <std::shared_ptr<ast::ConstDefs>> constdefs
%type   <std::shared_ptr<ast::ConstDef>> constdef
%type   <std::shared_ptr<ast::Vars>> vars
%type   <std::shared_ptr<ast::Var>> var
%type   <std::shared_ptr<ast::Exps>> exps
%type   <std::shared_ptr<ast::CompUnit>> compunit
%type   <std::shared_ptr<ast::FuncDef>> funcdef

%%

compunit:       %empty
                {
                    $$ = std::make_shared<ast::CompUnit>();
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
                    $$ = std::make_shared<ast::FuncDef>(
                        std::make_shared<ast::Ident>($2), $5);
                    INFO("%s", $$->production.c_str());
                }
        ;

stmt:           matched
        |       unmatched
        ;

matched:        "if" "(" cond ")" matched "else" matched
                {
                    $$ = std::make_shared<ast::IfStmt>($3, $5, $7);
                    INFO("%s", $$->production.c_str());
                }
        |       otherstmt
                {
                    $$ = $1;
                }
        ;
unmatched:      "if" "(" cond ")" stmt
                {
                    $$ = std::make_shared<ast::IfStmt>($3, $5);
                    INFO("%s", $$->production.c_str());
                }
        |       "if" "(" cond ")" matched "else" unmatched
                {
                    $$ = std::make_shared<ast::IfStmt>($3, $5, $7);
                    INFO("%s", $$->production.c_str());
                }
        ;

otherstmt:      lval "=" exp ";"
                {
                    $$ = std::make_shared<ast::AsgnStmt>($1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       "while" "(" cond ")" otherstmt
                {
                    $$ = std::make_shared<ast::WhileStmt>($3, $5);
                    INFO("%s", $$->production.c_str());
                }
        |       ";"
                {
                    $$ = std::make_shared<ast::Stmt>();
                    INFO("%s", $$->production.c_str());
                }
        |       ID "(" ")" ";"
                {
                    $$ = std::make_shared<ast::FuncCall>(
                        std::make_shared<ast::Ident>($1));
                    INFO("%s", $$->production.c_str());
                }
        |       block
                {
                    $$ = $1;
                }
        |       error ";"
                {
                    $$ = std::make_shared<ast::Stmt>();
                }
        ;

block:          "{" blockitem "}"
                {
                    $$ = $2;
                }
        ;

blockitem:      %empty
                {
                    $$ = std::make_shared<ast::Block>();
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
                    $$ = std::make_shared<ast::Decl>($3);
                    INFO("%s", $$->production.c_str());
                }
        |       "int" vars ";"
                {
                    $$ = std::make_shared<ast::Decl>($2);
                    INFO("%s", $$->production.c_str());
                }
        |       "const" constdefs ";"
                {
                    $$ = std::make_shared<ast::Decl>($2);
                    INFO("%s", $$->production.c_str());
                    WARN("Should generate warning.");
                }
        ;

constdefs:      constdef
                {
                    $$ = std::make_shared<ast::ConstDefs>();
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
                    $$ = std::make_shared<ast::ConstDef>(
                        std::make_shared<ast::Ident>($1), $3);
                    INFO("%s", $$->production.c_str());
                }
        |       ID "[" exp "]" "=" "{" exps "}"
                {
                    $$ = std::make_shared<ast::ConstDef>(
                        std::make_shared<ast::Ident>($1), $3, $7);
                    INFO("%s", $$->production.c_str());
                }
        ;

vars:           var
                {
                    $$ = std::make_shared<ast::Vars>();
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
                    $$ = std::make_shared<ast::Var>(
                        std::make_shared<ast::Ident>($1));
                    INFO("%s", $$->production.c_str());
                }
        |       ID "[" exp "]"
                {
                    $$ = std::make_shared<ast::Var>(
                        std::make_shared<ast::Ident>($1), $3);
                    INFO("%s", $$->production.c_str());
                }
        |       ID "=" exp
                {
                    // with initialization
                    $$ = std::make_shared<ast::Var>(
                        std::make_shared<ast::Ident>($1), $3, true);
                    INFO("%s", $$->production.c_str());
                }
        |       ID "[" exp "]" "=" "{" exps "}"
                {
                    $$ = std::make_shared<ast::Var>
                        (std::make_shared<ast::Ident>($1), $3, $7);
                    INFO("%s", $$->production.c_str());
                }
        ;

exps:           exp
                {
                    $$ = std::make_shared<ast::Exps>();
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
                    $$ = std::make_shared<ast::Exp>(
                        "Num", std::make_shared<ast::Number>($1));
                    INFO("%s", $$->production.c_str());
                }
        |       lval
                {
                    $$ = std::make_shared<ast::Exp>("V", $1);
                    INFO("%s", $$->production.c_str());
                }
        |       "-"exp %prec UNARY
                {
                    $$ = std::make_shared<ast::Exp>("N", $2);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "+" exp
                {
                    $$ = std::make_shared<ast::Exp>("+", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "-" exp
                {
                    $$ = std::make_shared<ast::Exp>("-", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "*" exp
                {
                    $$ = std::make_shared<ast::Exp>("*", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "/" exp
                {
                    if ($3 == 0)
                        ERROR("Can't be devided by zero.");
                    $$ = std::make_shared<ast::Exp>("/", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "%" exp
                {
                    $$ = std::make_shared<ast::Exp>("%", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       "(" exp ")"
                {
                    $$ = std::make_shared<ast::Exp>("()", $2);
                    INFO("%s", $$->production.c_str());
                }
        ;

lval:           ID
                {
                    $$ = std::make_shared<ast::LVal>(
                        std::make_shared<ast::Ident>($1));
                    INFO("%s", $$->production.c_str());
                }
        |       ID "[" exp "]"
                {
                    $$ = std::make_shared<ast::LVal>(
                        std::make_shared<ast::Ident>($1), $3);
                    INFO("%s", $$->production.c_str());
                }
        ;

cond:           "odd" exp
                {
                    $$ = std::make_shared<ast::Cond>("odd", $2);
                    INFO("%s", $$->production.c_str());
                }
        |       "!" cond %prec UNARY
                {
                    $$ = std::make_shared<ast::Cond>($2);
                    INFO("%s", $$->production.c_str());
                }
        |       cond "and" cond
                {
                    $$ = std::make_shared<ast::Cond>("and", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       cond "or" cond
                {
                    $$ = std::make_shared<ast::Cond>("or", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "==" exp
                {
                    $$ = std::make_shared<ast::Cond>("==", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "!=" exp
                {
                    $$ = std::make_shared<ast::Cond>("!=", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "<" exp
                {
                    $$ = std::make_shared<ast::Cond>("<", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp "<=" exp
                {
                    $$ = std::make_shared<ast::Cond>("<=", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp ">" exp
                {
                    $$ = std::make_shared<ast::Cond>(">", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        |       exp ">=" exp
                {
                    $$ = std::make_shared<ast::Cond>(">=", $1, $3);
                    INFO("%s", $$->production.c_str());
                }
        ;
%%
void yy::Parser::error(const location_type &l, const std::string& m)
{
    // TODO: should use facilities in ParsingDriver.
    std::cerr << l << ": " << m << std::endl;
}
