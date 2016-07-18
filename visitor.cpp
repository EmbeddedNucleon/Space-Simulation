#include "Visitor.h"

class Dialog;

int Visitor::visit(Dialog &dialog) {
    std::cout << "Width of screen" << std::endl;
    return dialog.getWidth();
}
