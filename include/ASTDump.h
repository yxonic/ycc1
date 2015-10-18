#pragma once

#include <string>
#include <fstream>
#include <vector>

class ASTDump {
public:
    ASTDump() = default;
    ASTDump(std::string f) : fname(f), fout(f) { }
    virtual int newNode(int num, ...);
    virtual int newNode(std::vector<std::string> list);
    virtual void drawLine(int nsrc, int fsrc, int ndst);
protected:
    std::string fname;
    std::ofstream fout;
};

class ASTDumpString : public ASTDump {
public:
    ASTDumpString(std::string f);
};

class ASTDumpDot : public ASTDump {
public:
    ASTDumpDot(std::string f) {
        fout << "digraph {\n";
        fout << "    node [shape = recode];\n";
        count = 0;
    }
  
private:
    int count;
};
