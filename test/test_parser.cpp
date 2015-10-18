#include <iostream>
#include "ParsingDriver.h"
#include "Utils.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    ParsingDriver pd;
    pd.parse(argv[1]);
    return 0;
}
