
#include <Arduino.h>
#include <stdio.h>

#include "button.h"

#define BUTTON_FONT 4
#define BUTTON_RADIUS 10

Button::Button(const Rect &r, int c, const char *l)
    : rect(r), color(c), label(l)
{
    // char buff[74];
    // sprintf(buff, "%s got rect %d %d %d %d",
            // label, rect.x, rect.y, rect.w, rect.h);
    // Serial.println(buff);
}

bool Button::draw(Display &d)
{
    TFT_eSPI &tft = d.get_tft();
    tft.fillRoundRect(rect.x, rect.y, rect.w, rect.h, BUTTON_RADIUS, color);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TL_DATUM);
    int16_t t_width = tft.textWidth(label, BUTTON_FONT);
    int32_t label_x = rect.x+(rect.w/2)-(t_width/2);
    int32_t label_y = rect.y+(rect.h/2);
    tft.drawString(label, label_x, label_y, BUTTON_FONT);
}

bool Button::pressed(uint16_t x, uint16_t y)
{
    // static char buff[72];
    // sprintf(buff, "%s::pressed checking %d, %d against %d, %d, %d, %d",
            // label, x, y, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
    // Serial.println(buff);

    return ((x > rect.x)
            && (x < rect.x+rect.w)
            && (y > rect.y)
            && (y < rect.y+rect.h));
}

