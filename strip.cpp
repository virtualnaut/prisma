#include "strip.h"

Strip::Strip(unsigned int count, const unsigned int dataPin, const unsigned int clockPin)
{
    this->count = count;
    this->dataPin = dataPin;
    this->clockPin = clockPin;

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

    initialisePins();
    startFrame();

    for (unsigned int colour = 0; colour < count; colour++)
    {
        sendPixel(colours[colour]);
    }

    endFrame();

    // Do the APA102 stuff here...
}

void Strip::initialisePins()
{
    pinMode(dataPin, OUTPUT);
    digitalWrite(dataPin, LOW);
    pinMode(clockPin, OUTPUT);
    digitalWrite(clockPin, LOW);
}

void Strip::startFrame()
{
    sendBlanks(4);
}

void Strip::endFrame()
{
    sendBlanks((count + 14) / 16);
}

void Strip::sendBit(uint8_t bit)
{
    digitalWrite(dataPin, bit);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
}

void Strip::sendBlanks(unsigned int n)
{
    for (unsigned int byte = 0; byte < n; byte++)
    {
        for (unsigned int bit = 0; bit < 8; bit++)
        {
            sendBit(LOW);
        }
    }
}

void Strip::sendValue(uint8_t value)
{
    for (int bit = 7; bit >= 0; bit--)
    {
        sendBit(value >> bit & 1);
    }
}

void Strip::sendPixel(ColourRGB colour)
{
    sendValue(0b11100000 | 12);
    sendValue(colour.blue);
    sendValue(colour.green);
    sendValue(colour.red);
}