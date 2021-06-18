
#include <Arduino.h>
#include <stdio.h>

#include "label.h"

#ifdef DEBUG
static char dbg_buffer[72];
#endif

LabelData::LabelData(const char *l, uint16_t pfg, uint16_t pbg, uint8_t f, bool c)
    : label(l), bg(pbg), fg(pfg), font(f), center(c)
{
}

Label::Label(const LabelData &d, const Rect &r) :
    m_labeldata(d),
    m_rect(r)
{
#ifdef DEBUG
    sprintf(dbg_buffer, "label \"%s\" created at x %d, y %d, w %d, h %d",
            m_labeldata.label, m_rect.x, m_rect.y, m_rect.w, m_rect.h);
    Serial.println(dbg_buffer);
#endif
}

bool Label::draw(Display &d)
{
    TFT_eSPI &tft = d.get_tft();
    this->draw(tft);
}

bool Label::draw(TFT_eSPI &tft)
{
    int16_t height = tft.fontHeight(m_labeldata.font);
    tft.setTextColor(m_labeldata.fg, m_labeldata.bg);
    int16_t t_width = tft.textWidth(m_labeldata.label, m_labeldata.font);
    tft.drawString(m_labeldata.label, m_rect.x, m_rect.y, m_labeldata.font);

#ifdef DEBUG
    {
        sprintf(dbg_buffer, "drawing label \"%s\" at %d, %d", m_labeldata.label, m_rect.x, m_rect.y);
        Serial.println(dbg_buffer);
    }
#endif
}
