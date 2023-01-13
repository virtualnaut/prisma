#ifndef DRIVER_H
#define DRIVER_H

#include <Arduino.h>

#include <NeoPixelBus.h>

#include "setup.h"
#include "configTemplates.h"
#include "colour.h"

/**
 * The abstract driver class.
 */
class Driver
{
public:
    Driver(unsigned int count, unsigned int dataPin, unsigned int clockPin);

    virtual void draw(ColourRGB *colours){};

protected:
    unsigned int count;
    unsigned int dataPin;
    unsigned int clockPin;
};

/**
 * Get a driver instance based on an LEDType.
 */
class DriverFactory
{
public:
    static Driver *create(unsigned int count, unsigned int dataPin, unsigned int clockPin, LEDChip ledType);
};

/**
 * Driver for APA102 or SK9822 LEDs
 */
class APA102Driver : public virtual Driver
{

public:
    APA102Driver(unsigned int count, unsigned int dataPin, unsigned int clockPin) : Driver(count, dataPin, clockPin){};
    void draw(ColourRGB *colours);

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
};

class WS2812BDriver : public virtual Driver
{

public:
    WS2812BDriver(unsigned int count, unsigned int dataPin, unsigned int clockPin);
    void draw(ColourRGB *colours);

protected:
    NeoPixelBus<NEO_PIXEL_COLOUR_FEATURE, NEO_PIXEL_METHOD> *driverStrip;
};

#endif