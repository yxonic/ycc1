#include <stdio.h>
#include "common.h"
#include "helpers.h"

YYSTYPE yylval;
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
        case plus:
            printf("<PLUS> ");
            break;
        }
        getsym();
    }

    INFO("Scanning finished.");
    
    fclose(infile);
    
    return 0;
}
