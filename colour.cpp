#include "colour.h"

#include "Arduino.h"

uint8_t colour::clamp(float value)
{
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

float colour::clampAlpha(float value)
{
    if (value < 0)
        return 0;
    if (value > 1)
        return 1;
    return value;
}

ColourRGB colour::dim(ColourRGB colour, float factor)
{
    return ColourRGB{
        clamp(colour.red * factor),
        clamp(colour.green * factor),
        clamp(colour.blue * factor)};
}

ColourRGBA colour::dim(ColourRGBA colour, float factor)
{
    return ColourRGBA{
        clamp((float)colour.red * factor),
        clamp((float)colour.green * factor),
        clamp((float)colour.blue * factor),
        colour.alpha};
}

ColourRGBA colour::transparify(ColourRGBA colour, float factor)
{
    return ColourRGBA{
        colour.red,
        colour.green,
        colour.blue,
        colour.alpha * factor};
}
