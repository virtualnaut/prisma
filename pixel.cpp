#include "pixel.h"

Pixel::Pixel()
{
}

void Pixel::set(ColourRGB colour)
{
    this->colour = colour;
}

void Pixel::set(ColourRBGA colour)
{
    this->colour = {colour.red, colour.green, colour.blue};
    alpha = colour.alpha;
}

ColourRGB Pixel::getRGB()
{
    return colour;
}
