Notes on Kaleidoscope compiler
==============================

Kaleidoscope is too simple a language to have a detailed description
here. Though by the time this compiler finishes, there will be much
more features, we don't need to care about that now.

We just go through the code, in the order same as the tutorial.

# The Lexer

Lexer turn strings to tokens. First, we must know what tokens should
it generate. Here, an enum is used to decide the type, and string
`IdentifierStr` and double `NumVal` is for additional information. For
trivial tokens that should have the same name as its ASCII code, it
just use its ASCII code for simplicity, which may not be so good, for
it's difficult to scan two-character operators.

The main scanning function, `gettok()`, as its name suggests, gets
tokens, incrementally, from stdin. It skips whitespaces. Then try to
get an identifier when encountering a letter, and try to get a number
when encountering a digit or a period. When a `#` occurs, it skips the
whole line after. EOF suggests a `tok_eof`. And in other situations,
it simply returns that character.
