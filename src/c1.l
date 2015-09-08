%{
#include "common.h"
#include "helpers.h"
#include <stdlib.h>
extern YYSTYPE yylval;
%}

delim           [ \t\n]
ws              {delim}+
letter          [A-Za-z]
digit           [0-9]
id              {letter}({letter}|{digit})*
number          {digit}+|0x({digit}|[a-fA-F])+

%x IN_COMMENT
%%

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

"+"             { return plus; }
"-"             { return minus; }
"*"             { return times; }
"/"             { return divide; }
"%"             { return mod; }
"=="            { return eql; }
"!="            { return neq; }
"<"             { return lss; }
"<="            { return leq; }
">"             { return gtr; }
">="            { return geq; }
"("             { return lparen; }
")"             { return rparen; }
","             { return comma; }
";"             { return semicolon; }
"="             { return assign; }
"["             { return lbracket; }
"]"             { return rbracket; }
"{"             { return lbrace; }
"}"             { return rbrace; }

"odd"           { return oddsym; }
"const"         { return constsym; }
"int"           { return intsym; }
"if"            { return ifsym; }
"else"          { return elsesym; }
"while"         { return whilesym; }

{number}        { char *endptr ;
                  yylval = (YYSTYPE) strtol(yytext, &endptr, 0);
                  if (*endptr)
                      ERROR("Invalid number: %s", yytext);
                  return number; }
{id}            { yylval = insid(); return ident; }

.               { return nul; }

%%
YYSTYPE insid()
{
    strcpy(symbols[nsym], yytext);
    return nsym++;
}
void getsym()
{
    sym = yylex();
}
