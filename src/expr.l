/*
 * expr.lex : Scanner for a simple
 *            expression parser.
 */

%{
#include <stdio.h>
#include "common.h"
extern void getsym();    
%}

%%

[0-9]+     { num = atol(yytext);
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
