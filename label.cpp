
#include <Arduino.h>
#include <stdio.h>

#include "label.h"

// #define DEBUG_NEWLABEL_DRAW
// #define DEBUG_LABEL_CTOR

#if defined(DEBUG_NEWLABEL_DRAW) || defined(DEBUG_LABEL_CTOR)
static char dbg_buffer[72];
#endif

LabelData::LabelData(const char *l, uint16_t pfg, uint16_t pbg, uint8_t f, bool c)
    : fg(pfg), bg(pbg), font(f), center(c)
{
    memset(label, 0, LABEL_MAX);
    strncpy(label, l, strlen(l) < LABEL_MAX-1?strlen(l):LABEL_MAX-1);
}

Label::Label(const LabelData &d, const Rect &r) :
    m_labeldata(d),
    m_rect(r)
{
#ifdef DEBUG_LABEL_CTOR
    // int16_t t_width = tft.textWidth(m_labeldata.label, m_labeldata.font);
    // int16_t height = tft.fontHeight(m_labeldata.font);
    // sprintf(dbg_buffer, "label \"%s\" created at x %d, y %d, w %d, h %d",
            // m_labeldata.label, m_rect.x, m_rect.y, width, height);
    sprintf(dbg_buffer, "new label \"%s\" created at x %u, y %u",
            m_labeldata.label, m_rect.x, m_rect.y);
    Serial.println(dbg_buffer);
#endif
}

void Label::draw(Display &d)
{
    TFT_eSPI &tft = d.get_tft();
    this->draw(tft);
}

void Label::set_label(const char *txt)
{
    // m_labeldata.label = txt;
    memset(m_labeldata.label, 0, LABEL_MAX);
    strncpy(m_labeldata.label, txt, strlen(txt) < LABEL_MAX-1?strlen(txt):LABEL_MAX-1);
}

void Label::draw(TFT_eSPI &tft)
{
    tft.setTextColor(m_labeldata.fg, m_labeldata.bg);
    int16_t t_width = tft.textWidth(m_labeldata.label, m_labeldata.font);
    int16_t t_height = tft.fontHeight(m_labeldata.font);
    if (m_labeldata.center)
    {
        tft.setTextDatum(TC_DATUM);
        tft.drawString(m_labeldata.label,
                m_rect.x+m_rect.w/2-t_width/2,
                m_rect.y+m_rect.h/2-t_height/2,
                m_labeldata.font);
#ifdef DEBUG_NEWLABEL_DRAW
        {
            sprintf(dbg_buffer, "drawing new label \"%s\" centered at %d, %d", m_labeldata.label,
                    m_rect.x+m_rect.w/2-t_width/2,
                    m_rect.y+m_rect.h/2-t_height/2
                    );
            Serial.println(dbg_buffer);
        }
#endif
        tft.setTextDatum(TL_DATUM);
    }
    else
    {
        tft.setTextDatum(TL_DATUM);
        tft.drawString(m_labeldata.label, m_rect.x, m_rect.y, m_labeldata.font);
#ifdef DEBUG_NEWLABEL_DRAW
        {
            sprintf(dbg_buffer, "drawing new label \"%s\" at %d, %d", m_labeldata.label, m_rect.x, m_rect.y);
            Serial.println(dbg_buffer);
        }
#endif
    }
}

