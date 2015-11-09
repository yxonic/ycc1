#include <fstream>
#include <string>
#include "ParsingDriver.h"
#include "Utils.h"
#include "ASTDump.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    ParsingDriver pd(argv[1]);
    pd.parse();
    ASTDumpDot dumper(cout);
    pd.ast_root->dump(dumper);
    dumper.end();
    return 0;
}
