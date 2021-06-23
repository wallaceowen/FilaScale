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
    ButtonData(const char *label, uint16_t color, uint16_t fg, uint16_t bg);
    const char *label;
    uint16_t color;
    uint16_t fg;
    uint16_t bg;
};

struct GridButtonData
{
    GridButtonData(const char *l, uint16_t r, uint16_t c, uint16_t w, uint16_t h, uint16_t f, uint16_t b)
        : row(r), col(c), width(w), height(h), fg(f), bg(b)
    {
        strcpy(label, l);
    }
    char label[8];
    unsigned row, col;
    uint16_t width, height;
    uint16_t fg, bg;
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
