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
    VirtualPixel *getPixel(unsigned int x, unsigned int y);

    void setHorizontalFlip(bool flipped);
    void setVerticalFlip(bool flipped);

    /** Set a number of pixels starting at (x,y) and bounded by width and height */
    void setRegion(unsigned int x, unsigned int y, unsigned int width, unsigned int height, ColourRGBA *colours);
    unsigned int flattenCoords(unsigned int x, unsigned int y);

    unsigned int length;

protected:
    std::vector<VirtualPixel *> pixels;

    unsigned int width;
    unsigned int height;
    bool flippedHorizontally = false;
    bool flippedVertically = false;
};

#endif
