#include "ASTDump.h"

using namespace std;

int ASTDumpDot::newNode(std::string name) {
    _out_stream << count << "[label = \"" << name
                 << "\"];" << endl;
    return count++;
}

void ASTDumpDot::drawLine(int s, int t) {
    _out_stream << s << " -> " << t << ";" << endl;
}

void ASTDumpDot::end() {
    _out_stream << "}" << endl;
}
