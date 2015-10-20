#include <iostream>
#include "ParsingDriver.h"
#include "Utils.h"

int main(int argc, char **argv)
{
    logger.setLevel(LogManager::Debug);
    if (argc < 2)
        return 1;
    ParsingDriver pd(argv[1]);
    pd.parse();
    return 0;
}
