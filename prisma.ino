#include "strip.h"

const uint8_t leftDataPin = 23;
const uint8_t leftClockPin = 22;
const uint8_t leftLength = 118;

const uint8_t rightDataPin = 14;
const uint8_t rightClockPin = 12;
const uint8_t rightLength = 117;

Strip strip = Strip(leftLength, leftDataPin, leftClockPin);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    // Add stuff here later...
    while (true)
    {
        strip.setPixelColour(117, {255, 255, 255});
        strip.draw();
        delay(500);
        strip.setPixelColour(117, {0, 0, 0});
        strip.draw();
        delay(50);
    }
}