
#include <Arduino.h>
#include <stdio.h>

#include "button.h"

static char dbg_buffer[72];

ButtonData::ButtonData(const char *l, uint16_t pc, uint16_t pfg, uint16_t pbg)
    : label(l), color(pc), fg(pfg), bg(pbg)
{
}

Button::Button(const ButtonData &d, const Rect &r) :
    b_d(d),
    rect(r)
{
#define DEBUG_BUTTON_CTOR
#ifdef DEBUG_BUTTON_CTOR
    sprintf(dbg_buffer, "button \"%s\" created at x %d, y %d, w %d, h %d",
            b_d.label, rect.x, rect.y, rect.w, rect.h);
    Serial.println(dbg_buffer);
#endif
}

void Button::draw(Display &d)
{
    Serial.println(dbg_buffer);
    TFT_eSPI &tft = d.get_tft();
    this->draw(tft);
}

void Button::draw(TFT_eSPI &tft)
{
    sprintf(dbg_buffer, "--- button \"%s\" drawing", b_d.label);
    Serial.println(dbg_buffer);
    int16_t height = tft.fontHeight(BUTTON_FONT);
    tft.fillRect(rect.x, rect.y, rect.w, rect.h, b_d.color);
    tft.setTextColor(b_d.fg, b_d.bg);
    tft.setTextDatum(TL_DATUM);
    int16_t t_width = tft.textWidth(b_d.label, BUTTON_FONT);
    int32_t label_x = rect.x+(rect.w/2)-(t_width/2);
    int32_t label_y = rect.y+(rect.h/2)-height/2;
    tft.drawString(b_d.label, label_x, label_y, BUTTON_FONT);

#define DEBUG_BUTTON_DRAW
#ifdef DEBUG_BUTTON_DRAW
    {
        char dbg_buff[60];
        sprintf(dbg_buff, "drawing label \"%s\" at %d, %d", b_d.label, label_x, label_y);
        Serial.println(dbg_buff);
    }
#endif
}

bool Button::within(uint16_t x, uint16_t y)
{
#define DEBUG_WITHIN
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
