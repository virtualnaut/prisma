#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>
#include <vector>

#include "pixel.h"
#include "virtualStrip.h"
#include "messages.h"

class Matrix
{
public:
    Matrix(unsigned int width, unsigned int height);
    ~Matrix();

    void clearAll();

    void setPixel(unsigned int x, unsigned int y, ColourRGBA colour);
    VirtualPixel *getPixel(unsigned int pixel);
    VirtualPixel *getPixel(unsigned int x, unsigned int y);

    void setHorizontalFlip(bool flipped);
    void setVerticalFlip(bool flipped);

    void setVirtualStrips(MatrixVirtualStripMessage virtualStrips[MAX_VIRTUAL_STRIPS], char count);
    MatrixVirtualStrip *getVirtualStrip(unsigned int strip);

    void setMask(unsigned int strip, uint16_t value);

    std::vector<VirtualPixel *> getPixels();

    /** Set a number of pixels starting at (x,y) and bounded by width and height */
    void setRegion(unsigned int x, unsigned int y, unsigned int width, unsigned int height, ColourRGBA *colours);
    unsigned int flattenCoords(unsigned int x, unsigned int y);

    unsigned int length;

protected:
    std::vector<VirtualPixel *> pixels;
    std::vector<MatrixVirtualStrip *> virtualStrips;

    unsigned int width;
    unsigned int height;
    bool flippedHorizontally = false;
    bool flippedVertically = false;
};

#endif
