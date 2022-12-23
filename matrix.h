#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>
#include <vector>

#include "pixel.h"

class Matrix
{
public:
    Matrix(unsigned int width, unsigned int height);

    void setPixel(unsigned int x, unsigned int y, ColourRGBA colour);
    VirtualPixel *getPixel(unsigned int pixel);

    unsigned int length;

protected:
    unsigned int flattenCoords(unsigned int x, unsigned int y);

    std::vector<VirtualPixel *> pixels;

    unsigned int width;
    unsigned int height;
};

#endif
