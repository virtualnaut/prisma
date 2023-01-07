#include "pixel.h"

#include "Arduino.h"

Pixel::Pixel()
{
}

void Pixel::set(ColourRGB colour)
{
    this->colour = {colour.red, colour.green, colour.blue, 1};
}

void Pixel::set(ColourRGBA colour)
{
    this->colour = {colour.red, colour.green, colour.blue, colour.alpha};
}

void Pixel::merge(VirtualPixel *pixel)
{
    ColourRGBA theirColour = pixel->getRGBA();
    ColourRGBA ourColour = getRGBA();

    float newAlpha = theirColour.alpha + ourColour.alpha * (1 - theirColour.alpha);

    colour = {
        (uint8_t)((theirColour.red * theirColour.alpha + ourColour.red * ourColour.alpha * (1 - theirColour.alpha)) / newAlpha),
        (uint8_t)((theirColour.green * theirColour.alpha + ourColour.green * ourColour.alpha * (1 - theirColour.alpha)) / newAlpha),
        (uint8_t)((theirColour.blue * theirColour.alpha + ourColour.blue * ourColour.alpha * (1 - theirColour.alpha)) / newAlpha),
        newAlpha};
}

ColourRGB Pixel::getRGB()
{
    return {
        colour.alpha * colour.red,
        colour.alpha * colour.green,
        colour.alpha * colour.blue,
    };
}

ColourRGBA Pixel::getRGBA()
{
    return colour;
}

ColourRGB VirtualPixel::getRGB()
{
    return masked
               ? ColourRGB{0, 0, 0}
               : Pixel::getRGB();
}

ColourRGBA VirtualPixel::getRGBA()
{
    if (masker != Masker::None)
    {
        return applyMask(Pixel::getRGBA());
    }

    return masked
               ? ColourRGBA{0, 0, 0, 0}
               : Pixel::getRGBA();
}

void VirtualPixel::mask(bool isMasked)
{
    masked = isMasked;
}

void VirtualPixel::setMasker(Masker masker, double value)
{
    this->masker = masker;
    this->maskerValue = value;
}

ColourRGBA VirtualPixel::applyMask(ColourRGBA colour)
{
    switch (masker)
    {
    case Masker::Random:
        return ((double)rand() / (double)RAND_MAX) > maskerValue
                   ? ColourRGBA{0, 0, 0, 0}
                   : colour;
    case Masker::Brightness:
        return colour::dim(colour, maskerValue);
    case Masker::Transparency:
        return colour::transparify(colour, maskerValue);
    }
}
