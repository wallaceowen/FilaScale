#ifndef __button_h
#define __button_h

#include <stdint.h>

#include "display.h"

struct Rect
{
    Rect(uint16_t px, uint16_t py, uint16_t pw, uint16_t ph )
        : x(px), y(py), w(pw), h(ph)
    { }
    uint16_t x, y, w, h;
};

struct Button
{
    // Button(const Rect &);
    Button(const Rect &r, int color, const char *label = 0);
    bool draw(Display &d);
    bool pressed(uint16_t x, uint16_t y);
    // void set_label(const char *text);
    Rect rect;
    int color;
    const char *label;
};

#endif
