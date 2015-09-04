ycc1: A Simple Compiler for the C1 Programming Language
=====

This project is for the course: Compilers (Fall, 2015). It implements
a rather plain compiler for a made-up language C1, which is a tiny
subset of the C programming language.

##Progress

This project just started and is far from finished. You can track
progress here.

<table>
<tr><td><b>Component</b></td><td><b>Status</b></td></tr>
<tr><td>Scanning</td><td>Coding</td></tr>
<tr><td>Parsing</td><td>N/A</td></tr>
<tr><td>Type Checking</td><td>N/A</td></tr>
<tr><td>Code Generation</td><td>N/A</td></tr>
</table>

##Build and test

###Tools

GCC, GNU Make, flex, bison are needed to build the whole project.

###Build the compiler (currently not finished)

The only thing to do is:

    make

Then you can find an executable at `bin/` named `ycc1`.

###Usage

ycc1 follows GCC's convention. You can use it as follows:

```
usage: ycc1 [-h] [-d] [-o file] source

positional arguments:
  source             source file to compile

optional arguments:
  -h, --help         show this help message and exit
  -d, --debug        print debug messages
  -o file            target path for the compiled code
```

Feel free to torture this compiler with all kinds of odd input. Bug
reports are welcome.

Note: before all the phases are completed, there WON'T be a compiler
at all. Please wait :)

###Perform unit tests

I've written some tests on each phase, and those tests can be
performed by typing:

    make test

And you can find test inputs inside `test/examples`. Random/artificial
inputs can be generated using tools from `scripts/`.

When debugging, showing more debugging information can be helpful. You
can enable this by defining `DEBUG` variable.

    make clean
    DEBUG=DEBUG make test

##Documentation

Implementation details are described in the documentation under `doc/`
directory. There are also sufficient comments in the source
code. However, if you find any point that is not clear enough, please
tell me and I will try to write more about it.
