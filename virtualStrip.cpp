#include "virtualStrip.h"

VirtualStrip::VirtualStrip(bool isFractional, MaskMode mode)
{
    this->isFractional = isFractional;
    this->mode = mode;
}

void VirtualStrip::setMask(uint16_t value)
{
    uint16_t mask = isFractional
                        ? (uint16_t)(((uint16_t)this->length()) * ((double)value / 65535))
                        : value;

    double normalised = isFractional
                            ? (double)value / 65535
                            : (double)value / this->length();

    // Clamp our normalised value.
    normalised = normalised > 1 ? 1 : normalised;
    normalised = normalised < 0 ? 0 : normalised;

    unsigned int length = this->length();

    srand(millis());

    switch (mode)
    {
    case MaskMode::Fill:
        for (unsigned int pixel = 0; pixel < length; pixel++)
        {
            getPixel(pixel)->mask(pixel >= mask);
        }
        break;
    case MaskMode::Dot:
        for (unsigned int pixel = 0; pixel < length; pixel++)
        {
            getPixel(pixel)->mask(pixel != mask);
        }
        break;
    case MaskMode::FillReverse:
        for (unsigned int pixel = 0; pixel < length; pixel++)
        {
            getPixel(pixel)->mask(pixel < mask);
        }
        break;
    case MaskMode::Random:
        for (unsigned int pixel = 0; pixel < length; pixel++)
        {
            getPixel(pixel)->setMasker(Masker::Random, normalised);
        }
        break;
    case MaskMode::Brightness:
        for (unsigned int pixel = 0; pixel < length; pixel++)
        {
            getPixel(pixel)->setMasker(Masker::Brightness, normalised);
        }
        break;
    case MaskMode::Transparency:
        for (unsigned int pixel = 0; pixel < length; pixel++)
        {
            getPixel(pixel)->setMasker(Masker::Transparency, normalised);
        }
        break;
    default:
        Serial.println("Invalid masking mode");
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

LinearVirtualStrip::LinearVirtualStrip(
    unsigned int start,
    unsigned int end,
    bool isFractional,
    MaskMode mode) : VirtualStrip(isFractional, mode)
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
    unsigned int thickness,
    bool isHorizontal,
    bool isPositive,
    bool isFractional,
    MaskMode mode) : VirtualStrip(isFractional, mode)
{
    this->x = x;
    this->y = y;
    this->stripLength = length;
    this->thickness = thickness;
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

unsigned int MatrixVirtualStrip::count()
{
    return stripLength * thickness;
}

void MatrixVirtualStrip::getComponents(MatrixStripComponent *components)
{
    int8_t positivity = isPositive ? 1 : -1;
    int8_t horizontality = isHorizontal ? 1 : 0;
    int8_t verticality = isHorizontal ? 0 : 1;

    for (unsigned int parallel = 0; parallel < thickness; parallel++)
    {
        for (unsigned int component = 0; component < stripLength; component++)
        {
            components[parallel * stripLength + component] = {
                x + (component * positivity) * horizontality + (verticality * parallel),
                y + (component * positivity) * verticality + (horizontality * parallel),
                pixels[component]};
        }
    }
}
