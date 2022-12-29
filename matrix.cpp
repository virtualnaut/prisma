#include "matrix.h"

Matrix::Matrix(unsigned int width, unsigned int height) : width(width), height(height)
{
    this->length = width * height;

    for (unsigned int pixel = 0; pixel < this->length; pixel++)
    {
        pixels.push_back(new VirtualPixel());
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