
#include "dialog.h"
#include "config.h"

#define ADJ_OFFSET 50
#define DEBUG_DIALOG_BASE

Dialog::Dialog(
        Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        uint16_t fg,
        uint16_t bg) :
    m_display(d),
    m_rect(rect),
    m_title(title),
    m_prompt(prompt),
    m_fg(fg),
    m_bg(bg)
{
#ifdef DEBUG_DIALOG_BASE
    static char msg[76];
    if (m_title)
    {
        sprintf(msg, "%s dlg_base rect: [%u, %u, %u, %u]",
                m_title, rect.x, rect.y, rect.w, rect.h);
        Serial.println(msg);
    }
    else
        Serial.println("Dialog got no title!!");
#endif
}


void Dialog::show(void)
{
    Serial.println("Dialog::show()");
    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(
            m_rect.x, m_rect.y,
            m_rect.w, m_rect.h,
            m_bg);
    tft.setTextColor(m_fg);

    // Show the title at the top, center justified
    if (m_title)
    {
        tft.setTextDatum(TC_DATUM);
        tft.drawString(m_title, m_rect.x+(m_rect.w/2), m_rect.y, TITLE_FONT);
    }

    // Show the prompt just below it, in dialog font, left justification
    if (m_prompt)
    {
        tft.setTextDatum(TL_DATUM);
        tft.drawString(m_prompt, m_rect.x+PROMPT_X, m_rect.y+tft.fontHeight(TITLE_FONT), DIALOG_FONT);
    }
}

