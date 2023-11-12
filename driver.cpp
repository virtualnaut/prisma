#include "driver.h"

Driver::Driver(unsigned int count, unsigned int dataPin, unsigned int clockPin)
    : count(count), dataPin(dataPin), clockPin(clockPin)
{
}

Driver *DriverFactory::create(unsigned int count, unsigned int dataPin, unsigned int clockPin, LEDChip ledType)
{
    switch (ledType)
    {
    case LEDChip::APA102:
    case LEDChip::SK9822:
        return new APA102Driver(count, dataPin, clockPin);

    case LEDChip::WS2812B:
        return new WS2812BDriver(count, dataPin, clockPin);
    }
}

// void APA102Driver::draw(ColourRGB *colours)
// {
//     initialisePins();
//     startFrame();

//     for (unsigned int colour = 0; colour < count; colour++)
//     {
//         sendPixel(colours[colour]);
//     }

//     endFrame();
// }

void APA102Driver::prepare(ColourRGB *colours)
{
    initialisePins();
    startFrame();

    for (unsigned int colour = 0; colour < count; colour++)
    {
        sendPixel(colours[colour]);
    }

    endFrame();

    queueEmpty = false;
}

void APA102Driver::initialisePins()
{
    pinMode(dataPin, OUTPUT);
    digitalWrite(dataPin, LOW);
    pinMode(clockPin, OUTPUT);
    digitalWrite(clockPin, LOW);
}

void APA102Driver::startFrame()
{
    sendBlanks(4);
}

void APA102Driver::endFrame()
{
    // sendBlanks((count + 14) / 16);
    sendValue(0xFF);
    sendValue(0xFF);
    sendValue(0xFF);
    sendValue(0xFF);
}

void APA102Driver::sendBit(uint8_t bit)
{
    digitalWrite(dataPin, bit);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
}

void APA102Driver::sendBlanks(unsigned int n)
{
    for (unsigned int byte = 0; byte < n; byte++)
    {
        sendValue(0);
    }
}

void APA102Driver::sendValue(uint8_t value)
{
    dataQueue.push(value);
    // for (int bit = 7; bit >= 0; bit--)
    // {
    //     sendBit(value >> bit & 1);
    // }
}

void APA102Driver::sendPixel(ColourRGB colour)
{
    sendValue(0b11100000 | 12);
    sendValue(colour.blue);
    sendValue(colour.green);
    sendValue(colour.red);
}

uint8_t APA102Driver::popPrepared()
{
    if (dataQueue.empty())
    {
        queueEmpty = true;
        return 0;
    }

    uint8_t value = dataQueue.front();
    dataQueue.pop();

    return value;
}

WS2812BDriver::WS2812BDriver(unsigned int count, unsigned int dataPin, unsigned int clockPin) : Driver(count, dataPin, clockPin)
{
    driverStrip = new NeoPixelBus<NEO_PIXEL_COLOUR_FEATURE, NEO_PIXEL_METHOD>(count, dataPin);
    driverStrip->Begin();
    driverStrip->Show();
}

void WS2812BDriver::draw(ColourRGB *colours)
{
    for (unsigned int pixel = 0; pixel < count; pixel++)
    {
        ColourRGB colour = colours[pixel];
        RgbColor c(colour.red, colour.green, colour.blue);
        driverStrip->SetPixelColor(pixel, c);
    }
    driverStrip->Show();
}