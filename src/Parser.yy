%skeleton "lalr1.cc"

%defines
%define parser_class_name {Parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
#include <string>
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

%token
     END 0      "end of file"
     ASSIGN     "="
     PLUS       "+"
     MINUS      "-"
     STAR       "*"
     SLASH      "/"
     LPAREN     "("
     RPAREN     ")"
     ;

%token <std::string> ID "identifier"
%token <int> NUMBER "number"
%type <int> exp

%printer { yyoutput << $$; } <*>
%%
%start unit;
unit:            assignments exp
                { driver.result = $2; };
assignments:    %empty {}
        |       assignments assignment {};
assignment:     "identifier" "=" exp
                { driver.variables[$1] = $3; };
%left "+" "-";
%left "*" "/";
exp:            exp "+" exp
                { $$ = $1 + $3; }
        |       exp "-" exp
                { $$ = $1 - $3; }
        |       exp "*" exp
                { $$ = $1 * $3; }
        |       exp "/" exp
                { $$ = $1 / $3; }
        |       "("exp ")"
                { std::swap($$, $2); }
        |       "identifier"
                { $$ = driver.variables[$1]; }
        |       "number"
                { std::swap($$, $1); }
        ;
%%
void yy::Parser::error(const location_type &l, const std::string& m)
{
}
