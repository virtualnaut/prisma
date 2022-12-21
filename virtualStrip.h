#ifndef VIRTUAL_STRIP_H
#define VIRTUAL_STRIP_H

#include <vector>
#include <cstdlib>
#include <Arduino.h>

#include "colour.h"
#include "pixel.h"

class VirtualStrip
{
public:
    VirtualStrip(unsigned int start, unsigned int end, bool isFractional);
    std::vector<VirtualPixel *> render();

    unsigned int length();

    void setMask(uint16_t value);

    void setPixelColour(unsigned int pixel, ColourRGB colour);
    void setPixelColour(unsigned int pixel, ColourRGBA colour);

    void setAll(ColourRGB colour);
    void setAll(ColourRGBA colour);

    VirtualPixel *getPixel(unsigned int pixel);

    void applyColourRange(unsigned int start, unsigned int end, ColourRGBA colour);
    void applyBlendRange(unsigned int start, unsigned int end, ColourRGBA colourStart, ColourRGBA colourEnd);

    bool isFractional;
    unsigned int start;
    unsigned int end;

protected:
    std::vector<VirtualPixel *> pixels;
};

#endif