%skeleton "lalr1.cc"

%defines
%define parser_class_name {Parser}
%define api.token.constructor
%define api.value.type variant
%code requires
{
#include <iostream>
#include <string>
#include <memory>
#include "Utils.h"
#include "AST.h"
class Driver;
}

%param { Driver &driver }
%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = &driver._file_name;
}

%define parse.error verbose

%code
{
#include "Driver.h"
#include "Lexer.h"
#undef yylex
#define yylex driver._lexer->lex
}

%define api.token.prefix {TOK_}

%token                  END 0           "end of file"
                        EOL             "\n"
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
                        RET             "return"
                        EXT             "extern"
     ;

%token  <std::string> ID
%token  <int> NUMBER

%right "if" "(" ")" "else"
%left "and"
%left "or"
%left "+" "-"
%left "*" "/" "%"
%precedence UNARY

%type   <std::shared_ptr<ast::Exp>> exp
%type   <std::shared_ptr<ast::LVal>> lval
%type   <std::shared_ptr<ast::Cond>> cond
%type   <std::shared_ptr<ast::Stmt>> stmt
%type   <std::shared_ptr<ast::Stmt>> otherstmt
%type   <std::shared_ptr<ast::Block>> block
%type   <std::shared_ptr<ast::Block>> blockitem
%type   <std::shared_ptr<ast::Decl>> decl
%type   <std::shared_ptr<ast::ConstDefs>> constdefs
%type   <std::shared_ptr<ast::ConstDef>> constdef
%type   <std::shared_ptr<ast::Vars>> vars
%type   <std::shared_ptr<ast::Var>> var
%type   <std::shared_ptr<ast::Params>> params
%type   <std::shared_ptr<ast::Param>> param
%type   <std::shared_ptr<ast::Exps>> exps
%type   <std::shared_ptr<ast::CompUnit>> compunit
%type   <std::shared_ptr<ast::FuncDef>> funcdef
%type   <std::shared_ptr<ast::ExtFunc>> extfunc

%%

