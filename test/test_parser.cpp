#include <iostream>
#include <string>
#include "Driver.h"
#include "Utils.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    if (argc > 2) {
        if (string(argv[1]) == "-d") {
            logger.setLevel(LogManager::Debug);
            --argc; ++argv;
        }
    }
    Driver pd(argv[1]);
    pd.parse();
    return 0;
}
