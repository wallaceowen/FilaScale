#ifndef __label_h
#define __label_h

#include <stdint.h>

#include "display.h"
#include "rect.h"

#define BUTTON_FONT 4
#define BUTTON_RADIUS 10

struct LabelData
{
    LabelData(const char *label, uint16_t fg, uint16_t bg, uint8_t font, bool center);
    void label_offset(int &off_x, int &off_y);
    const char *label;
    uint16_t fg;
    uint16_t bg;
    uint8_t font;
    bool center;
};

class Label
{
public:
    Label(const LabelData &data, const Rect &r);
    bool draw(Display &d);
    bool draw(TFT_eSPI&);
    // const char *text(void) const { return b_d.label; }
private:
    LabelData m_labeldata;
    Rect m_rect;
};

// typedef Label *LPTR;

#endif
