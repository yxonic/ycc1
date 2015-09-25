%{
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <string>
#include "ParsingDriver.h"
#include "Parser.tab.hh"

static yy::location loc;
%}
%option noyywrap nounput batch debug noinput

delim           [ \t\n]
ws              {delim}+
letter_         [A-Za-z_]
digit           [0-9]
id              {letter_}({letter_}|{digit})*
number          {digit}+|0x({digit}|[a-fA-F])+

%x IN_COMMENT
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
}
<IN_COMMENT>{
"*/"            BEGIN(INITIAL);
[^*\n]+         // eat comment in chunks
"*"             // eat the lone star
\n              // TODO: yylineno
}

"+"             { return yy::Parser::make_PLUS(loc); }
"-"             { return yy::Parser::make_MINUS(loc); }
"*"             { return yy::Parser::make_STAR(loc); }
"/"             { return yy::Parser::make_SLASH(loc); }
"%"             { }
"=="            { }
"!="            { }
"<"             { }
"<="            { }
">"             { }
">="            { }
"("             { return yy::Parser::make_LPAREN(loc); }
")"             { return yy::Parser::make_RPAREN(loc); }
","             { }
";"             { }
"="             { return yy::Parser::make_ASSIGN(loc); }
"["             { }
"]"             { }
"{"             { }
"}"             { }

"const"         { }
"int"           { }
"if"            { }
"else"          { }
"while"         { }

{number}        { char *endptr;
                  long n = strtol(yytext, &endptr, 0);
                  return yy::Parser::make_NUMBER(n, loc); }
{id}            { return yy::Parser::make_ID(yytext, loc); }

.               { }
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
