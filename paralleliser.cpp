#include "paralleliser.h"

void Paralleliser::draw(std::vector<Strip *> strips)
{
    std::vector<unsigned int> clockPins;

    for (unsigned int i = 0; i < strips.size(); i++)
    {
        Strip *strip = strips.at(i);
        strip->prepare();

        // If we haven't seen this pin before, remember it.
        if (std::find(clockPins.begin(), clockPins.end(), strip->clockPin) == clockPins.end())
        {
            clockPins.push_back(strip->clockPin);
        }
    }

    bool allEmpty = false;
    unsigned int count = 0;

    while (!allEmpty && count < 5000)
    {
        allEmpty = true;

        std::vector<uint8_t> values;

        for (unsigned int i = 0; i < strips.size(); i++)
        {
            Strip *strip = strips.at(i);
            // if (i == 0)
            // {
            //     Serial.printf("Value: %d\n", value);
            // }
            values.push_back(strip->driver->popPrepared());
            // Strip *strip = strips.at(i);
            // uint8_t value = strip->driver->popPrepared();

            // Serial.printf("Value: %d\n", value);

            // for (int bit = 7; bit >= 0; bit--)
            // {
            //     digitalWrite(strip->dataPin, (value >> bit) & 1);
            //     // digitalWrite(12, (value >> bit) & 1);
            //     // digitalWrite(27, (value >> bit) & 1);
            //     // digitalWrite(26, (value >> bit) & 1);
            //     // digitalWrite(25, (value >> bit) & 1);
            //     // digitalWrite(33, (value >> bit) & 1);
            //     // digitalWrite(32, (value >> bit) & 1);
            //     // digitalWrite(2, (value >> bit) & 1);
            //     // digitalWrite(strip->clockPin, HIGH);
            //     // digitalWrite(strip->clockPin, LOW);
            //     digitalWrite(13, HIGH);
            //     digitalWrite(13, LOW);
            // for (unsigned int i = 0; i < clockPins.size(); i++)
            // {
            //     digitalWrite(13, HIGH);
            //     digitalWrite(13, LOW);
            // }
            // }
            allEmpty = allEmpty && strip->driver->queueEmpty;
        }

        for (int bit = 7; bit >= 0; bit--)
        {
            for (unsigned int i = 0; i < values.size(); i++)
            {
                digitalWrite(strips.at(i)->dataPin, (values.at(i) >> bit) & 1);
            }

            for (unsigned int i = 0; i < clockPins.size(); i++)
            {
                digitalWrite(clockPins.at(i), HIGH);
                digitalWrite(clockPins.at(i), LOW);
            }
        }

        count++;
    }

    if (count >= 5000)
    {
        Serial.println("Warning: Paralleliser took too long");
    }
}