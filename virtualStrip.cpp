#include "virtualStrip.h"

VirtualStrip::VirtualStrip(bool isFractional)
{
    this->isFractional = isFractional;
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

LinearVirtualStrip::LinearVirtualStrip(unsigned int start, unsigned int end, bool isFractional) : VirtualStrip(isFractional)
{
    this->start = start;
    this->end = end;

    for (unsigned int pixel = 0; pixel < this->length(); pixel++)
    {
        pixels.push_back(new VirtualPixel());
    }
}

unsigned int LinearVirtualStrip::length()
{
    return abs((int)end - (int)start) + 1;
}

MatrixVirtualStrip::MatrixVirtualStrip(
    unsigned int x,
    unsigned int y,
    unsigned int length,
    bool isHorizontal,
    bool isPositive,
    bool isFractional) : VirtualStrip(isFractional)
{
    this->x = x;
    this->y = y;
    this->stripLength = length;
    this->isHorizontal = isHorizontal;
    this->isPositive = isPositive;

    for (unsigned int pixel = 0; pixel < this->length(); pixel++)
    {
        pixels.push_back(new VirtualPixel());
    }
}

MatrixVirtualStrip::~MatrixVirtualStrip()
{
    for (unsigned int pixel = 0; pixel < length(); pixel++)
    {
        delete pixels[pixel];
    }
}

unsigned int MatrixVirtualStrip::length()
{
    return stripLength;
}

void MatrixVirtualStrip::getComponents(MatrixStripComponent *components)
{
    unsigned int count = length();

    int8_t positivity = isPositive ? 1 : -1;
    int8_t horizontality = isHorizontal ? 1 : 0;
    int8_t verticality = isHorizontal ? 0 : 1;

    for (unsigned int component = 0; component < count; component++)
    {
        components[component] = {
            x + (component * positivity) * horizontality,
            y + (component * positivity) * verticality,
            pixels[component]};
    }
}
