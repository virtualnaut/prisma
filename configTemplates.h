#ifndef CONFIG_TEMPLATES_H
#define CONFIG_TEMPLATES_H

#include "Freenove_WS2812_Lib_for_ESP32.h"

typedef struct StripConfiguration
{
    unsigned int dataPin;
    unsigned int clockPin;
    unsigned int count;
} StripConfiguration;

enum LEDChip
{
    APA102,
    WS2812B,
    SK9822
};

#endif