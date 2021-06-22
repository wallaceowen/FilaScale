#ifndef __point_h
#define __point_h

#include <stdint.h>
#include "display.h"

struct Point
{
    Point(uint16_t px, uint16_t py) : x(px), y(py) { }
    uint16_t x;
    uint16_t y;
};

#endif
