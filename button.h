#ifndef __button_h
#define __button_h

#include <stdint.h>

#include "display.h"
#include "events.h"
#include "rect.h"

#define BUTTON_FONT 4
#define BUTTON_RADIUS 10

// typedef void (*PressEventCB)(const char *label, bool pressed, void *user_data);

struct ButtonData
{
    ButtonData(const char *label, uint16_t fg, uint16_t bg, uint16_t txt_datum = TL_DATUM);
    const char *label;
    uint16_t fg;
    uint16_t bg;
    uint16_t datum;
};

class Button
{
public:
    Button(const ButtonData &data, const Rect &r);

    void draw(Display &d);
    void draw(TFT_eSPI&);

    bool within(uint16_t x, uint16_t y);

    // Get the label
    const char *label(void) const { return b_d.label; }

    // Set the label
    void label(const char *l) { b_d.label = l; }

private:
    ButtonData b_d;
    Rect rect;
};

typedef Button *BPTR;

#endif
