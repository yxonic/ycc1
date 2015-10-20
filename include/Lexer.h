#pragma once

#ifndef YY_DECL

#define	YY_DECL                                                         \
    yy::Parser::symbol_type yy::Lexer::lex(ParsingDriver &driver)

#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer BaseFlexLexer
#include <FlexLexer.h>
#endif

#include "Parser.tab.hh"

namespace yy {

    class Lexer : public BaseFlexLexer
    {
    public:
        Lexer(std::istream *in = 0, std::ostream *out = 0)
            : BaseFlexLexer(in, out) { }
        virtual ~Lexer() { }

        virtual Parser::symbol_type lex(ParsingDriver &);

    };

}
