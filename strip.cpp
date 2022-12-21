#include "strip.h"

Strip::Strip(unsigned int count, const unsigned int dataPin, const unsigned int clockPin, LEDChip ledType)
{
    this->count = count;
    this->dataPin = dataPin;
    this->clockPin = clockPin;

    this->driver = DriverFactory::create(count, dataPin, clockPin, ledType);

    for (unsigned int pixel = 0; pixel < count; pixel++)
    {
        pixels.push_back(new Pixel());
    }
}

void Strip::setPixelColour(unsigned int pixel, ColourRGB colour)
{
    pixels.at(pixel)->set(colour);
}

void Strip::mergePixel(unsigned int pixel, VirtualPixel *secondary)
{
    pixels.at(pixel)->merge(secondary);
}

void Strip::setAll(ColourRGB colour)
{
    for (unsigned int pixel = 0; pixel < pixels.size(); pixel++)
    {
        setPixelColour(pixel, colour);
    }
}

void Strip::render(ColourRGB *colours)
{
    for (unsigned int pixel; pixel < pixels.size(); pixel++)
    {
        colours[pixel] = pixels[pixel]->getRGB();
    }
}

void Strip::draw()
{
    ColourRGB colours[count];
    render(colours);
    this->driver->draw(colours);
}