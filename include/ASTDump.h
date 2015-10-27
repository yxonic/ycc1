// -*- mode: c++ -*-

#pragma once

#include <string>
#include <fstream>
#include <vector>

/// ASTDump - Interface for AST dumpers.
class ASTDump {
public:
    ASTDump() = default;
    virtual int newNode(int num, ...);
    virtual int newNode(std::vector<std::string> list);
    virtual void drawLine(int nsrc, int fsrc, int ndst);
protected:
    std::string _file_name;
    std::shared_ptr<std::ostream> _out_stream;
};

class ASTDumpString : public ASTDump {
public:
    ASTDumpString(std::string f);
};

class ASTDumpDot : public ASTDump {
public:
    ASTDumpDot(std::string f);
private:
    int count;
};
