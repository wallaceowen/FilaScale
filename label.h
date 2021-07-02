#ifndef __label_h
#define __label_h

#include <stdint.h>

#include "display.h"
#include "rect.h"
#include "point.h"

#define LABEL_FONT 4
#define BUTTON_RADIUS 10
#define LABEL_MAX 42


struct LabelData
{
    LabelData(const char *label, uint16_t fg, uint16_t bg, uint8_t font, bool center);
    char label[LABEL_MAX];
    uint16_t fg;
    uint16_t bg;
    uint8_t font;
    bool center;
};

class Label
{
public:
    Label(const LabelData &data, const Rect &p);
    void draw(Display &d);
    void draw(TFT_eSPI&);
    void set_label(const char *txt);

private:

    LabelData m_labeldata;
    Rect      m_rect;
};

#endif
