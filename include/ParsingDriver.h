#ifndef PARSING_DRIVER_H
#define PARSING_DRIVER_H

#include <string>
#include <map>
#include "Parser.tab.hh"

// Tell flex to declare a proper yylex.
#define YY_DECL                                                         \
    yy::Parser::symbol_type yylex (ParsingDriver& driver)

YY_DECL;
    
class ParsingDriver {
public:
    ParsingDriver();
    virtual ~ParsingDriver();

    int result;
    // Run the parser on file
    int parse(const std::string &f);
    friend class yy::Parser;
private:
    std::string filename;
    std::map<std::string, int> variables;

    void scan_begin();
    void scan_end();
};

#endif