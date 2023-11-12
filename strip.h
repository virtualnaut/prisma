#ifndef STRIP_H
#define STRIP_H

#include <vector>
#include <Arduino.h>

#include "configTemplates.h"
#include "colour.h"
#include "pixel.h"
#include "driver.h"

class Strip
{
public:
    Strip(unsigned int count, unsigned int dataPin, unsigned int clockPin, LEDChip ledType);
    unsigned int dataPin;
    unsigned int clockPin;

    unsigned int count;

    void setPixelColour(unsigned int pixel, ColourRGB colour);
    void mergePixel(unsigned int pixel, VirtualPixel *secondary);

    void setAll(ColourRGB colour);
    void render(ColourRGB *colours);
    void prepare();
    void draw();

    Driver *driver;

protected:
    std::vector<Pixel *> pixels;
};

#endif