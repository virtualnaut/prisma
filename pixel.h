#include "colour.h"

class Pixel
{
public:
    Pixel();
    void setRGB(ColourRGB colour);
    ColourRGB getRGB();

protected:
    ColourRGB colour = {0, 0, 0};
};