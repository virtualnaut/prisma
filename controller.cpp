#include "controller.h"

Controller::Controller(const StripConfiguration *strips, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        StripConfiguration config = strips[i];
        this->strips.push_back(new Strip(config.count, config.dataPin, config.clockPin));
        this->order[i] = i;
    }
}

void Controller::setPixel(unsigned int pixel, ColourRGB colour)
{
    unsigned int covered = 0;
    for (unsigned int i = 0; i < strips.size(); i++)
    {
        Strip *strip = strips[order[i]];

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

void Controller::setMelds(const bool melds[STRIP_COUNT])
{
    memcpy(this->melds, melds, STRIP_COUNT * sizeof(bool));
}

void Controller::setOrder(const char order[STRIP_COUNT])
{
    memcpy(this->order, order, STRIP_COUNT * sizeof(char));
}