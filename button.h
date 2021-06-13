#ifndef __button_h
#define __button_h

#include <stdint.h>

#include "display.h"

struct Rect
{
    Rect(uint16_t px, uint16_t py, uint16_t pw, uint16_t ph)
        : x(px), y(py), w(pw), h(ph)
    { }
    uint16_t x, y, w, h;
};

struct ButtonData
{
    ButtonData(const char *label, uint16_t color, uint16_t tcolor);
    const char *label;
    uint16_t color;
    uint16_t txt_color;
};

class Button
{
public:
    Button(const char *label, const Rect &r, uint16_t color, uint16_t txt_color);
    bool draw(Display &d);
    bool within(uint16_t x, uint16_t y);
    const char *label(void) const { return m_label; }
private:
    const char *m_label;
    Rect rect;
    uint16_t color;
    uint16_t txt_color;
};

#endif
