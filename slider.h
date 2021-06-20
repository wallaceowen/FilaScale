// slider.h

#ifndef __slider_h
#define __slider_h

#include <stdint.h>

#include "display.h"
#include "rect.h"

#define SLIDER_COLOR TFT_BLUE
#define SLIDER_HEIGHT 30
#define SLIDER_BAR_HEIGHT 20
#define SLIDER_BUTTON_RADIUS 14
#define SLIDER_BUTTON_COLOR TFT_RED
#define SLIDER_BG_COLOR TFT_WHITE

class Slider
{
public:
    Slider(Display &d, const Rect &r);

    uint16_t height(void) const { return SLIDER_HEIGHT; }

    void show();
    void update_display();

    bool within(uint16_t x, uint16_t y);

    // Scale the value to fit m_min, m_max
    void set(uint16_t value);

protected:

    Display &m_display;
    Rect m_rect;
    uint16_t m_val;
};

#endif
