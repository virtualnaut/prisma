#ifndef PIXEL_H
#define PIXEL_H

#include "colour.h"

class VirtualPixel;

enum Masker
{
    None = 0,
    Random,
    Brightness,
    Transparency,
};

class Pixel
{
public:
    Pixel();
    void set(ColourRGB colour);
    void set(ColourRGBA colour);

    void merge(VirtualPixel *pixel);

    ColourRGB getRGB();
    ColourRGBA getRGBA();

protected:
    ColourRGBA colour = {0, 0, 0, 0};
};

/**
 * Virtual pixels are contained in virtual strips so there may be multiple virtual pixels
 * which contribute to the colour of a single real pixel.
 */
class VirtualPixel : public Pixel
{
public:
    ColourRGB getRGB();
    ColourRGBA getRGBA();
    void mask(bool isMasked);

    void setMasker(Masker masker, double value);

protected:
    ColourRGBA applyMask(ColourRGBA colour);

    bool masked = false;
    Masker masker = Masker::None;
    double maskerValue;
};

#endif