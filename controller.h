#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <algorithm>

#include "strip.h"
#include "configTemplates.h"
#include "colour.h"
#include "setup.h"
#include "virtualStrip.h"
#include "matrix.h"
#include "messages.h"

enum VirtualStripStatus
{
    Valid,      // The virtual strips set had no errors.
    OutOfBounds // One or more virtual strips include pixels that don't exist.
};

enum ControllerMode
{
    SingleMode,
    MatrixMode,
};

class Controller
{
public:
    Controller(const StripConfiguration *strips, unsigned int count);

    void setAll(ColourRGB colour);
    void clearAll();
    void mergePixel(unsigned int pixel, VirtualPixel *secondary);

    void draw();

    void setMelds(const bool melds[STRIP_COUNT]);
    void setOrder(const char order[STRIP_COUNT]);

    VirtualStripStatus setVirtualStrips(LinearVirtualStripMessage strips[MAX_VIRTUAL_STRIPS], char count);

    VirtualStrip *getVirtualStrip(unsigned int strip);

    Matrix *getMatrix();

    void setMask(unsigned int strip, uint16_t value);

    void initialiseMatrix(unsigned int width, unsigned int height);

protected:
    void drawForSingleStrip();
    void drawForMatrix();

    ControllerMode mode;
    std::vector<Strip *> strips;
    unsigned int pixelCount = 0;

    std::vector<LinearVirtualStrip *> virtualStrips;

    bool melds[STRIP_COUNT];
    char order[STRIP_COUNT];

    Matrix *matrix;
};

#endif