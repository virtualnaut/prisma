#include "pixel.h"

Pixel::Pixel()
{
}

void Pixel::setRGB(ColourRGB colour)
{
    this->colour = colour;
}

ColourRGB Pixel::getRGB()
{
    return colour;
}
