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

#endif