#include "controller.h"

Controller::Controller(const StripConfiguration *strips, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        StripConfiguration config = strips[i];
        this->strips.push_back(new Strip(config.count, config.dataPin, config.clockPin));
    }

    melds = new bool[count]{true, false};
}

void Controller::setPixel(unsigned int pixel, ColourRGB colour)
{
    unsigned int covered = 0;
    for (unsigned int i = 0; i < strips.size(); i++)
    {
        Strip *strip = strips[i];

        if (pixel >= covered && pixel < covered + strip->count)
        {
            strip->setPixelColour(
                melds[i]
                    ? pixel - covered
                    : (strip->count - 1) - (pixel - covered),
                colour);
            break;
        }
        covered += strip->count;
    }
}

void Controller::setAll(ColourRGB colour)
{
    for (unsigned int i = 0; i < strips.size(); i++)
    {
        strips[i]->setAll(colour);
    }
}

void Controller::draw()
{
    for (unsigned int i = 0; i < strips.size(); i++)
    {
        strips[i]->draw();
    }
}