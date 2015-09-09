#include <iostream>
#include <stdio.h>
#include "common.h"
#include "helpers.h"

using namespace std;

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
            cout << "<NUM, " << yylval << "> ";
            break;
        case ident:
            cout << "<ID, " << yylval << ": "
                 << symbols[yylval] << "> ";
            break;
        case plus:
            cout << "<+> ";
            break;
        case minus:
            cout << "<-> ";
            break;
        case times:
            cout << "<*> ";
            break;
        case divide:
            cout << "</> ";
            break;
        case mod:
            cout << "<%%> ";
            break;
        case eql:
            cout << "<EQ> ";
            break;
        case neq:
            cout << "<NE> ";
            break;
        case lss:
            cout << "<LT> ";
            break;
        case leq:
            cout << "<LE> ";
            break;
        case gtr:
            cout << "<GT> ";
            break;
        case geq:
            cout << "<GE> ";
            break;
        case lparen:
            cout << "<(> ";
            break;
        case rparen:
            cout << "<)> ";
            break;
        case comma:
            cout << "<,> ";
            break;
        case semicolon:
            cout << "<;> ";
            break;
        case assign:
            cout << "<=> ";
            break;
        case lbracket:
            cout << "<[> ";
            break;
        case rbracket:
            cout << "<]> ";
            break;
        case lbrace:
            cout << "<{> ";
            break;
        case rbrace:
            cout << "<}> ";
            break;
        case oddsym:
            cout << "<ODD> ";
            break;
        case constsym:
            cout << "<CONST> ";
            break;
        case intsym:
            cout << "<INT> ";
            break;
        case ifsym:
            cout << "<IF> ";
            break;
        case elsesym:
            cout << "<ELSE> ";
            break;
        case whilesym:
            cout << "<WHILE> ";
            break;
        default:
            WARN("Unknown symbol '%s' at line %d.", yytext, yylineno);
            break;
        }
        getsym();
    }
    cout << "\n";
    
    INFO("Scanning finished.");
    
    fclose(infile);
    
    return 0;
}
