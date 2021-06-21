#ifndef __button_h
#define __button_h

#include <stdint.h>

#include "display.h"
#include "rect.h"

#define BUTTON_FONT 4
#define BUTTON_RADIUS 10

typedef void (*ButtonCB)(const char *label, bool pressed, void *user_data);

struct ButtonData
{
    ButtonData(const char *label, uint16_t color, uint16_t fg, uint16_t bg);
    const char *label;
    uint16_t color;
    uint16_t fg;
    uint16_t bg;
};

class Button
{
public:
    Button(const ButtonData &data, const Rect &r);
    void draw(Display &d);
    void draw(TFT_eSPI&);
    bool within(uint16_t x, uint16_t y);
    const char *label(void) const { return b_d.label; }
private:
    ButtonData b_d;
    Rect rect;
};

typedef Button *BPTR;

#endif
