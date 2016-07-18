#ifndef PLANETARYVISITOR_H
#define PLANETARYVISITOR_H

#include "dialog.h"

// Concrete Visitor Class
class PlanetaryVisitor : public Visitor{
    // This formats the item prices to 2 decimal places
    // This is created so that each item is sent to the
    // right version of visit() which is required by the
    // Visitor interface and defined below
public:
    PlanetaryVisitor() {}

    // Calculates total price based on this being taxed
    // as a liquor item
    int visit(Dialog &dialog) {
        std::cout << "Width of screen" << std::endl;
        return dialog.getWidth();
    }
};

#endif // PLANETARYVISITOR_H
