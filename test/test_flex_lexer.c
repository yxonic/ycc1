#include <stdio.h>
#include "common.h"
#include "helpers.h"

extern void getsym();

int main(int argc, char **argv)
{
    // skip program name
    --argc; ++argv;

    FILE *infile;
    
    if (argc == 0) {
        ERROR("No input file!");
        return 1;
    }
    
    if ((infile = fopen(argv[0], "r")) == NULL) {
        ERROR("%s: file can't be opened!", argv[0]);
        return 1;
    }

    extern FILE *yyin;
    yyin = infile;
    
    INFO("Started scanning.");
    
    getsym();
    while (sym) {
        switch(sym) {
        case number:
            printf("<NUM, %d> ", yylval);
            break;
        case ident:
            printf("<ID, %s> ", symbols[yylval]);
            break;
        case plus:
            printf("<+> ");
            break;
        case minus:
            printf("<-> ");
            break;
        case times:
            printf("<*> ");
            break;
        case divide:
            printf("</> ");
            break;
        case mod:
            printf("<%%> ");
            break;
        case eql:
            printf("<EQ> ");
            break;
        case neq:
            printf("<NE> ");
            break;
        case lss:
            printf("<LT> ");
            break;
        case leq:
            printf("<LE> ");
            break;
        case gtr:
            printf("<GT> ");
            break;
        case geq:
            printf("<GE> ");
            break;
        case lparen:
            printf("<(> ");
            break;
        case rparen:
            printf("<)> ");
            break;
        case comma:
            printf("<,> ");
            break;
        case semicolon:
            printf("<;> ");
            break;
        case assign:
            printf("<=> ");
            break;
        case lbracket:
            printf("<[> ");
            break;
        case rbracket:
            printf("<]> ");
            break;
        case lbrace:
            printf("<{> ");
            break;
        case rbrace:
            printf("<}> ");
            break;
        case oddsym:
            printf("<ODD> ");
            break;
        case constsym:
            printf("<CONST> ");
            break;
        case intsym:
            printf("<INT> ");
            break;
        case ifsym:
            printf("<IF> ");
            break;
        case elsesym:
            printf("<ELSE> ");
            break;
        case whilesym:
            printf("<WHILE> ");
            break;
        default:
            WARN("Unknown symbol!");
            break;
        }
        getsym();
    }
    printf("\n");

    INFO("Scanning finished.");
    
    fclose(infile);
    
    return 0;
}
