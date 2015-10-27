1. Lexical Analyzer
===================

## Basic Ideas

> As the first phase of a compiler, the main task of the lexical
> analyzer is to read the input characters of the source program,
> group them into lexemes, and produce as output a sequence of tokens
> for each lexeme in the source program. ---Compilers: Principles,
> Techniques and Tools

Here are some basic terms in lexical analysis to know first: *tokens*,
*patterns*, and *lexemes*. A *token* is a pair consisting of a token
name and an optional attribute value, which is later used by the
parser. A *pattern* describes the form that the lexemes of a token may
take. And a *lexeme* is the underlying string corresponding to a
token.

Obviously, we need to formally define patterns and automatically
generate tokens according to these patterns. We use regular
expressions and a program called `flex` to finish this job.

Regular expressions can match strings, or, to be formal, denotes a
specific *language*. Regular expressions can be expressed using
induction rules: if `r` is a regular expression, so is `(r)`,
`(r)|(s)`, `(r)(s)`, and `(r)*`. Also, we can name a regular
expression, and there are some other rules for convenience.

To illustrate, here is an (a little bit complex) example:

    digit -> [0-9]                      // a regular definition
    letter -> [A-Za-z]
    id -> {letter}({letter}|{digit})*   // matches a typical C identifier

We can easily use regular expressions to define rules in `flex`. On
more detailed usage of `flex`, refer to section *Using flex*.

Besides, we also want to know how computers actually match strings
according to regular expressions. And the solution is to convert
regular expressions to finite automatons, which is fully described in
the textbook.

## Using `flex`

A `flex` "program" consists of three parts, seperated by `%%`, which
are: declarations, translation rules, and auxiliary functions.

Inside declaration part, we can place C code to be at the top of the
generated source file (such as `#define`s or `#include`s), as well as
regular definitions. Next, we can follow the form: `regex
{operations}` to describe translation rules. Here, C code such as
`printf` is evaluated once certain pattern get matched, and `return`
statement returns corresponding predefined token. If a token has a
value with it, it should be stored in `yylval` before return. Here we
can also call user defined functions. These functions should either be
defined in some of the header files included before, or defined
directly afterwards. A symbol table needed by the parser can be
implemented and used in such manner. Following such convension, we can
call `yylex()` to get tokens, use `yylval` to check corresponding
value, and connect the generated lexer to the parser later on.

Pay attention to the order of the rules. For example, keywords should
be placed before identifiers. We can see that keywords, if not
specifying a certain meaning, also matches the rules for
identifiers. However, we want to match, for instance, `if`, as a
keyword instead of an identifier. Therefore, we should remember that
order of the rules matters.

Many rules can be expressed by regular expressions, as shown above.
However, not all. Tricky things happen when we want to describe rules
defining C-style comments.

At first we might be tempted to try something like this:

     "/*".*"*/"       // WRONG!

or, worse, this:

     "/*"(.|\n)"*/"   // WRONG!

The above rules will eat too much input, and blow up on things like:

     /* a comment */ do_my_thing( "oops */" );

Here is one way which allows you to track line information:

     <INITIAL>{
     "/*"              BEGIN(IN_COMMENT);
     }
     <IN_COMMENT>{
     "*/"      BEGIN(INITIAL);
     [^*\n]+   // eat comment in chunks
     "*"       // eat the lone star
     \n        yylineno++;
     }

Though lexical analysis is performed before parsing, in many
tutorials, the order is different, where parser written in `bison` is
taught first, then `flex`. That's because what the lexer do is merely
providing tokens to the parser, therefore we need to know in what ways
the parser will ask for tokens. By now, we can't write a lexer for
later use, but can only write rules and see whether it correctly lexed
the tokens. More details will be described in the next section about
parser.
