#ifndef COLOUR_H
#define COLOUR_H

#include <stdint.h>

typedef struct ColourRGBA
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    float alpha;
};

typedef struct ColourRGB
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

namespace colour
{
    uint8_t clamp(float value);
    float clampAlpha(float value);

    ColourRGB dim(ColourRGB colour, float factor);
    ColourRGBA dim(ColourRGBA colour, float factor);

    ColourRGBA transparify(ColourRGBA colour, float factor);
}

#endif