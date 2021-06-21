// Adjuster class - presents a slider that can be used to set a value 

#include "config.h"
#include "adjuster.h"

// layout:
// title in ctr
// prompt below that
// 
//          current
//+---+-----------------+---+
//| - |                 | + |
//+---+-----------------+---+
//

#define BUTTON_WIDTH 20
#define BUTTON_HEIGHT 20

#define BAR_X BUTTON_WIDTH

// FIXME
#define LABELS_Y PROMPT_Y+20
#define LABELS_WIDTH 20
#define LABELS_HEIGHT 16
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
        const char *t,
        const char *p,
        const char *v,
        uint16_t min,
        uint16_t max) :
    m_display(d),
    m_tft(d.get_tft()),
    m_rect(r),
    m_current_l(current_label_data, Rect(m_rect.x+LABELS_WIDTH, LABELS_Y, m_rect.w-LABELS_WIDTH, LABELS_HEIGHT)),
    m_less_button(less_button_data, Rect(m_rect.x, BUTTONS_Y, BUTTON_WIDTH, BUTTON_HEIGHT)),
    m_slider(d,
            Rect(m_rect.x+BAR_X,
                // m_rect.y+LABELS_Y+m_tft.fontHeight(DIALOG_FONT),
                BUTTONS_Y,
                m_rect.w-(2*BUTTON_WIDTH),
                SLIDER_HEIGHT)),
    m_more_button(more_button_data, Rect(m_rect.x+m_rect.w-BUTTON_WIDTH, BUTTONS_Y, BUTTON_WIDTH, BUTTON_HEIGHT)),
    m_title(t),
    m_prompt(p),
    m_varname(v),
    m_inmap(min, max, m_rect.x, m_rect.x+m_rect.w),
    m_outmap(m_rect.x, m_rect.x+m_rect.w, min, max),
    m_inmin(min),
    m_inmax(max),
    value(0)
{
    this->show();
}


bool Adjuster::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    if (m_less_button.within(x, y))
    {
        Serial.println("less button within");
        if (value > m_inmin)
            --value;

        // uint16_t mapped = m_inmap.map(value);
        update_bar(value);
        return 1;
    }
    else if (m_more_button.within(x, y))
    {
        Serial.println("more button within");
        if (value < m_inmax)
            ++value;

        // uint16_t mapped = m_inmap.map(value);
        update_bar(value);
        return 1;
    }

    else if (m_slider.within(x, y))
    {
        Serial.println("slider within");
        return 1;
    }

    else
    {
        Serial.println("Adjuster: nothing within.");
        return 0;
    }
    return 0;
}

void Adjuster::update_bar(uint16_t plotval)
{
    // m_tft.fillRect(m_rect.x, m_rect.y, plotval, m_rect.h, incolor);
    // m_tft.fillRect(m_rect.x+plotval, m_rect.y, m_rect.w-plotval, m_rect.h, outcolor);
    // m_tft.drawRect(m_rect.x, m_rect.y, m_rect.w, m_rect.h-1, TFT_WHITE);
    m_slider.set(plotval);
}

void Adjuster::loop(void)
{
}


uint16_t Adjuster::get_value(void) const
{
    uint16_t mapped = m_outmap.map(value);
    return mapped;
}

void Adjuster::show(void)
{
    // Render to the display
    m_tft.fillRect(
            m_rect.x, m_rect.y,
            m_rect.w, m_rect.h,
            TFT_WHITE);
    // Show the title
    m_tft.setTextColor(TFT_BLACK, TFT_WHITE);
    m_tft.setTextDatum(TC_DATUM);
    m_tft.drawString(m_title, m_rect.x+(m_rect.w/2), m_rect.y, TITLE_FONT);
    // Show the prompt
    m_tft.setTextDatum(TL_DATUM);
    m_tft.drawString(m_prompt, m_rect.x+PROMPT_X, m_rect.y+PROMPT_Y, DIALOG_FONT);

    Serial.println("Adjuster: drawing labels and buttons.");
    m_current_l.draw(m_tft);
    m_less_button.draw(m_tft);
    // Show the slider
    m_slider.show();
    m_more_button.draw(m_tft);
}

// bool Adjuster::check_touch(uint16_t x, uint16_t y, bool pressed)
// {
// }

