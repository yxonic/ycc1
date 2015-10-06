%{
#include "common.h"
#include "helpers.h"

extern YYSTYPE yylval;
YYSTYPE insid();
unsigned long sym;
int nsym;
std::string symbols[256];
std::map<std::string, int> symmap;

%}

delim           [ \t\n]
ws              {delim}+
letter_         [A-Za-z_]
digit           [0-9]
id              {letter_}({letter_}|{digit})*
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
                      ERROR("Invalid number '%s' at line %d.", yytext, yylineno);
                  return number; }
{id}            { yylval = insid(); return ident; }

.               { return nul; }

%%
YYSTYPE insid()
{
    std::string str = std::string(yytext);
    std::map<std::string, int>::iterator i = symmap.find(str);
    if (i == symmap.end()) {
        symbols[nsym] = str;
        symmap[str] = nsym;
        return nsym++;
    } else {
        return i->second;
    }
}
void getsym()
{
    sym = yylex();
}
