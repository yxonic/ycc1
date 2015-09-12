Understanding clang
===================

Understand how `clang` work might be really helpful for learning
compilers. And we do this by reading the code. However, we may have a
look at how to use it first.

# Usage

>`clang` is a C, C++, and Objective-C compiler which encompasses
>preprocessing, parsing, optimization, code generation, assembly, and
>linking.
>
> *clang MAN page*

Let's use C for example. Though we can compile a C program by just one
line, actually there are at least five steps: preprocessing (turn ".c"
into ".i"), parsing and semantic analysis (generate an AST), code
generation and optimization (turn AST into ".s" or assembly file),
assembling (turn into ".o" or object file), and linking (to generate a
program, or a shared object).

We can use `clang` with different options to run specific phase only.

    -E     Run the preprocessor stage.

    -fsyntax-only
           Run the preprocessor, parser and type checking stages.

    -S     Run the previous stages as well as LLVM generation and
           optimization stages and target-specific code generation,
           producing an assembly file.

    -c     Run all of the above, plus the assembler, generating a
           target ".o" object file.

    no stage selection option
           If no stage selection option is specified, all stages
           above are run, and the linker is run to combine the results
           into an executable or shared library.
