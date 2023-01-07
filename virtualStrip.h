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

enum class MaskMode
{
    Fill = 0,     // Mask defines a pixel. Show all pixels between the start and this pixel.
    Dot,          // Mask defines a pixel which is the only pixel that should be shown.
    FillReverse,  // Mask defines a pixel. Show all pixels from this pixel to the end.
    Random,       // Mask defines a probability for each pixel to be masked.
    Transparency, // Mask defines a factor to apply to the transparency.
    Brightness,   // Mask defines a factor to apply to the brightness.
    // HueRotation // Mask defines an amount round the colour wheel to rotate the colour of each pixel.
};

class VirtualStrip
{
public:
    VirtualStrip(bool isFractional, MaskMode mode);

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
    MaskMode mode;
};

class LinearVirtualStrip : public virtual VirtualStrip
{
public:
    LinearVirtualStrip(unsigned int start, unsigned int end, bool isFractional, MaskMode mode);
    unsigned int length();
    unsigned int start;
    unsigned int end;

protected:
};

class MatrixVirtualStrip : public virtual VirtualStrip
{
public:
    MatrixVirtualStrip(
        unsigned int x,
        unsigned int y,
        unsigned int length,
        unsigned int thickness,
        bool isHorizontal,
        bool isPositive,
        bool isFractional,
        MaskMode mode);
    ~MatrixVirtualStrip();
    unsigned int length();
    unsigned int count();
    void getComponents(MatrixStripComponent *components);

protected:
    unsigned int x;
    unsigned int y;
    unsigned int stripLength;
    unsigned int thickness;

    bool isHorizontal;
    bool isPositive;
};

#endif