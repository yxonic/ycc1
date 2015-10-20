#include "Utils.h"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    cerr << "Under level: Debug" << endl;
    logger.setLevel(LogManager::Debug);

    logger.debug << "debug message";
    logger.info << "info message";
    logger.warning << "warning message";
    logger.error << "error message";

    cerr << "Under level: Info" << endl;
    logger.setLevel(LogManager::Info);

    logger.debug << "debug message";
    logger.info << "info message";
    logger.warning << "warning message";
    logger.error << "error message";

    cerr << "Under level: Warning" << endl;
    logger.setLevel(LogManager::Warning);

    logger.debug << "debug message";
    logger.info << "info message";
    logger.warning << "warning message";
    logger.error << "error message";

    cerr << "Under level: Error" << endl;
    logger.setLevel(LogManager::Error);

    logger.debug << "debug message";
    logger.info << "info message";
    logger.warning << "warning message";
    logger.error << "error message";

    ofstream log_file("test.log");
    logger.setStream(log_file);
    logger.setLevel(LogManager::Debug);

    logger.debug << "debug message";
    logger.info << "info message";
    logger.warning << "warning message";
    logger.error << "error message";

    return 0;
}
