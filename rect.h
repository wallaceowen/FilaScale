#ifndef __rect_h
#define __rect_h

#include <stdint.h>
#include "display.h"

struct Rect
{
    Rect(uint16_t px, uint16_t py, uint16_t pw, uint16_t ph)
        : x(px), y(py), w(pw), h(ph)
    { }
    uint16_t x, y, w, h;
};

#endif
