// -*- mode: c++ -*-
#pragma once

// Define YY_DECL to let flex generate a proper lexing function.
#ifndef YY_DECL
#define	YY_DECL                                                         \
    yy::Parser::symbol_type yy::Lexer::lex(Driver &driver)
#endif

// Base classes: FlexLexer and BaseFlexLexer
#ifndef __FLEX_LEXER_H
#define yyFlexLexer BaseFlexLexer
#include <FlexLexer.h>
#endif

#include <vector>
#include "Parser.tab.hh"

namespace yy {

    /// Lexer - The lexer class for C1, derived from the
    /// auto-generated BaseFlexLexer.
    class Lexer : public BaseFlexLexer
    {
    public:
        Lexer(std::istream *in = 0, std::ostream *out = 0)
            : BaseFlexLexer(in, out), markers { in->tellg() } { }
        virtual ~Lexer() { }
        virtual Parser::symbol_type lex(Driver &);
        yy::location loc;
        std::vector<std::stringstream::pos_type> markers;
    };
}
