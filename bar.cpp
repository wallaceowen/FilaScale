// bar.cpp

#include "bar.h"

#ifdef USE_CLASS
Bar(Display &d, const Rect &r, uint16_t inlo, uint16_t inhi, uint16_t incolor, uint16_t outcolor) :
    m_display(d),
    m_rect(r), 
    m_inlo(inlo),
    m_inhi(inhi),
    m_incolor(incolor),
    m_outcolor(outcolor),
    m_val(0)
{
}

// Show the bar
virtual void Bar::show()
{
    TFT_eSPI &tft = m_disp.get_tft();
    tft.fillRect(m_rect.x, m_rect.y, m_val, m_rect.h);
}

void Bar::set(uint16_t value)
{
    m_val = map(value, m_inlo, m_rect.x, m_inhi, m_rect.x+m_rect.w);
}
#else
void plot_bar(
        Display &d,
        const Rect &rect,
        uint16_t value,
        uint16_t inlo,
        uint16_t inhi,
        uint16_t incolor,
        uint16_t outcolor)
{
    TFT_eSPI &tft = d.get_tft();
    int outlo = 0;
    int outhi = rect.w;
    int plotval = map((int) value, inlo, inhi, outlo, outhi);

    if (plotval < 0)
        plotval = 0;

    tft.fillRect(rect.x, rect.y, plotval, rect.h, incolor);
    tft.fillRect(rect.x+plotval, rect.y, rect.w-plotval, rect.h, outcolor);
    tft.drawRect(rect.x, rect.y, rect.w, rect.h-1, TFT_WHITE);
}
#endif

