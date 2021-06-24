
#include "thresh_dialog.h"

#define ADJ_OFFSET 50

ThreshDialog::ThreshDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        uint16_t rows, uint16_t columns) :
    GridDialog(d, rect, title, prompt, TFT_WHITE, TFT_BLACK, rows, columns, 180),
    m_adj(d, Rect(rect.x, rect.y+ADJ_OFFSET, rect.w, 40), "VARNAME", 20, 500)
{
}

bool ThreshDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    bool button_touched = this->GridDialog::check_touch(x, y, pressed)?1:0;
    // Serial.print("GridDialog::check_touch(): ");
    // Serial.println(r?1:0);
    bool slider_touched = m_adj.check_touch(x, y, pressed)?1:0;
    // Serial.print("ThreshDialog::check_touch(): ");
    // Serial.println(r?1:0);
    if (slider_touched && pressed)
    {
        bool result = false;
        TFT_eSPI &tft = m_display.get_tft();
        do
        {
            uint16_t x, y;
            result = tft.getTouch(&x, &y);
            if (result)
                m_adj.check_touch(x, y, pressed);
        } while (result);
    }
    return button_touched || slider_touched;
}

void ThreshDialog::show(void)
{
    this->GridDialog::show();

    // Show the adjuster
    m_adj.show();
}
