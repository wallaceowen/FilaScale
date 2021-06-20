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

    tft.fillRect(m_rect.x, m_rect.y, m_rect.w, SLIDER_HEIGHT, SLIDER_BG_COLOR);

    // tft.fillRect(m_rect.x, m_rect.y, m_rect.w, SLIDER_HEIGHT, SLIDER_COLOR);
    tft.fillRect(
            m_rect.x,
            m_rect.y+(SLIDER_HEIGHT-SLIDER_BAR_HEIGHT)/2,
            m_rect.w,
            SLIDER_BAR_HEIGHT,
            SLIDER_COLOR);
    tft.fillCircle(
            m_rect.x+m_val+SLIDER_BUTTON_RADIUS/2,
            m_rect.y+m_val+SLIDER_BUTTON_RADIUS/2,
            SLIDER_BUTTON_RADIUS,
            SLIDER_BUTTON_COLOR);
    tft.drawCircle(
            (m_rect.x+m_val+SLIDER_BUTTON_RADIUS/2)+1,
            (m_rect.y+m_val+SLIDER_BUTTON_RADIUS/2)+1,
            SLIDER_BUTTON_RADIUS-2,
            TFT_BLACK);

    // tft.fillCircle(m_rect.x, m_rect.y, m_val, m_rect.h, SLIDER_COLOR);
}

void Slider::update_display()
{
}

void Slider::set(uint16_t value)
{
    // m_val = map(value, m_inlo, m_rect.x, m_inhi, m_rect.x+m_rect.w);
    bool redraw = m_val == value;
    m_val = value;
    if (redraw)
        show();
}

bool Slider::within(uint16_t x, uint16_t y)
{
#ifdef DEBUG_WITHIN
    sprintf(dbg_buffer, "\"%s\".within %d, %d vs %d, %d, %d, %d",
            b_d.label, x, y, m_rect.x, m_rect.y, m_rect.x+m_rect.w, m_rect.y+m_rect.h);
    Serial.println(dbg_buffer);
#endif

    return ((x > m_rect.x)
            && (x < m_rect.x+m_rect.w)
            && (y > m_rect.y)
            && (y < m_rect.y+m_rect.h));
}
