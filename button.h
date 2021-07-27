#ifndef __button_h
#define __button_h

#include <stdint.h>

#include "display.h"
#include "events.h"
#include "rect.h"

#define BUTTON_FONT 4
#define BUTTON_PAD 8
#define BUTTON_RADIUS 10

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
    const char *label(void) const { return m_b_d.label; }

    // Set the label
    void label(const char *l) { m_b_d.label = l; }

    // Get the datum
    uint16_t datum(void) const { return m_b_d.datum; }

private:
    ButtonData m_b_d;
    Rect m_rect;
};

typedef Button *BPTR;

#endif
