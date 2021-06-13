// state_view.cpp

#include <stdio.h>

#include "state_view.h"

#define UPDATE_INTERVAL 250
#define STATE_Y 40
#define BUTTONS_Y 190
#define BUTTONS_HEIGHT 40
#define BUTTON_GAP 2
#define BUTTONS_START 4
#define R_MARGIN 10

uint16_t button_colors[NUM_BUTTONS] = {TFT_PURPLE, TFT_BLUE, TFT_RED };
const char *button_labels[NUM_BUTTONS] = {"Scale", "Drying", "STOP" };

ButtonData state_menu_button_data[] = {
    ButtonData("SETUP", TFT_PURPLE, TFT_WHITE),
    ButtonData("REPORT", TFT_ORANGE, TFT_WHITE),
    ButtonData("STOP", TFT_RED, TFT_WHITE)
};

#define NUM_STATE_BUTTONS 3
#define STATE_MENU_ORIENT Menu::O_Horiz

StateView::StateView(Display &d, Scale &s, BME280_IF &b) :
    View(d),
    m_display(d),
    m_menu(
            d,
            Rect(BUTTONS_START, BUTTONS_Y, m_display.get_tft().width(), BUTTONS_HEIGHT),
            state_menu_button_data, NUM_STATE_BUTTONS, STATE_MENU_ORIENT),
    m_scale(s),
    m_bme(b),
    m_active(false),
    m_temp(0.0),
    m_humid(0.0),
    m_weight(0.0),
    m_full_weight(0.0)
{
}

// Show the static part of the view
void StateView::show()
{
    Serial.println("StateView::show()");
    TFT_eSPI &tft = m_display.get_tft();

    // Fill screen with dark grey
    tft.fillRect(0, 0, WIDTH, HEIGHT, TFT_DARKGREY);

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

void StateView::touch_callback(Display *d, uint16_t x, uint16_t y, bool pressed)
{
#ifdef DEBUG_TOUCH
    Serial.print("StateView got touch callback. x: ");
    Serial.print(x);
    Serial.print(", y: ");
    Serial.println(y);

    Serial.println("checking buttons");
#endif

    m_menu.check_touch(d, x, y, pressed);

    // d->calibrate();
}

// Update the state on the screen with the latest values we hold.
// Only called when the state has changed.
void StateView::draw_state()
{
    TFT_eSPI &tft = m_display.get_tft();
    char value_buffer[40];
    // tft.setTextColor(TFT_DARKGREEN);
    tft.setTextColor(TFT_BLACK);
    int line = 1;

    sprintf(value_buffer, "%3.1fC %3.1fF", m_temp, (m_temp*9/5.0+32.0));
    int width = tft.width()-VALUES_X-R_MARGIN;
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            TFT_LIGHTGREY);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y, STATE_FONT);
    ++line;

    sprintf(value_buffer, "%4.2f%%RH", m_humid);
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            TFT_LIGHTGREY);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y, STATE_FONT);
    ++line;

    sprintf(value_buffer, "%4.2fg", m_weight);
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            TFT_LIGHTGREY);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS+STATE_Y, STATE_FONT);
    ++line;
}

