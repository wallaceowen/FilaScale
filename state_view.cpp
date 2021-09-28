// state_view.cpp

#include <stdio.h>

#include "state_view.h"
#include "bar.h"

#define SCREEN_H 240

#define UPDATE_INTERVAL 500
#define VALUES_X 148
#define VALUES_Y 50
#define LABELS_Y VALUES_Y+16

#define MENU_X 0
#define MENU_HEIGHT 40
#define MENU_Y SCREEN_H-MENU_HEIGHT
#define PLOT_THICKNESS 10
#define VALUES_GAP 10
#define STATE_BG TFT_DARKGREY
// #define VALUE_BG TFT_BLACK
#define VALUE_BG STATE_BG
#define TITLE_FG TFT_WHITE
#define VALUE_FG TFT_BLACK


#define NUM_STATE_LINES 4

StateView::StateView(Display &d,
        FilaConfig *fc,
        ViewChangeCallback ccb,
        void *change_user_data,
        Scale &s, BME280_IF &b) :
    View(d, fc, ccb, change_user_data),
    m_display(d),
    m_scale(s),
    m_bme(b),
    m_temp(0.0),
    m_humid(0.0),
    m_weight(0.0),
    m_full_weight(0.0),
    m_tag(0llu)
{
    m_title_height = m_display.get_tft().fontHeight(TITLE_FONT);
}

// All the state menus are for views.  But a menu button could be handled
// differently here, doing something other than telling control to change the view.
void StateView::state_menu_callback(const char *label, bool pressed)
{
    // Signal to the control module that a new view is requested
    if (m_change_cb && !pressed)
        m_change_cb(label, m_change_data);
}

void StateView::set_tag(uint64_t &tag)
{
    m_tag = tag;
}

// Show the static part of the view
void StateView::show()
{

    TFT_eSPI &tft = m_display.get_tft();

    // Fill screen with dark grey
    tft.fillRect(0, 0, tft.width(), tft.height(), STATE_BG);

    // Show the view name and our field names
    tft.setTextSize(2);
    tft.setTextColor(TITLE_FG, VALUE_BG);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("FilaScale", tft.width()/2, 0, TITLE_FONT);

    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW, STATE_BG);
    tft.setTextDatum(CR_DATUM);
    int field_spacing = (MENU_Y-VALUES_Y)/NUM_STATE_LINES;
    int line = 0;
    int x = VALUES_X-VALUES_GAP;
    int y = LABELS_Y+(tft.fontHeight(STATE_FONT)/2);
    tft.drawString("TEMP", x, y, STATE_FONT);
    ++line;
    y += field_spacing;
    tft.drawString("HUMIDITY", x, y, STATE_FONT);
    ++line;
    y += field_spacing;
    tft.drawString("WEIGHT", x, y, STATE_FONT);
    ++line;
    y += field_spacing;
    tft.drawString("TOKEN", x, y, STATE_FONT);


    // Now show the state
    draw_state();
}

// Update the state we render.  Side-effect: maintain changed,
// used to know whether to update the gui from the updated values
bool StateView::update(void)
{
    bool changed = false;
    if (m_scale.get_calibrated() != m_weight)
    {
        m_weight = m_scale.get_calibrated();
        changed = true;
    }
    if (m_bme.temp() != m_temp)
    {
        m_temp = m_bme.temp();
        changed = true;
    }
    if (m_bme.humid() != m_humid)
    {
        m_humid = m_bme.humid();
        changed = true;
    }
    return changed;
}

// Call this often.  It only does something every UPDATE_INTERVAL.
void StateView::loop()
{
    static auto scheduled = millis()+UPDATE_INTERVAL;

    auto now = millis();
    if (now > scheduled)
    {
        update();
        draw_state();
        scheduled = now+UPDATE_INTERVAL;
    }
}

bool StateView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
    state_menu_callback("SETTINGS", pressed);
    return 1;
}

// Update the state on the screen with the latest values we hold.
// called at UPDATE_INTERVAL
void StateView::draw_state()
{
    TFT_eSPI &tft = m_display.get_tft();
    char value_buffer[45];
    tft.setTextColor(VALUE_FG, VALUE_BG);
    tft.setTextPadding(12);

    int width = tft.width()-VALUES_X;
    int field_spacing = (MENU_Y-VALUES_Y)/NUM_STATE_LINES;

    int line = 0;
    int x = VALUES_X;
    tft.setTextDatum(TL_DATUM);
    int y = VALUES_Y;

    float f = m_temp*9.0/5.0+32.0;
    sprintf(value_buffer, "%3.1fC %3.1fF  ", m_temp, f);
    tft.drawString(value_buffer, x, y, STATE_FONT);

    int bar_y = y+tft.fontHeight(STATE_FONT);
    plot_bar(m_display, Rect(x, bar_y, width, PLOT_THICKNESS),
            m_temp, 0, 50, TFT_ORANGE, TFT_BLUE);
    ++line;
    y += field_spacing;

    sprintf(value_buffer, "%4.2f %%RH  ", m_humid);
    tft.drawString(value_buffer, x, y, STATE_FONT);

    bar_y = y+tft.fontHeight(STATE_FONT);
    plot_bar(m_display, Rect(x, bar_y, width, PLOT_THICKNESS),
            m_humid, 0, 95, TFT_ORANGE, TFT_BLUE);

    ++line;
    y += field_spacing;
    if (m_weight >= 1000.0)
        sprintf(value_buffer, "%4.3f Kg    ", m_weight/1000.0);
    else
        sprintf(value_buffer, "%4.3f g     ", m_weight);
    tft.drawString(value_buffer, x, y, STATE_FONT);

    bar_y = y+tft.fontHeight(STATE_FONT);
    plot_bar(m_display, Rect(x, bar_y, width, PLOT_THICKNESS),
            m_weight, 0,
            static_cast<uint16_t>(CAL_WEIGHT_GRAMS),
            TFT_ORANGE, TFT_BLUE);

    ++line;
    y += field_spacing;
    y += tft.fontHeight(STATE_FONT);
    sprintf(value_buffer, "%llx ", m_tag);
    tft.drawString(value_buffer, x, y, STATE_FONT);
}
