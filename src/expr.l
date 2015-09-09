/*
 * expr.lex : Scanner for a simple
 *            expression parser.
 */

%{
#include <stdio.h>
#include "common.h"
extern YYSTYPE yylval;
unsigned long sym;
int nsym;
std::string symbols[256];
std::map<std::string, int> symmap;
%}

%%

[0-9]+     { yylval = atoi(yytext);
             return(number);
           }
"+"        { return(plus); }
[\n]	   { printf("\n"); }	
[\t ]*     /* throw away whitespace */
.          { printf("Illegal character"); 
             return(nul);
           }
%%
void getsym()
{
    sym = yylex();
}
