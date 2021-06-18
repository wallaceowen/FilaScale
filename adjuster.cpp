// Adjuster class - presents a slider that can be used to set a value 

#include "config.h"
#include "adjuster.h"

// layout:
// title in ctr
// prompt blow that
// 
//          current
//+---+-----------------+---+
//| - |                 | + |
//+---+-----------------+---+
//

#define BUTTON_WIDTH 20
#define BUTTON_HEIGHT 20
#define LO_BUTTON_X 0

#define BAR_X BUTTON_WIDTH

// FIXME
#define LABELS_Y PROMPT_Y+20
#define LABELS_WIDTH 20
#define LABELS_HEIGHT 20
#define BAR_Y LABELS_Y+LABELS_HEIGHT
#define BUTTONS_Y BAR_Y

#define CURRENT_LABEL_X 

#define CURRENT_LABEL_FONT 4

ButtonData less_button_data("-", TFT_WHITE, TFT_BLACK, TFT_WHITE);
ButtonData more_button_data("+", TFT_WHITE, TFT_BLACK, TFT_WHITE);
LabelData current_label_data("", TFT_WHITE, TFT_BLACK, CURRENT_LABEL_FONT, true);

Adjuster::Adjuster(
        Display &d,
        const Rect &r,
        const char *title,
        const char *prompt,
        const char *v,
        uint16_t min,
        uint16_t max) :
    m_display(d),
    rect(r),
    less_button(less_button_data, Rect(rect.x+BUTTON_WIDTH, LABELS_Y, BUTTON_WIDTH, BUTTON_HEIGHT)),
    more_button(more_button_data, Rect(rect.x+rect.w-BUTTON_WIDTH, BUTTONS_Y, BUTTON_WIDTH, BUTTON_HEIGHT)),
    current_l(current_label_data, Rect(rect.x, LABELS_Y, rect.w-(2*LABELS_WIDTH), LABELS_HEIGHT)),
    m_title(title),
    m_prompt(prompt),
    varname(v),
    inmin(min),
    inmax(max),
    mapper(min, max, rect.x, rect.x+rect.w)
{
}

bool Adjuster::loop(void)
{
}

void Adjuster::show(void)
{
    // Render to the display
    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(
            rect.x, rect.y,
            rect.w, rect.h,
            TFT_WHITE);
    // Show the title
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextDatum(TC_DATUM);
    tft.drawString(m_title, rect.x+(rect.w/2), rect.y, TITLE_FONT);
    // Show the prompt
    tft.setTextDatum(TL_DATUM);
    tft.drawString(m_prompt, rect.x+PROMPT_X, rect.y+PROMPT_Y, DIALOG_FONT);

    // Show the slider
}

// bool Adjuster::check_touch(uint16_t x, uint16_t y, bool pressed)
// {
// }

