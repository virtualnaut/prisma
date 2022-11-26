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

void VirtualStrip::applyColourRange(unsigned int start, unsigned int end, ColourRGBA colour)
{
    for (unsigned int pixel = start; pixel <= end; pixel++)
    {
        getPixel(pixel)->set(colour);
    }
}

void VirtualStrip::applyBlendRange(unsigned int start, unsigned int end, ColourRGBA startColour, ColourRGBA endColour)
{
    double redInterval = (endColour.red - (int)startColour.red) / (double)(end - start);
    double greenInterval = (endColour.green - (int)startColour.green) / (double)(end - start);
    double blueInterval = (endColour.blue - (int)startColour.blue) / (double)(end - start);
    double alphaInterval = (endColour.alpha - (int)startColour.alpha) / (double)(end - start);

    for (unsigned int pixel = start; pixel <= end; pixel++)
    {
        getPixel(pixel)->set(ColourRGBA{
            (uint8_t)(startColour.red + redInterval * pixel),
            (uint8_t)(startColour.green + greenInterval * pixel),
            (uint8_t)(startColour.blue + blueInterval * pixel),
            (float)(startColour.alpha + alphaInterval * pixel),
        });
    }
}