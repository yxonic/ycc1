%{
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <string>
#include "ParsingDriver.h"
#include "Parser.tab.hh"

static yy::location loc;
%}
%option noyywrap nounput batch noinput

delim           [ \t]
ws              {delim}+
letter_         [A-Za-z_]
digit           [0-9]
id              {letter_}({letter_}|{digit})*
number          {digit}+|0x({digit}|[a-fA-F])+

%x IN_COMMENT
%x IN_LINECOMMENT

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}
                        
{ws}            { /* throw away whitespace */ }

<INITIAL>{
"/*"            BEGIN(IN_COMMENT);
"//"            BEGIN(IN_LINECOMMENT);
}
<IN_COMMENT>{
"*/"            BEGIN(INITIAL);
[^*]+           // eat comment in chunks
"*"             // eat the lone star
}
<IN_LINECOMMENT>{
"\n"            BEGIN(INITIAL);
[^\n]+          // eat comment
}

[\n]+           loc.lines (yyleng); loc.step();
"+"             return yy::Parser::make_PLUS(loc);
"-"             return yy::Parser::make_MINUS(loc);
"*"             return yy::Parser::make_STAR(loc);
"/"             return yy::Parser::make_SLASH(loc);
"%"             return yy::Parser::make_MOD(loc);
"=="            return yy::Parser::make_EQ(loc);
"!="            return yy::Parser::make_NE(loc);
"<"             return yy::Parser::make_LT(loc);
"<="            return yy::Parser::make_LE(loc);
">"             return yy::Parser::make_GT(loc);
">="            return yy::Parser::make_GE(loc);
"!"             return yy::Parser::make_NOT(loc);
"("             return yy::Parser::make_LPAREN(loc);
")"             return yy::Parser::make_RPAREN(loc);
","             return yy::Parser::make_COMMA(loc);
";"             return yy::Parser::make_SEMI(loc);
"="             return yy::Parser::make_ASSIGN(loc);
"["             return yy::Parser::make_LBRACKET(loc);
"]"             return yy::Parser::make_RBRACKET(loc);
"{"             return yy::Parser::make_LBRACE(loc);
"}"             return yy::Parser::make_RBRACE(loc);

"const"         return yy::Parser::make_CONST(loc);
"int"           return yy::Parser::make_INT(loc);
"void"          return yy::Parser::make_VOID(loc);
"if"            return yy::Parser::make_IF(loc);
"else"          return yy::Parser::make_ELSE(loc);
"while"         return yy::Parser::make_WHILE(loc);
"and"           return yy::Parser::make_AND(loc);
"or"            return yy::Parser::make_OR(loc);
"odd"           return yy::Parser::make_ODD(loc);

{number}        { char *endptr;
                  long n = strtol(yytext, &endptr, 0);
                  return yy::Parser::make_NUMBER(n, loc); }
{id}            return yy::Parser::make_ID(yytext, loc);
.
<<EOF>>         return yy::Parser::make_END(loc);
%%
void ParsingDriver::scan_begin()
{
    if (filename.empty() || filename == "-")
        yyin = stdin;
    else if (!(yyin = fopen(filename.c_str (), "r")))
    {
        exit (1);
    }
}
void ParsingDriver::scan_end()
{
    fclose(yyin);
}
