#ifndef MESSAGES_H
#define MESSAGES_H

#define MAX_VIRTUAL_STRIPS 256

typedef struct LinearVirtualStripMessage
{
    bool isFractional;  // Whether values set on this strip are fractions of the full strip of a number of pixels.
    unsigned int start; // The start pixel for this strip.
    unsigned int end;   // The end pixel for this strip.
} LinearVirtualStripMessage;

typedef struct MatrixVirtualStripMessage
{
    bool isFractional;   // Whether values set on this strip are fractions of the full strip of a number of pixels.
    bool isPositive;     // Whether the strip should extend in the positive or negative direction.
    bool isHorizontal;   // Whether the strip should be displayed horizontally or vertically.
    unsigned int x;      // X-coordinate of the start of the strip.
    unsigned int y;      // Y-coordinate of the start of the strip.
    unsigned int length; // Length of the strip.
} MatrixVirtualStripMessage;

#endif