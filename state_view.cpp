// state_view.cpp

#include <stdio.h>

#include "state_view.h"

#define UPDATE_INTERVAL 250
#define STATE_Y 40

#define HORIZ_BUTTONS
#ifdef HORIZ_BUTTONS
#define MENU_X 4
#define MENU_Y 190
// #define MENU_HEIGHT 40
#define MENU_HEIGHT 40
// #define MENU_WIDTH 320
#define MENU_WIDTH 80
#define STATE_MENU_ORIENT Menu::O_Horiz
#else
#define MENU_X 4
#define MENU_Y 4
// #define MENU_HEIGHT 120
#define MENU_HEIGHT 40
// #define MENU_WIDTH 96
#define MENU_WIDTH 80
#define STATE_MENU_ORIENT Menu::O_Vert
#endif

#define BUTTON_GAP 2
#define R_MARGIN 10

#define WIDTH 320
#define HEIGHT 240
#define VALUES_X 148
#define STATE_FONT 4
#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2

#define SCREEN_BG TFT_BLACK
#define VALUE_BG TFT_BLACK
#define TEXT_BG TFT_WHITE

// #define DEBUG_TOUCH

ButtonData state_menu_button_data[] = {
    ButtonData("CFG", TFT_PURPLE, TFT_WHITE),
    ButtonData("CAL", TFT_GREEN, TFT_BLACK),
    ButtonData("CONN", TFT_ORANGE, TFT_WHITE),
    ButtonData("STOP", TFT_RED, TFT_WHITE)
};

#define NUM_STATE_BUTTONS (sizeof(state_menu_button_data)/sizeof(state_menu_button_data[0]))

StateView::StateView(Display &d, ViewChangeCallback ccb, void *change_user_data, Scale &s, BME280_IF &b) :
    View(d, ccb, change_user_data),
    m_display(d),
    m_menu(
            d,
            Rect(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT),
            state_menu_button_data, NUM_STATE_BUTTONS, STATE_MENU_ORIENT),
    m_scale(s),
    m_bme(b),
    m_active(false),
    m_temp(0.0),
    m_humid(0.0),
    m_weight(0.0),
    m_full_weight(0.0)
{
    m_menu.set_callback(state_menu_callback_func, this);
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
        m_change_cb(label, m_user_data);
}

void StateView::state_menu_callback_func(const char *label, bool pressed, void *user_data)
{
    StateView *sv = reinterpret_cast<StateView*>(user_data);
    sv->state_menu_callback(label, pressed);
}

// Show the static part of the view
void StateView::show()
{
    Serial.println("StateView::show()");
    TFT_eSPI &tft = m_display.get_tft();

    // Fill screen with dark grey
    tft.fillRect(0, 0, WIDTH, HEIGHT, SCREEN_BG);

    // Show the view name and our field names
    tft.setTextColor(TFT_WHITE);
    int line = 0;
    tft.drawString("Filament Management", MARGIN, tft.fontHeight(STATE_FONT)*line+MARGIN, STATE_FONT);
    ++line;
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("TEMP", MARGIN, tft.fontHeight(STATE_FONT)*line+MARGIN+STATE_Y, STATE_FONT);
    ++line;
    tft.drawString("HUMIDITY", MARGIN, tft.fontHeight(STATE_FONT)*line+MARGIN+STATE_Y, STATE_FONT);
    ++line;
    tft.drawString("WEIGHT", MARGIN, tft.fontHeight(STATE_FONT)*line+MARGIN+STATE_Y, STATE_FONT);

    // Show the buttons
    m_menu.show(m_display);

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

    m_menu.check_touch(&m_display, x, y, pressed);

    // d->calibrate();
}

// Update the state on the screen with the latest values we hold.
// called at UPDATE_INTERVAL
void StateView::draw_state()
{
    TFT_eSPI &tft = m_display.get_tft();
    char value_buffer[45];
    tft.setTextColor(TEXT_BG);
    int line = 1;

    sprintf(value_buffer, "%3.1f°C %3.1f°F", m_temp, (m_temp*9/5.0+32.0));
    int width = tft.width()-VALUES_X-R_MARGIN;
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            VALUE_BG);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y, STATE_FONT);
    ++line;

    sprintf(value_buffer, "%4.2f%%RH", m_humid);
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            VALUE_BG);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y, STATE_FONT);
    ++line;

    sprintf(value_buffer, "%4.2fg", m_weight);
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            VALUE_BG);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y, STATE_FONT);
    ++line;
}

