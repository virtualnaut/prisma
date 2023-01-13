#include "matrix.h"

Matrix::Matrix(unsigned int width, unsigned int height) : width(width), height(height)
{
    this->length = width * height;

    for (unsigned int pixel = 0; pixel < length; pixel++)
    {
        pixels.push_back(new VirtualPixel());
    }
}

Matrix::~Matrix()
{
    for (unsigned int virtualStrip = 0; virtualStrip < this->virtualStrips.size(); virtualStrip++)
    {
        delete this->virtualStrips[virtualStrip];
    }
    this->virtualStrips.clear();

    for (unsigned int pixel = 0; pixel < this->pixels.size(); pixel++)
    {
        delete this->pixels[pixel];
    }
    this->pixels.clear();
}

void Matrix::clearAll()
{
    for (unsigned int pixel = 0; pixel < length; pixel++)
    {
        pixels[pixel]->set({0, 0, 0, 0});
    }
}

void Matrix::setPixel(unsigned int x, unsigned int y, ColourRGBA colour)
{
    this->pixels.at(flattenCoords(x, y))->set(colour);
}

VirtualPixel *Matrix::getPixel(unsigned int pixel)
{
    return this->pixels.at(pixel);
}

VirtualPixel *Matrix::getPixel(unsigned int x, unsigned int y)
{
    return this->pixels.at(flattenCoords(x, y));
}

void Matrix::setHorizontalFlip(bool flipped)
{
    flippedHorizontally = flipped;
}

void Matrix::setVerticalFlip(bool flipped)
{
    flippedVertically = flipped;
}

void Matrix::setVirtualStrips(MatrixVirtualStripMessage virtualStrips[MAX_VIRTUAL_STRIPS], char count)
{
    for (unsigned int virtualStrip = 0; virtualStrip < this->virtualStrips.size(); virtualStrip++)
    {
        delete this->virtualStrips[virtualStrip];
    }

    this->virtualStrips.clear();

    // Make sure that none of the ranges goes outside the number of pixels we have.
    for (unsigned int virtualStrip = 0; virtualStrip < count; virtualStrip++)
    {
        MatrixVirtualStripMessage strip = virtualStrips[virtualStrip];

        this->virtualStrips.push_back(new MatrixVirtualStrip(
            strip.x,
            strip.y,
            strip.length,
            strip.thickness,
            strip.isHorizontal,
            strip.isPositive,
            strip.isFractional,
            strip.mode));
    }
}

MatrixVirtualStrip *Matrix::getVirtualStrip(unsigned int strip)
{
    return virtualStrips[strip];
}

void Matrix::setMask(unsigned int strip, uint16_t value)
{
    if (strip >= virtualStrips.size())
    {
        Serial.println("Cannot set the mask for an undefined virtual strip");
        return;
    }
    virtualStrips[strip]->setMask(value);
}

std::vector<VirtualPixel *> Matrix::getPixels()
{
    size_t stripCount = virtualStrips.size();
    for (unsigned int strip = 0; strip < stripCount; strip++)
    {
        unsigned int length = virtualStrips[strip]->count();

        MatrixStripComponent components[length];
        virtualStrips[strip]->getComponents(components);

        for (unsigned int component = 0; component < length; component++)
        {
            getPixel(components[component].x, components[component].y)->merge(components[component].pixel);
        }
    }

    return pixels;
}

void Matrix::setRegion(unsigned int x, unsigned int y, unsigned int width, unsigned int height, ColourRGBA *colours)
{
    unsigned int pixel = 0;
    for (unsigned int vertical = 0; vertical < height; vertical++)
    {
        for (unsigned int horizontal = 0; horizontal < width; horizontal++)
        {
            getPixel(x + horizontal, y + vertical)
                ->set(colours[pixel]);
            pixel++;
        }
    }
}

unsigned int Matrix::flattenCoords(unsigned int x, unsigned int y)
{
    unsigned int adjustedX = flippedHorizontally ? width - x - 1 : x;
    unsigned int adjustedY = flippedVertically ? height - y - 1 : y;
    return adjustedX * height + (adjustedX % 2 ? adjustedY : (height - adjustedY - 1));
}