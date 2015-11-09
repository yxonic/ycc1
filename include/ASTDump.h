// -*- mode: c++ -*-

#pragma once

#include <string>
#include <iostream>
#include <vector>

/// ASTDump - Interface for AST dumpers.
class ASTDump {
public:
    virtual int newNode(std::string) = 0;
    virtual void drawLine(int, int) = 0;
    virtual void end() = 0;
};

class ASTDumpDot : public ASTDump {
public:
    ASTDumpDot(std::ostream &os) : _out_stream(os) {
        _out_stream << "digraph { node [shape = box];" << std::endl;
    }
    virtual int newNode(std::string);
    virtual void drawLine(int, int);
    virtual void end();
private:
    int count = 1;
    std::ostream &_out_stream;
};
