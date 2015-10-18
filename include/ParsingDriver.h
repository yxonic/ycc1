#pragma once

#include <string>
#include <map>
#include <memory>
#include "Parser.tab.hh"
#include "AST.h"

// Tell flex to declare a proper yylex.
#define YY_DECL                                                         \
    yy::Parser::symbol_type yylex (ParsingDriver& driver)

YY_DECL;
    
class ParsingDriver {
public:
    ParsingDriver() = default;

    std::shared_ptr<ast::AST> root;
    
    // Run the parser on file
    int parse(const std::string &f);
    friend class yy::Parser;
    
private:
    std::string filename;
    std::map<std::string, int> variables;

    void scan_begin();
    void scan_end();
};

