#pragma once

// Abstract class for ASTs
class AST {
public:
    virtual void dump(ASTDump &) = 0;
};

// ASTs for specific grammar elements
