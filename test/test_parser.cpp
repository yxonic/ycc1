#include <iostream>
#include "ParsingDriver.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    ParsingDriver pd;
    std::cout << pd.parse(argv[1]) << std::endl;
    return 0;
}
