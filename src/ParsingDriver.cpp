#include "ParsingDriver.h"

ParsingDriver::ParsingDriver()
{
}

ParsingDriver::~ParsingDriver()
{
}

int ParsingDriver::parse(const std::string &f)
{
    filename = f;
    scan_begin();
    yy::Parser parser(*this);
    parser.parse();
    scan_end();
    return 0;
}
