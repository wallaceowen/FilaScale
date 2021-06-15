// bar.h

#ifndef __bar_h
#define __bar_h

#include <stdint.h>

#include "display.h"
#include "rect.h"

#ifdef USE_CLASS

class Bar
{
public:
    Bar(Display &d, const Rect &r, uint16_t inlo, uint16_t inhi, uint16_t incolor, uint16_t outcolor);
    virtual void show();
    // Scale the value to fit m_min, m_max
    void set(uint16_t value);

protected:

    Display &m_disp;
    Rect m_rect;
    uint16_t m_inlo;
    uint16_t m_inhi;
    uint16_t m_incolor;
    uint16_t m_outcolor;
    uint16_t m_val;
}

#else
extern void plot_bar(
        Display &d,
        const Rect &rect,
        uint16_t value,
        uint16_t inlo,
        uint16_t inhi,
        uint16_t incolor,
        uint16_t outcolor
        );
#endif

#endif
