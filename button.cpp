
#include <Arduino.h>
#include <stdio.h>

#include "button.h"

// #define DEBUG_BUTTON_CTOR
// #define DEBUG_BUTTON_DRAW
// #define DEBUG_WITHIN

ButtonData::ButtonData(const char *l, uint16_t pfg, uint16_t pbg, uint16_t txt_datum)
    : label(l), fg(pfg), bg(pbg), datum(txt_datum) { }

Button::Button(const ButtonData &d, const Rect &r)
    : m_b_d(d), m_rect(r) { }

void Button::draw(Display &d)
{
    this->draw(d.get_tft());
}

void Button::draw(TFT_eSPI &tft)
{
    tft.fillRoundRect(m_rect.x, m_rect.y, m_rect.w-1, m_rect.h-1, m_rect.h/2, TFT_WHITE);
    tft.fillRoundRect(m_rect.x+2, m_rect.y+2, m_rect.w-1, m_rect.h-1, m_rect.h/2, TFT_BLACK);
    tft.fillRoundRect(m_rect.x+1, m_rect.y+1, m_rect.w-2, m_rect.h-2, m_rect.h/2, m_b_d.bg);
    tft.setTextColor(m_b_d.fg, m_b_d.bg);
    int16_t label_x =  0;
    int16_t label_y =  0;
    if (m_b_d.datum == CC_DATUM)
    {
        label_x = m_rect.x+(m_rect.w/2);
        label_y = m_rect.y+(m_rect.h/2)+(BUTTON_PAD/2)-1;
    }
    else
    {
        label_x = m_rect.x;
        label_y = m_rect.y;
    }
    tft.setTextDatum(m_b_d.datum);
    tft.drawString(m_b_d.label, label_x, label_y, BUTTON_FONT);
}

bool Button::within(uint16_t x, uint16_t y)
{
    return ((x > m_rect.x)
            && (x < m_rect.x+m_rect.w)
            && (y > m_rect.y)
            && (y < m_rect.y+m_rect.h));
}
