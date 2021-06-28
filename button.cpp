
#include <Arduino.h>
#include <stdio.h>

#include "button.h"

// #define DEBUG_BUTTON_CTOR
// #define DEBUG_BUTTON_DRAW
// #define DEBUG_WITHIN

ButtonData::ButtonData(const char *l, uint16_t pfg, uint16_t pbg, uint16_t txt_datum)
    : label(l), fg(pfg), bg(pbg), datum(txt_datum) { }

Button::Button(const ButtonData &d, const Rect &r)
    : b_d(d), rect(r) { }

void Button::draw(Display &d)
{
    TFT_eSPI &tft = d.get_tft();
    this->draw(tft);
}

void Button::draw(TFT_eSPI &tft)
{
    tft.fillRoundRect(rect.x, rect.y, rect.w, rect.h, rect.h/2, b_d.bg);
    tft.setTextColor(b_d.fg, b_d.bg);
    int16_t label_x =  0;
    int16_t label_y =  0;
    if (b_d.datum == CC_DATUM)
    {
        label_x = rect.x+(rect.w/2);
        label_y = rect.y+(rect.h/2);
    }
    else
    {
        label_x = rect.x;
        label_y = rect.y;
    }
    tft.setTextDatum(b_d.datum);
    tft.drawString(b_d.label, label_x, label_y, BUTTON_FONT);
}

bool Button::within(uint16_t x, uint16_t y)
{
    return ((x > rect.x)
            && (x < rect.x+rect.w)
            && (y > rect.y)
            && (y < rect.y+rect.h));
}
