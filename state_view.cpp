// state_view.cpp

#include <stdio.h>

#include "state_view.h"
#include "bar.h"

#define SCREEN_W 320
#define SCREEN_H 240

#define UPDATE_INTERVAL 500
#define VALUES_X 148
#define LABELS_X VALUES_X
#define VALUES_Y 40

#define MENU_X 0
#define MENU_HEIGHT 36
#define MENU_Y SCREEN_H-MENU_HEIGHT
#define MENU_WIDTH 80
#define STATE_MENU_ORIENT Menu::O_Horiz

#define R_MARGIN 10

#define TITLE_FONT 4
#define STATE_FONT 4
#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2
#define PLOT_THICKNESS 10
#define VALUES_GAP 10

// #define SCREEN_BG TFT_BLACK
#define VALUE_BG TFT_BLACK
#define VALUE_FG TFT_WHITE

// #define DEBUG_TOUCH

#ifdef DEBUG
void show_x_y_line(const char *what, int x, int y, int line)
{
    char buffer[60];
    sprintf(buffer, "%s line: %d x: %d y: %d", what, line, x, y);
    Serial.println(buffer);
}
#endif

ButtonData state_menu_button_data[] = {
    ButtonData("CFG", TFT_PURPLE, TFT_WHITE, TFT_PURPLE),
    ButtonData("CAL", TFT_GREEN, TFT_BLACK, TFT_GREEN),
    ButtonData("NET", TFT_ORANGE, TFT_WHITE, TFT_ORANGE),
    ButtonData("STOP", TFT_RED, TFT_WHITE, TFT_RED)
};
#define NUM_STATE_BUTTONS (sizeof(state_menu_button_data)/sizeof(state_menu_button_data[0]))

#define NUM_STATE_LINES 4

StateView::StateView(Display &d,
        ViewChangeCallback ccb,
        void *change_user_data,
        Scale &s, BME280_IF &b) :
    View(d, ccb, change_user_data),
    m_display(d),
    m_menu(
            d,
            Rect(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT),
            state_menu_button_data, NUM_STATE_BUTTONS, STATE_MENU_ORIENT),
    m_scale(s),
    m_bme(b),
    m_temp(0.0),
    m_humid(0.0),
    m_weight(0.0),
    m_full_weight(0.0)
{
    m_menu.set_callback(state_menu_callback_func, this);

    TFT_eSPI &tft = m_display.get_tft();
    tft.setTextSize(2);
    m_title_height = tft.fontHeight(STATE_FONT);
}

// All the state menus are for views.  But a menu button could be handled
// differently here, doing something other than telling control to change the view.
void StateView::state_menu_callback(const char *label, bool pressed)
{
    // Serial.print("State menu callback got \"");
    // Serial.print(label),
    // Serial.print("\" ");
    // Serial.println(pressed?"PRESSED":"RELEASED");
    // Signal to the control module that a new view is requested
    if (m_change_cb && !pressed)
        m_change_cb(label, m_change_data);
}

void StateView::state_menu_callback_func(const char *label, bool pressed, void *user_data)
{
    StateView *sv = reinterpret_cast<StateView*>(user_data);
    sv->state_menu_callback(label, pressed);
}

// Show the static part of the view
void StateView::show()
{

    // Serial.println("StateView::show()");
    TFT_eSPI &tft = m_display.get_tft();

    // Fill screen with dark grey
    tft.fillRect(0, 0, tft.width(), tft.height(), SCREEN_BG);

    // Show the view name and our field names
    tft.setTextSize(2);
    int title_height = tft.fontHeight(STATE_FONT);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("FilaScale", tft.width()/2, 0, TITLE_FONT);

    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextDatum(TR_DATUM);

    int width = VALUES_X;
    int field_spacing = (MENU_Y-VALUES_Y)/NUM_STATE_LINES;
    int line = 0;
    int x = VALUES_X-VALUES_GAP;
    // int y = VALUES_Y+(tft.fontHeight(STATE_FONT)/2);
    int y = m_title_height+(tft.fontHeight(STATE_FONT)/2);
    tft.drawString("TEMP", x, y, STATE_FONT);
#ifdef DEBUG
    show_x_y_line("show", x, y, line);
#endif
    ++line;
    y += field_spacing;
    tft.drawString("HUMIDITY", x, y, STATE_FONT);
#ifdef DEBUG
    show_x_y_line("show", x, y, line);
#endif
    ++line;
    y += field_spacing;
    tft.drawString("WEIGHT", x, y, STATE_FONT);
#ifdef DEBUG
    show_x_y_line("show", x, y, line);
#endif

    // Show the buttons
    m_menu.show();

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

void StateView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
#ifdef DEBUG_TOUCH
    Serial.print("StateView got touch callback. x: ");
    Serial.print(x);
    Serial.print(", y: ");
    Serial.println(y);
    Serial.println("checking buttons");
#endif

    m_menu.check_touch(x, y, pressed);
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
#ifdef DEBUG_FIELD_SPACING
    {
        char buffer[70];
        sprintf(buffer, "field_spacing %d = (MENU_Y %d- VALUES_Y %d)/ NUM_STATE_LINES %d",
                (MENU_Y - VALUES_Y)/NUM_STATE_LINES,
                MENU_Y, VALUES_Y, NUM_STATE_LINES);
        Serial.println(buffer);
    }
#endif

    int line = 0;
    int x = VALUES_X;
    int y = m_title_height+(tft.fontHeight(STATE_FONT)/2);
    int bar_y = y+tft.fontHeight(STATE_FONT);

    plot_bar(m_display, Rect(x, bar_y, width, PLOT_THICKNESS),
            m_temp, 0, 50, TFT_ORANGE, TFT_BLUE);
    sprintf(value_buffer, "%3.1f °C  ", m_temp);
    tft.drawString(value_buffer, x, y, STATE_FONT);
    ++line;

    y += field_spacing;
    bar_y = y+tft.fontHeight(STATE_FONT);

    plot_bar(m_display, Rect(x, bar_y, width, PLOT_THICKNESS),
            m_humid, 0, 95, TFT_ORANGE, TFT_BLUE);
    sprintf(value_buffer, "%4.2f %%RH  ", m_humid);
    tft.drawString(value_buffer, x, y, STATE_FONT);
    ++line;

    y += field_spacing;
    bar_y = y+tft.fontHeight(STATE_FONT);

    plot_bar(m_display, Rect(x, bar_y, width, PLOT_THICKNESS),
            m_weight, 0, 200, TFT_ORANGE, TFT_BLUE);
    if (m_weight >= 1000.0)
        sprintf(value_buffer, "%4.3f Kg    ", m_weight/1000.0);
    else
        sprintf(value_buffer, "%4.3f g     ", m_weight);
    tft.drawString(value_buffer, x, y, STATE_FONT);
}
