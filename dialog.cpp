
#include "dialog.h"
#include "config.h"

#define TEXT_DAILOG_FONT 4

#define ADJ_OFFSET 50

Dialog::Dialog(
        Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt_txt,
        uint16_t fg,
        uint16_t bg) :
    m_display(d),
    m_rect(rect),
    m_title(title),
    m_prompt(
            d,
            Rect(
                m_rect.x+PROMPT_X,
                m_rect.y+PROMPT_Y,
                rect.w-PROMPT_X,
                rect.h-PROMPT_Y),
            TEXT_DAILOG_FONT,
            prompt_txt),
    m_fg(fg),
    m_bg(bg)
{
}


void Dialog::show(void)
{
    Serial.print(m_title);
    Serial.println(" Dialog::show( )");
    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(
            m_rect.x, m_rect.y,
            m_rect.w, m_rect.h,
            m_bg);

    // Show the title at the top, center justified
    if (m_title)
    {
        tft.setTextColor(m_fg, m_bg);
        tft.setTextDatum(TC_DATUM);

        // {
            // static char buff[90];
            // sprintf(buff, "TITLE %s at %hu, %hu",
                // m_title, m_rect.x+(m_rect.w/2), m_rect.y);
            // Serial.println(buff);
        // }

        tft.drawString(m_title, m_rect.x+(m_rect.w/2), m_rect.y, TITLE_FONT);
        tft.setTextDatum(TL_DATUM);
    }

    m_prompt.show();
}

