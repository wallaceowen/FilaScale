// slider.h

#ifndef __slider_h
#define __slider_h

#include <stdint.h>

#include "display.h"
#include "rect.h"

#define SLIDER_BG_COLOR TFT_DARKGREY
#define SLIDER_HEIGHT 30
#define SLIDER_BAR_HEIGHT 20
#define SLIDER_BUTTON_RADIUS 10
#define SLIDER_BUTTON_COLOR TFT_RED

class Slider
{
public:
    Slider(Display &d, const Rect &r);

    uint16_t height(void) const { return SLIDER_HEIGHT; }

    void show();
    void update_display();

    uint16_t value_at_x(uint16_t x) const;

    bool within(uint16_t x, uint16_t y);

    // Scale the value to fit m_min, m_max
    void set(uint16_t value);

    uint16_t value(void) const { return m_val; }

protected:

    Display &m_display;
    Rect     m_rect;
    uint16_t m_val;
};

#endif
