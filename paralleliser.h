#ifndef PARALLELISER_H
#define PARALLELISER_H

#include <vector>

#include "strip.h"

class Paralleliser
{
public:
    static void draw(std::vector<Strip *> strips);
};

#endif
