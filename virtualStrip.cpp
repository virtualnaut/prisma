#include "virtualStrip.h"

VirtualStrip::VirtualStrip(unsigned int start, unsigned int end, bool isFractional)
{
    this->start = start;
    this->end = end;
    this->isFractional = isFractional;

    for (unsigned int pixel = 0; pixel < this->length(); pixel++)
    {
        Serial.printf("Pix %d\n", pixel);
        pixels.push_back(new Pixel());
    }
}

unsigned int VirtualStrip::length()
{
    return abs((int)end - (int)start) + 1;
}

void VirtualStrip::setPixelColour(unsigned int pixel, ColourRGB colour)
{
    pixels.at(pixel)->set(colour);
}

void VirtualStrip::setPixelColour(unsigned int pixel, ColourRBGA colour)
{
    pixels.at(pixel)->set(colour);
}

Pixel *VirtualStrip::getPixel(unsigned int pixel)
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
