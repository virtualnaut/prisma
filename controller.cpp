#include "controller.h"

Controller::Controller(const StripConfiguration *strips, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        StripConfiguration config = strips[i];
        this->strips.push_back(new Strip(config.count, config.dataPin, config.clockPin, LED_CHIP));
        this->pixelCount += config.count;
        this->order[i] = i;
    }
}

void Controller::mergePixel(unsigned int pixel, VirtualPixel *seconary)
{
    unsigned int covered = 0;
    for (unsigned int i = 0; i < strips.size(); i++)
    {
        Strip *strip = strips[order[i]];

        if (pixel >= covered && pixel < covered + strip->count)
        {
            strip->mergePixel(
                melds[i]
                    ? pixel - covered
                    : (strip->count - 1) - (pixel - covered),
                seconary);
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

    for (unsigned int strip = 0; strip < virtualStrips.size(); strip++)
    {
        virtualStrips[strip]->setAll(colour);
    }
}

void Controller::clearAll()
{
    for (unsigned int i = 0; i < strips.size(); i++)
    {
        strips[i]->setAll({0, 0, 0});
    }

    // for (unsigned int strip = 0; strip < virtualStrips.size(); strip++)
    // {
    //     virtualStrips[strip]->setAll({0, 0, 0, 0});
    // }
}

void Controller::draw()
{
    for (unsigned int strip = 0; strip < virtualStrips.size(); strip++)
    {
        VirtualStrip *virtualStrip = virtualStrips[strip];
        for (unsigned int relativePixel = 0; relativePixel < virtualStrip->length(); relativePixel++)
        {
            if (virtualStrip->start <= virtualStrip->end)
            {
                mergePixel(virtualStrip->start + relativePixel, virtualStrip->getPixel(relativePixel));
            }
            else
            {
                mergePixel(virtualStrip->start - relativePixel, virtualStrip->getPixel(relativePixel));
            }
        }
    }

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

VirtualStripStatus Controller::setVirtualStrips(VirtualStripMessage virtualStrips[MAX_VIRTUAL_STRIPS], char count)
{
    this->virtualStrips.clear();

    // Make sure that none of the ranges goes outside the number of pixels we have.
    for (unsigned int virtualStrip = 0; virtualStrip < count; virtualStrip++)
    {
        VirtualStripMessage strip = virtualStrips[virtualStrip];

        if (strip.start >= pixelCount || strip.end >= pixelCount)
        {
            return VirtualStripStatus::OutOfBounds;
        }

        this->virtualStrips.push_back(new VirtualStrip(strip.start, strip.end, strip.isFractional));
    }

    return VirtualStripStatus::Valid;
}

VirtualStrip *Controller::getVirtualStrip(unsigned int strip)
{
    return virtualStrips[strip];
}

void Controller::setMask(unsigned int strip, uint16_t value)
{
    if (strip >= virtualStrips.size())
    {
        Serial.println("Cannot set the mask for an undefined virtual strip");
        return;
    }
    virtualStrips[strip]->setMask(value);
}