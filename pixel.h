#ifndef PIXEL_H
#define PIXEL_H

#include "colour.h"

class Pixel
{
public:
    Pixel();
    void set(ColourRGB colour);
    void set(ColourRBGA colour);

    ColourRGB getRGB();

protected:
    ColourRGB colour = {0, 0, 0};
    uint8_t alpha = 1;
};

#endif