#ifndef MELD_H
#define MELD_H

#include <vector>
#include "strip.h"
#include "configTemplates.h"
#include "colour.h"
#include "setup.h"

typedef struct Meld
{
    bool atStartA;
    unsigned int stripA;
    bool atStartB;
    unsigned int stripB;
} Meld;

typedef struct VirtualStrip
{
    unsigned int start;
    unsigned int end;
} VirtualStrip;

class Controller
{
public:
    Controller(const StripConfiguration *strips, unsigned int count);

    void setPixel(unsigned int pixel, ColourRGB colour);
    void setAll(ColourRGB colour);
    void draw();

    void setMelds(bool melds[STRIP_COUNT]);

protected:
    std::vector<Strip *> strips;
    bool melds[STRIP_COUNT];
};

#endif