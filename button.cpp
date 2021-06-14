
#include <Arduino.h>
#include <stdio.h>

#include "button.h"

#define BUTTON_FONT 4
#define BUTTON_RADIUS 10

static char dbg_buffer[72];

ButtonData::ButtonData(const char *l, uint16_t pc, uint16_t ptc) :
    label(l),
    color(pc),
    txt_color(ptc)
{
}

Button::Button(const char *l, const Rect &r, uint16_t c, uint16_t tc) :
    m_label(l),
    rect(r),
    color(c),
    txt_color(tc)
{
    sprintf(dbg_buffer, "button \"%s\" created at x %d, y %d, w %d, h %d",
            m_label, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
    Serial.println(dbg_buffer);
}

bool Button::draw(Display &d)
{
    TFT_eSPI &tft = d.get_tft();
    int16_t height = tft.fontHeight(BUTTON_FONT);
    tft.fillRoundRect(rect.x, rect.y, rect.w, rect.h, BUTTON_RADIUS, color);
    tft.setTextColor(txt_color);
    tft.setTextDatum(TL_DATUM);
    int16_t t_width = tft.textWidth(m_label, BUTTON_FONT);
    int32_t label_x = rect.x+(rect.w/2)-(t_width/2);
    int32_t label_y = rect.y+(rect.h/2)-height/2;
    tft.drawString(m_label, label_x, label_y, BUTTON_FONT);

    // char dbg_buff[60];
    // sprintf(dbg_buff, "drawing \"%s\" at %d, %d", m_label, label_x, label_y);
    // Serial.println(dbg_buff);
}

bool Button::within(uint16_t x, uint16_t y)
{
    // sprintf(dbg_buffer, "\"%s\".within %d, %d vs %d, %d, %d, %d",
            // m_label, x, y, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
    // Serial.println(dbg_buffer);

    return ((x > rect.x)
            && (x < rect.x+rect.w)
            && (y > rect.y)
            && (y < rect.y+rect.h));
}
