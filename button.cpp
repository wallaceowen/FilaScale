
#include <Arduino.h>
#include <stdio.h>

#include "button.h"

static char dbg_buffer[72];

ButtonData::ButtonData(const char *l, uint16_t pfg, uint16_t pbg, uint16_t txt_datum)
    : label(l), fg(pfg), bg(pbg), datum(txt_datum)
{
}

Button::Button(const ButtonData &d, const Rect &r) :
    b_d(d),
    rect(r)
{
// #define DEBUG_BUTTON_CTOR
#ifdef DEBUG_BUTTON_CTOR
    sprintf(dbg_buffer, "button \"%s\" created at x %d, y %d, w %d, h %d",
            b_d.label, rect.x, rect.y, rect.w, rect.h);
    Serial.println(dbg_buffer);
#endif
}

void Button::draw(Display &d)
{
    TFT_eSPI &tft = d.get_tft();
    this->draw(tft);
}

#define DEBUG_BUTTON_DRAW
void Button::draw(TFT_eSPI &tft)
{
    tft.fillRect(rect.x, rect.y, rect.w, rect.h, b_d.bg);
    tft.setTextColor(b_d.fg, b_d.bg);
    int16_t label_x =  0;
    int16_t label_y =  0;
    int16_t t_width = tft.textWidth(b_d.label, BUTTON_FONT);
    int16_t t_height = tft.fontHeight(BUTTON_FONT);
#ifdef DEBUG_BUTTON_DRAW
    {
        sprintf(dbg_buffer, "but \"%s\" at %u, %u rectw %u recth %u width %hd height %hd",
                b_d.label, label_x, label_y, rect.w, rect.h, t_width, t_height);
        Serial.println(dbg_buffer);
    }
#endif
    if (b_d.datum == TC_DATUM)
    {
        label_x = rect.x+(rect.w/2)-(t_width/2);
        label_y = rect.y+(rect.h/2)-(t_height/2);
    }
    else
    {
        label_x = rect.x;
        label_y = rect.y;
    }
    tft.setTextDatum(b_d.datum);
    tft.drawString(b_d.label, label_x, label_y, BUTTON_FONT);
#ifdef DEBUG_BUTTON_DRAW
    {
        sprintf(dbg_buffer, "button \"%s\" drawn at %u, %u",
                b_d.label, label_x, label_y);
        Serial.println(dbg_buffer);
    }
#endif

}

bool Button::within(uint16_t x, uint16_t y)
{
// #define DEBUG_WITHIN
#ifdef DEBUG_WITHIN
    sprintf(dbg_buffer, "\"%s\".within %d, %d vs %d, %d, %d, %d",
            b_d.label, x, y, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
    Serial.println(dbg_buffer);
#endif

    return ((x > rect.x)
            && (x < rect.x+rect.w)
            && (y > rect.y)
            && (y < rect.y+rect.h));
}
