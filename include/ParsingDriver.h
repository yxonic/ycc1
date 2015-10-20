#pragma once

#include <string>
#include <fstream>
#include <map>
#include <memory>

#include "AST.h"
#include "Lexer.h"
#include "Parser.tab.hh"        // generated by bison
    
class ParsingDriver {
public:
    ParsingDriver(std::string s) : filename(s) {
        fs = new std::ifstream(s);
    }
    std::shared_ptr<ast::AST> root;
    // Run the parser on file
    int parse();
    friend class yy::Parser;
    friend class yy::Lexer;
    
private:
    void initializeLexer();
    std::string filename;
    std::ifstream *fs;
    std::shared_ptr<yy::Lexer> lexer;
};
