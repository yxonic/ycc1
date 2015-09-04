/*
 * expr.lex : Scanner for a simple
 *            expression parser.
 */

%{
#include <stdio.h>
#include "common.h"
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
