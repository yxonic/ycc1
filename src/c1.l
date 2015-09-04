%{
#include "common.h"
%}

delim           [ \t\n]
ws              {delim}+
letter          [A-Za-z]
digit           [0-9]
id              {letter}({letter}|{digit})*
number          {digit}+

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

{number}        { yylval = atoi(yytext); return number; }
{id}            { yylval = insid(); return ident; }

.               { return nul; }

%%
int insid()
{
    strcpy(symbols[nsym], yytext);
    return nsym++;
}

void getsym()
{
    sym = yylex();
}
