#ifndef VISITOR_H
#define VISITOR_H

#include <iostream>

class Dialog;

class Visitor {
public:
    virtual int visit(Dialog &dialog) = 0;
};

#endif // VISITOR_H
