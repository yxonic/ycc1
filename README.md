ycc1: A Simple Compiler for the C1 Programming Language
=====
@mainpage

This project is for the course: Compilers (Fall, 2015). It implements
a rather plain compiler for a made-up language C1, which is a tiny
subset of the C programming language.

## Progress

This project just started and is far from finished. You can track
progress here.

|Component      |Status  |
|---------------|-------:|
|Scanning       |Finished|
|Parsing        |Coding  |
|Type Checking  |N/A     |
|Code Generation|N/A     |

## Build and test

### Tools

GCC, GNU Make, flex, bison are needed to build the whole project.

### Build the compiler (currently not finished)

The only thing to do is:

    make

Then you can find an executable at `bin/` named `ycc1`.

### Usage

ycc1 follows GCC's convention. You can use it as follows:

    usage: ycc1 [-h] [-d] [-o file] source

    positional arguments:
      source             source file to compile

    optional arguments:
      -h, --help         show this help message and exit
      -d, --debug        print debug messages
      -o file            target path for the compiled code

Feel free to torture this compiler with all kinds of odd input. Bug
reports are welcome.

Note: before all the phases are completed, there WON'T be a compiler
at all. Please wait :)

### Perform unit tests

I've written some tests on each phase, and those tests can be
performed by typing:

    make test

And you can find test inputs inside `test/examples`. Random/artificial
inputs can be generated using tools from `scripts/`.

When debugging, showing more debugging information can be
helpful. This feature is turned on by default. You can define RELEASE
variable to disable it.

    make clean
    RELEASE=1 make test

## Documentation

Implementation details are described in the documentation under `doc/`
directory. You can also generate full document using doxygen, by just
typing `make doc`. There are also sufficient comments in the source
code. However, if you find any point that is not clear enough, please
tell me and I will try to write more about it.

