#include "ParsingDriver.h"
#include "Utils.h"

int ParsingDriver::parse(const std::string &f)
{
    filename = f;
    scan_begin();
    INFO("Started parsing...");
    yy::Parser parser(*this);
    parser.parse();
    scan_end();
    return 0;
}