compunit:       %empty
                {
                    $$ = std::make_shared<ast::CompUnit>();
                    logger.debug << $$->production;
                    driver._ast_root = $$;
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
        |       compunit extfunc
                {
                    $$ = $1;
                    $$->append($2);
                }
        ;

funcdef:        "int" ID "(" params ")" block
                {
                    $$ = std::make_shared<ast::FuncDef>($2, $4, $6);
                    logger.debug << $$->production;
                }
        |       "int" ID "(" ")" block
                {
                    $$ = std::make_shared<ast::FuncDef>(
                        $2, std::make_shared<ast::Params>(), $5);
                    logger.debug << $$->production;
                }
        ;

extfunc:        "extern" "int" ID "(" params ")" ";"
                {
                    $$ = std::make_shared<ast::ExtFunc>($3, $5);
                    logger.debug << $$->production;
                }
        |       "extern" "int" ID "(" ")" ";"
                {
                    $$ = std::make_shared<ast::ExtFunc>(
                        $3, std::make_shared<ast::Params>());
                    logger.debug << $$->production;
                }
        ;

params:         param
                {
                    $$ = std::make_shared<ast::Params>();
                    $$->append($1);
                    logger.debug << $$->production;
                }
        |       params "," param
                {
                    $$ = $1;
                    $$->append($3);
                }
        ;

param:          "int" ID
                {
                    $$ = std::make_shared<ast::Param>($2);
                }
        |       "int" ID "[" "]"
                {
                    $$ = std::make_shared<ast::Param>($2, true);
                }
        ;

stmt:           "if" "(" cond ")" stmt "else" stmt
                {
                    $$ = std::make_shared<ast::IfStmt>($3, $5, $7);
                    logger.debug << $$->production;
                }
        |       "if" "(" cond ")" stmt
                {
                    $$ = std::make_shared<ast::IfStmt>($3, $5);
                    logger.debug << $$->production;
                }
        |       "if" "(" cond stmt "else" stmt
                {
                    error(@2, "unmatched ( in if-statement");
                }
        |       "if" "(" cond stmt
                {
                    error(@2, "unmatched ( in if-statement");
                }
        |       "while" "(" cond ")" stmt
                {
                    $$ = std::make_shared<ast::WhileStmt>($3, $5);
                    logger.debug << $$->production;
                }
        |       "while" "(" cond stmt
                {
                    error(@2, "unmatched ( in while-statement");
                }
        |       "return" exp ";"
                {
                    $$ = std::make_shared<ast::RetStmt>($2);
                    logger.debug << $$->production;
                }
        |       block
                {
                    $$ = $1;
                }
        |       otherstmt
                {
                    $$ = $1;
                }
        ;

otherstmt:      lval "=" exp ";"
                {
                    $$ = std::make_shared<ast::AsgnStmt>($1, $3);
                    logger.debug << $$->production;
                }
        |       ";"
                {
                    $$ = std::make_shared<ast::Stmt>();
                    logger.debug << $$->production;
                }
        |       ID "(" exps ")" ";"
                {
                    $$ = std::make_shared<ast::FuncCall>($1, $3);
                    logger.debug << $$->production;
                }
        |       ID "(" ")" ";"
                {
                    $$ = std::make_shared<ast::FuncCall>(
                        $1, std::make_shared<ast::Exps>());
                    logger.debug << $$->production;
                }
        |       error ";"
                {
                    yyerrok;
                }
        ;

block:          "{" blockitem "}"
                {
                    $$ = $2;
                }
        |       error "}"
                {
                    yyerrok;
                }
        ;

blockitem:      %empty
                {
                    $$ = std::make_shared<ast::Block>();
                    logger.debug << $$->production;
                }
        |       blockitem stmt
                {
                    $$ = $1;
                    $$->append($2);
                }
        |       blockitem decl
                {
                    $$ = $1;
                    $2->setLocal();
                    $$->append($2);
                }
        ;

decl:           "const" "int" constdefs ";"
                {
                    $$ = std::make_shared<ast::Decl>($3);
                    logger.debug << $$->production;
                }
        |       "int" vars ";"
                {
                    $$ = std::make_shared<ast::Decl>($2);
                    logger.debug << $$->production;
                }
        |       "const" constdefs ";"
                {
                    $$ = std::make_shared<ast::Decl>($2);
                    logger.debug << $$->production;
                    driver.warning("type specifier missing, defaults to "
                                   "\'int\'", @2);
                }
        |       "const" error ";"
                {
                    yyerrok;
                }
        |       "int" error ";"
                {
                    yyerrok;
                }
        ;

constdefs:      constdef
                {
                    $$ = std::make_shared<ast::ConstDefs>();
                    logger.debug << $$->production;
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
                    $$ = std::make_shared<ast::ConstDef>($1, $3);
                    logger.debug << $$->production;
                }
        |       ID "[" exp "]" "=" "{" exps "}"
                {
                    $$ = std::make_shared<ast::ConstDef>($1, $3, $7);
                    logger.debug << $$->production;
                }
        ;

vars:           var
                {
                    $$ = std::make_shared<ast::Vars>();
                    logger.debug << $$->production;
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
                    $$ = std::make_shared<ast::Var>($1);
                    logger.debug << $$->production;
                }
        |       ID "[" exp "]"
                {
                    $$ = std::make_shared<ast::Var>($1, $3);
                    logger.debug << $$->production;
                }
        |       ID "=" exp
                {
                    // with initialization
                    $$ = std::make_shared<ast::Var>($1, $3, true);
                    logger.debug << $$->production;
                }
        |       ID "[" exp "]" "=" "{" exps "}"
                {
                    $$ = std::make_shared<ast::Var>($1, $3, $7);
                    logger.debug << $$->production;
                }
        ;

exps:           exp
                {
                    $$ = std::make_shared<ast::Exps>();
                    logger.debug << $$->production;
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
                    $$ = std::make_shared<ast::Number>($1);
                    logger.debug << $$->production;
                }
        |       lval
                {
                    $$ = $1;
                    logger.debug << $$->production;
                }
        |       ID "(" exps ")"
                {
                    $$ = std::make_shared<ast::CallExp>($1, $3);
                    logger.debug << $$->production;
                }
        |       ID "(" ")"
                {
                    $$ = std::make_shared<ast::CallExp>(
                        $1, std::make_shared<ast::Exps>());
                    logger.debug << $$->production;
                }
        |       "-" exp %prec UNARY
                {
                    $$ = std::make_shared<ast::Exp>('N', $2);
                    logger.debug << $$->production;
                }
        |       exp "+" exp
                {
                    $$ = std::make_shared<ast::Exp>('+', $1, $3);
                    logger.debug << $$->production;
                }
        |       exp "-" exp
                {
                    $$ = std::make_shared<ast::Exp>('-', $1, $3);
                    logger.debug << $$->production;
                }
        |       exp "*" exp
                {
                    $$ = std::make_shared<ast::Exp>('*', $1, $3);
                    logger.debug << $$->production;
                }
        |       exp "/" exp
                {
                    $$ = std::make_shared<ast::Exp>('/', $1, $3);
                    logger.debug << $$->production;
                }
        |       exp "%" exp
                {
                    $$ = std::make_shared<ast::Exp>('%', $1, $3);
                    logger.debug << $$->production;
                }
        |       "(" exp ")"
                {
                    $$ = $2;
                    logger.debug << $$->production;
                }
        ;

lval:           ID
                {
                    $$ = std::make_shared<ast::LVal>($1);
                    logger.debug << $$->production;
                }
        |       ID "[" exp "]"
                {
                    $$ = std::make_shared<ast::LVal>($1, $3);
                    logger.debug << $$->production;
                }
        ;

cond:           "odd" exp
                {
                    $$ = std::make_shared<ast::Cond>("odd", $2);
                    logger.debug << $$->production;
                }
        |       "!" cond %prec UNARY
                {
                    $$ = std::make_shared<ast::Cond>($2);
                    logger.debug << $$->production;
                }
        |       "(" cond ")"
                {
                    $$ = $2;
                }
        |       cond "and" cond
                {
                    $$ = std::make_shared<ast::Cond>("and", $1, $3);
                    logger.debug << $$->production;
                }
        |       cond "or" cond
                {
                    $$ = std::make_shared<ast::Cond>("or", $1, $3);
                    logger.debug << $$->production;
                }
        |       exp "==" exp
                {
                    $$ = std::make_shared<ast::Cond>("==", $1, $3);
                    logger.debug << $$->production;
                }
        |       exp "!=" exp
                {
                    $$ = std::make_shared<ast::Cond>("!=", $1, $3);
                    logger.debug << $$->production;
                }
        |       exp "<" exp
                {
                    $$ = std::make_shared<ast::Cond>("<", $1, $3);
                    logger.debug << $$->production;
                }
        |       exp "<=" exp
                {
                    $$ = std::make_shared<ast::Cond>("<=", $1, $3);
                    logger.debug << $$->production;
                }
        |       exp ">" exp
                {
                    $$ = std::make_shared<ast::Cond>(">", $1, $3);
                    logger.debug << $$->production;
                }
        |       exp ">=" exp
                {
                    $$ = std::make_shared<ast::Cond>(">=", $1, $3);
                    logger.debug << $$->production;
                }
        ;
%%
void yy::Parser::error(const location_type &loc, const std::string& m)
{
    driver._ast_root = nullptr;
    driver.error(m, loc);
}
