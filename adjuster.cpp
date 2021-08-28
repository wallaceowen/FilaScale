// Adjuster class - presents a slider that can be used to set a value 

#include "display_config.h"
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

#define BUTTON_WIDTH 40
#define BUTTON_HEIGHT 30

#define BAR_X BUTTON_WIDTH

// FIXME
#define BAR_Y PROMPT_Y+20
#define LABELS_WIDTH 20
#define LABELS_HEIGHT 16
#define BUTTONS_Y BAR_Y+SLIDER_HEIGHT
#define LABELS_Y BUTTONS_Y+LABELS_HEIGHT
#define VARNAME_GAP 40

#define VARNAME_WIDTH 100
#define CURRENT_WIDTH 60

#define CURRENT_LABEL_FONT 4
#define ADJUSTER_BG TFT_BLACK

ButtonData less_button_data("-", TFT_WHITE, TFT_DARKGREEN, TL_DATUM);
ButtonData more_button_data("+", TFT_WHITE, TFT_DARKGREEN, TL_DATUM);
LabelData varname_label_data("", TFT_BLACK, TFT_WHITE, CURRENT_LABEL_FONT, false);
LabelData current_label_data("", TFT_BLACK, TFT_WHITE, CURRENT_LABEL_FONT, false);

Adjuster::Adjuster(
        Display &d,
        const Rect &r,
        const char *v,
        uint16_t min,
        uint16_t max) :
    m_display(d),
    m_tft(d.get_tft()),
    m_rect(r),
    m_slider(d,
            Rect(m_rect.x,
                m_rect.y+BAR_Y,
                m_rect.w,
                SLIDER_HEIGHT)),
    m_less_button(less_button_data,
            Rect(
                m_rect.x,
                m_rect.y+BAR_Y+SLIDER_HEIGHT,
                BUTTON_WIDTH, BUTTON_HEIGHT)),
    m_varname_l(varname_label_data,
            Rect(
                m_rect.x+BUTTON_WIDTH,
                m_rect.y+BAR_Y+SLIDER_HEIGHT,
                (m_rect.w-(BUTTON_WIDTH/2))/2,
                LABELS_HEIGHT)),
    m_current_l(current_label_data,
            Rect(
                m_rect.x+BUTTON_WIDTH+VARNAME_WIDTH+VARNAME_GAP,
                m_rect.y+BAR_Y+SLIDER_HEIGHT,
                (m_rect.w-(BUTTON_WIDTH/2))/2,
                LABELS_HEIGHT)),
    m_more_button(more_button_data,
            Rect(
                m_rect.x+m_rect.w-BUTTON_WIDTH,
                m_rect.y+BAR_Y+SLIDER_HEIGHT,
                BUTTON_WIDTH, BUTTON_HEIGHT)),
    m_varname(v),
    // Map from app to display
    m_inmap("in", min, max, m_rect.x, m_rect.x+m_rect.w),
    // Map from display to app
    m_outmap("out", m_rect.x, m_rect.x+m_rect.w, min, max),
    m_inmin(min),
    m_inmax(max),
    m_value(0)
{
}

bool Adjuster::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    if (m_less_button.within(x, y))
    {
        if (m_value > m_inmin)
            --m_value;

        // update_bar(m_outmap.map(m_value));
        update_bar(m_value);
        return 1;
    }
    else if (m_more_button.within(x, y))
    {
        if (m_value < m_inmax)
            ++m_value;

        update_bar(m_value);
        return 1;
    }

    else if (m_slider.within(x, y))
    {
        m_value = m_slider.value_at_x(x);
        update_bar(m_value);

        return 1;
    }

    else
    {
        // Serial.println("Adjuster: nothing within.");
        return 0;
    }
    return 0;
}
void Adjuster::update_bar(uint16_t plotval)
{
    m_slider.set(plotval);
    char plotlabel[16];
    sprintf(plotlabel, "%u    ", m_outmap.map(plotval));
    m_current_l.set_label(plotlabel);
    m_current_l.draw(m_tft);
}

void Adjuster::loop(void)
{
}


uint16_t Adjuster::get_value(void) const
{
    uint16_t mapped = m_outmap.map(m_value);
    return mapped;
}

void Adjuster::show(void)
{
    // Render to the display
    m_tft.fillRect(
            m_rect.x, m_rect.y,
            m_rect.w, m_rect.h,
            ADJUSTER_BG);
    m_varname_l.set_label(m_varname);
    m_current_l.draw(m_tft);
    m_varname_l.draw(m_tft);
    m_less_button.draw(m_tft);

    // Show the slider
    m_slider.show();
    m_more_button.draw(m_tft);
}

