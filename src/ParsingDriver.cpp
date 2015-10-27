#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ParsingDriver.h"
#include "Utils.h"

using namespace std;

ParsingDriver::ParsingDriver(string s)
    : _file_name(s)
{
    std::ifstream fin(s);
    _in_stream << fin.rdbuf();

    initializeLexer();
    initializeParser();
}

void ParsingDriver::initializeLexer()
{
    _lexer = make_shared<yy::Lexer>(&_in_stream);
}

void ParsingDriver::initializeParser()
{
    // Pass reference to current object to enable the parser to access
    // source and report errors.
    _parser = make_shared<yy::Parser>(*this);
}

void ParsingDriver::parse()
{
    /// \todo Add error handling.
    _parser->parse();
}

void ParsingDriver::error(string err, ErrorLevel level)
{
    error(err, _lexer->loc, level);
}

void ParsingDriver::error(string err, const yy::location &loc,
    ErrorLevel level)
{
    string err_color;
    string err_type;
    switch (level) {
    case Error:
        err_color = "\033[31;1m";
        err_type = "Error";
        break;
    case Warning:
        err_color = "\033[33;1m";
        err_type = "Warning";
        break;
    case Info:
        err_color = "\033[30;1m";
        err_type = "Info";
        break;
    }
    
    // store current line text
    string line;

    // record current position in stream
    auto cur = _in_stream.tellg();
    _in_stream.seekg(_lexer->markers[loc.end.line - 1]);
    getline(_in_stream, line);
    // recover stream
    _in_stream.seekg(cur);

    stringstream err_text;

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    unsigned int w1 = 16, w2 = w.ws_col - w1 - 3;

    int beg_line = loc.begin.line, beg_col = loc.begin.column;
    int end_line = loc.end.line, end_col = loc.end.column;
    if (end_col > 0)
        --end_col;

    string indicator;
    if (beg_line != end_line || beg_col >= end_col)
        indicator = string(end_col, ' ') + "^";
    else
        indicator = string(beg_col, ' ') + " ^" +
            string(end_col - beg_col - 1, '~');

    err_text << fit_text(file_name, w1) << " ! "
             << fit_text(err, w2, true) << "\n"
             << fit_text(to_string(end_line) + ":" +
                         to_string(end_col), w1)
             << " | " << fit_text(line, w2, true) << "\n"
             << err_color << fit_text(err_type, w1)
             << "\033[0m !\033[32;1m"
             << fit_text(indicator, w2 + 1, true) << "\033[0m";

    cerr << err_text.str() << endl;
}

void ParsingDriver::warning(string err)
{
    error(err, _lexer->loc, Warning);
}

void ParsingDriver::warning(string err, const yy::location &loc)
{
    error(err, loc, Warning);
}

void ParsingDriver::info(string err)
{
    error(err, _lexer->loc, Info);
}

void ParsingDriver::info(string err, const yy::location &loc)
{
    error(err, loc, Info);
}

string ParsingDriver::fit_text(string text, size_t width, bool left)
{
    auto l = text.length();
    if (width > l) {
        if (left)
            return text + string(width - l, ' ');
        else
            return string(width - l, ' ') + text;
    } else {
        if (left) {
            string rv(text, 0, width);
            if (width > 3)
                rv[width - 1] = rv[width - 2] = rv[width - 3] = '.';
            return rv;
        } else {
            string rv(text, l - width, width);
            if (width > 2)
                rv[0] = rv[1] = '.';
            return rv;
        }
    }
}
