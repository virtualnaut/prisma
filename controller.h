#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <algorithm>
#include "strip.h"
#include "configTemplates.h"
#include "colour.h"
#include "setup.h"
#include "virtualStrip.h"

#define MAX_VIRTUAL_STRIPS 256

typedef struct VirtualStripMessage
{
    bool isFractional;  // Whether values set on this strip are fractions of the full strip of a number of pixels.
    unsigned int start; // The start pixel for this strip.
    unsigned int end;   // The end pixel for this strip.
} VirtualStripMessage;

enum VirtualStripStatus
{
    Valid,      // The virtual strips set had no errors.
    OutOfBounds // One or more virtual strips include pixels that don't exist.
};

class Controller
{
public:
    Controller(const StripConfiguration *strips, unsigned int count);

    void setAll(ColourRGB colour);
    void clearAll();
    void mergePixel(unsigned int pixel, VirtualPixel *seconary);
    void draw();

    void setMelds(const bool melds[STRIP_COUNT]);
    void setOrder(const char order[STRIP_COUNT]);

    VirtualStripStatus setVirtualStrips(VirtualStripMessage strips[MAX_VIRTUAL_STRIPS], char count);

    VirtualStrip *getVirtualStrip(unsigned int strip);

    void setMask(unsigned int strip, uint16_t value);

protected:
    std::vector<Strip *> strips;
    unsigned int pixelCount = 0;

    std::vector<VirtualStrip *> virtualStrips;

    bool melds[STRIP_COUNT];
    char order[STRIP_COUNT];
};

#endif