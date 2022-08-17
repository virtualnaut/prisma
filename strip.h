#ifndef STRIP_H
#define STRIP_H

#include <vector>
#include <Arduino.h>

#include "colour.h"

class Strip
{
public:
    Strip(unsigned int count, unsigned int dataPin, unsigned int clockPin);

    unsigned int count;

    void setPixelColour(unsigned int pixel, ColourRGB colour);
    void setAll(ColourRGB colour);
    void render(ColourRGB *colours);
    void draw();

protected:
    /** Set up the pins */
    void initialisePins();

    /** Send the start frame */
    void startFrame();

    /** Send a pixel value to the lights */
    void sendPixel(ColourRGB colour);

    void sendValue(uint8_t value);
    void sendBlanks(unsigned int n);

    void sendBit(uint8_t bit);

    /** Send the end frame */
    void endFrame();

    unsigned int dataPin;
    unsigned int clockPin;

    std::vector<ColourRGB> pixels;
};

#endif