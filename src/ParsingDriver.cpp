#include <iostream>

#include "ParsingDriver.h"
#include "Utils.h"

using namespace std;

ParsingDriver::ParsingDriver(string s)
    : _file_name(s)
{
    _in_stream = make_shared<ifstream>(s);
    initializeLexer();
    initializeParser();
}

void ParsingDriver::initializeLexer()
{
    _lexer = make_shared<yy::Lexer>(_in_stream.get());
}

void ParsingDriver::initializeParser()
{
    // Pass reference to current object to enable the parser to access
    // source and report errors.
    _parser = make_shared<yy::Parser>(*this);
}

void ParsingDriver::parse()
{
    // TODO: Add error handling.
    _parser->parse();
}

void ParsingDriver::error(string err)
{
    cerr << err << endl;
}

void ParsingDriver::warning(string err)
{
    cerr << err << endl;
}
