#include "Utils.h"

using namespace std;

LogManager logger;

const std::string LogManager::_level_text[] = { "\033[35mDEBUG: ",
                                                "\033[36mINFO : ",
                                                "\033[33mWARN : ",
                                                "\033[31mERROR: " };
const std::string LogManager::_color_reset { "\033[0m" };


LogManager::LogManager(Level level, ostream &os)
{
    setLevel(level);
    setStream(os);
}

void LogManager::setStream(ostream &os)
{
    _output_stream = &os;
    debug._output_stream = _output_stream;
    info._output_stream = _output_stream;
    warning._output_stream = _output_stream;
    error._output_stream = _output_stream;
}

void LogManager::setLevel(Level level)
{
    _level = level;
    debug._level = level;
    info._level = level;
    warning._level = level;
    error._level = level;
}
