#ifndef VIRTUAL_STRIP_H
#define VIRTUAL_STRIP_H

#include <vector>
#include <cstdlib>
#include <Arduino.h>

#include "colour.h"
#include "pixel.h"

typedef struct MatrixStripComponent
{
    unsigned int x;
    unsigned int y;
    VirtualPixel *pixel;
} MatrixStripComponent;

class VirtualStrip
{
public:
    VirtualStrip(bool isFractional);

    virtual unsigned int length() = 0;

    void setMask(uint16_t value);

    void setPixelColour(unsigned int pixel, ColourRGB colour);
    void setPixelColour(unsigned int pixel, ColourRGBA colour);

    void setAll(ColourRGB colour);
    void setAll(ColourRGBA colour);

    VirtualPixel *getPixel(unsigned int pixel);

    void applyColourRange(unsigned int start, unsigned int end, ColourRGBA colour);
    void applyBlendRange(unsigned int start, unsigned int end, ColourRGBA colourStart, ColourRGBA colourEnd);

    bool isFractional;

protected:
    std::vector<VirtualPixel *> pixels;
};

class LinearVirtualStrip : public virtual VirtualStrip
{
public:
    LinearVirtualStrip(unsigned int start, unsigned int end, bool isFractional);
    unsigned int length();
    unsigned int start;
    unsigned int end;

protected:
};

class MatrixVirtualStrip : public virtual VirtualStrip
{
public:
    MatrixVirtualStrip(unsigned int x, unsigned int y, unsigned int length, bool isHorizontal, bool isPositive, bool isFractional);
    ~MatrixVirtualStrip();
    unsigned int length();
    void getComponents(MatrixStripComponent *components);

protected:
    unsigned int x;
    unsigned int y;
    unsigned int stripLength;

    bool isHorizontal;
    bool isPositive;
};

#endif