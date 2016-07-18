#ifndef VISITABLE_H
#define VISITABLE_H

// Implementation of the Visitor design pattern

class Visitor;

class Visitable {
public:
    virtual int accept(Visitor &visitor) = 0;
};

#endif // VISITABLE_H
