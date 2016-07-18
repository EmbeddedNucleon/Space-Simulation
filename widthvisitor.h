#ifndef WIDTHVISITOR_H
#define WIDTHVISITOR_H

#include "dialog.h"

// Concrete Visitor Class
class WidthVisitor : public Visitor{
public:
    WidthVisitor(int widthValue)
        :m_widthValue(widthValue){}

    int visit(Dialog &dialog) {
        return dialog.getZoomWidth(m_widthValue);
    }

private:
    int m_widthValue;
};

// Concrete Visitor Class
class HeightVisitor : public Visitor{
public:
    HeightVisitor(int heightValue)
        :m_heightValue(heightValue){}

    int visit(Dialog &dialog) {
        return dialog.getZoomHeight(m_heightValue);
    }

private:
    int m_heightValue;
};

#endif // WIDTHVISITOR_H
