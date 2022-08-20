#include "virtualStrip.h"

VirtualStrip::VirtualStrip(unsigned int start, unsigned int end, bool isFractional)
{
    this->start = start;
    this->end = end;
    this->isFractional = isFractional;

    for (unsigned int pixel = 0; pixel < this->length(); pixel++)
    {
        pixels.push_back(new VirtualPixel());
    }
}

unsigned int VirtualStrip::length()
{
    return abs((int)end - (int)start) + 1;
}

void VirtualStrip::setMask(uint16_t value)
{
    uint16_t mask = isFractional
                        ? (uint16_t)(((uint16_t)this->length()) * ((double)value / 65535))
                        : value;

    unsigned int length = this->length();
    for (unsigned int pixel = 0; pixel < length; pixel++)
    {
        getPixel(pixel)->mask(pixel >= mask);
    }
}

void VirtualStrip::setPixelColour(unsigned int pixel, ColourRGB colour)
{
    pixels.at(pixel)->set(colour);
}

void VirtualStrip::setPixelColour(unsigned int pixel, ColourRGBA colour)
{
    pixels.at(pixel)->set(colour);
}

VirtualPixel *VirtualStrip::getPixel(unsigned int pixel)
{
    return pixels[pixel];
}

void VirtualStrip::setAll(ColourRGB colour)
{
    for (unsigned int pixel = 0; pixel < pixels.size(); pixel++)
    {
        setPixelColour(pixel, colour);
    }
}

void VirtualStrip::setAll(ColourRGBA colour)
{
    for (unsigned int pixel = 0; pixel < pixels.size(); pixel++)
    {
        setPixelColour(pixel, colour);
    }
}