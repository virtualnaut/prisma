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
    std::vector<Pixel *> render();

    unsigned int length();

    void setPixelColour(unsigned int pixel, ColourRGB colour);
    void setPixelColour(unsigned int pixel, ColourRBGA colour);

    void setAll(ColourRGB colour);

    Pixel *getPixel(unsigned int pixel);

    bool isFractional;
    unsigned int start;
    unsigned int end;

protected:
    std::vector<Pixel *> pixels;
};

#endif