#include "ParsingDriver.h"
#include "Utils.h"

using namespace std;

void ParsingDriver::initializeLexer()
{
    lexer = std::make_shared<yy::Lexer>(fs);
}

int ParsingDriver::parse()
{
    initializeLexer();
    yy::Parser parser(*this);
    parser.parse();
    return 0;
}
