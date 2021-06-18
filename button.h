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
    ButtonData(const char *label, uint16_t color, uint16_t tcolor);
    const char *label;
    uint16_t color;
    uint16_t txt_color;
};

class Button
{
public:
    Button(const ButtonData &data, const Rect &r);
    bool draw(Display &d);
    bool draw(TFT_eSPI&);
    bool within(uint16_t x, uint16_t y);
    const char *label(void) const { return b_d.label; }
private:
    ButtonData b_d;
    Rect rect;
};

typedef Button *BPTR;

#endif
