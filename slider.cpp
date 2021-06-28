// slider.cpp

#include "slider.h"

Slider::Slider(Display &d, const Rect &r) :
    m_display(d),
    m_rect(r), 
    m_val(0)
{
}

// Show the slider
void Slider::show()
{
    m_rect.h = SLIDER_HEIGHT;

    TFT_eSPI &tft = m_display.get_tft();

    tft.fillRect(m_rect.x, m_rect.y, m_rect.w, SLIDER_HEIGHT, TFT_BLACK);

    tft.fillRect(
            m_rect.x,
            m_rect.y+(SLIDER_HEIGHT-SLIDER_BAR_HEIGHT)/2,
            m_rect.w,
            SLIDER_BAR_HEIGHT,
            SLIDER_BG_COLOR);
    tft.fillCircle(
            m_rect.x+(m_val+SLIDER_BUTTON_RADIUS/2),
            m_rect.y+(SLIDER_BAR_HEIGHT/2)+(SLIDER_BUTTON_RADIUS/2),
            SLIDER_BUTTON_RADIUS,
            TFT_BLACK);
    tft.fillCircle(
            m_rect.x+(m_val+SLIDER_BUTTON_RADIUS/2)+2,
            m_rect.y+(SLIDER_BAR_HEIGHT/2)+(SLIDER_BUTTON_RADIUS/2)+2,
            SLIDER_BUTTON_RADIUS-2,
            SLIDER_BUTTON_COLOR);
}

void Slider::update_display()
{
    this->show();
}

uint16_t Slider::value_at_x(uint16_t x) const
{
    return x-m_rect.x;
}

void Slider::set(uint16_t value)
{
    bool redraw = m_val != value;
    m_val = value;
    if (redraw)
        show();
}

bool Slider::within(uint16_t x, uint16_t y)
{
    return ((x > m_rect.x)
            && (x < m_rect.x+m_rect.w)
            && (y > m_rect.y)
            && (y < m_rect.y+m_rect.h));
}
