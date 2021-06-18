// config_view.cpp

#include <stdio.h>

#include "config_view.h"
#include "stock_buttons.h"

#define CALIB_Y 10
#define DLG_X 18
#define DLG_Y 18
#define DLG_HEIGHT 160
#define DLG_WIDTH 270

#define WIDTH 320
#define HEIGHT 240
#define VALUES_X 148
#define CALIB_FONT 4
#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2

#define SCREEN_BG TFT_BLACK

static ButtonData config_offer_bd[] = {
    ButtonData("THRESH", TFT_BLUE, TFT_WHITE),
    ButtonData("NET", TFT_GREEN, TFT_WHITE),
    ButtonData("CANCEL", TFT_RED, TFT_WHITE),
};
#define NUM_CO_BUTTONS (sizeof(config_offer_bd)/sizeof(config_offer_bd[0]))

#define THRESH_ROWS 2
#define  THRESH_COLS 3
static ButtonData thresh_bd[] = {
    ButtonData("WEIGHT", TFT_BLACK, TFT_WHITE),
    ButtonData("PLA", TFT_BLACK, TFT_WHITE),
    ButtonData("ABS", TFT_BLACK, TFT_WHITE),
    ButtonData("Nylon", TFT_BLACK, TFT_WHITE),
    ButtonData("PETG", TFT_BLACK, TFT_WHITE),
    ButtonData("CANCEL", TFT_RED, TFT_WHITE),
};
#define NUM_THRESH_BUTTONS (sizeof(thresh_bd)/sizeof(thresh_bd[0]))

static ButtonData network_bd[] = {
    ButtonData("IP", TFT_BLUE, TFT_WHITE),
    ButtonData("NETMASK", TFT_GREEN, TFT_WHITE),
    ButtonData("GATEWAY", TFT_BLUE, TFT_WHITE),
    ButtonData("CANCEL", TFT_RED, TFT_WHITE),
};
#define NUM_NET_BUTTONS (sizeof(network_bd)/sizeof(network_bd[0]))


static const char *state_names[] = { "COS_Offer", "COS_Thresholds", "COS_Network", "COS_NUmStates" };


void ConfigView::add_threshold_buttons()
{
    for (int i = 0; i < NUM_THRESH_BUTTONS; ++i)
    {
        m_thresh_config_dialog.add_button(thresh_bd[i], i/THRESH_COLS,
                i%THRESH_COLS);
    }

}

// Here is where we configure our filament temperature and humidity thresholds, for each of
// the types of filament defined
ConfigView::ConfigView(Display &d, ViewChangeCallback ccb, void *change_user_data) :
    View(d, ccb, change_user_data),
    m_display(d),
    m_state(COS_Offer),
    m_offer_config_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "CONFIG",
            "Choose an option",
            config_offer_bd, NUM_CO_BUTTONS),
    // m_thresh_config_dialog(
            // d,
            // Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            // "THRESHOLDS",
            // "Select a threshold to adjust",
            // thresh_bd, NUM_THRESH_BUTTONS,
            // Menu::O_Vert),
    m_thresh_config_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "THRESHOLDS",
            "Select a threshold to adjust",
            THRESH_ROWS, THRESH_COLS),
    m_network_config_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Config Network",
            "Press OK or CANCEL",
            network_bd, NUM_NET_BUTTONS,
            Menu::O_Vert),
    m_current_dialog(&m_offer_config_dialog)
{
    add_threshold_buttons();

    m_offer_config_dialog.set_callback(menu_callback_func, this);
    m_thresh_config_dialog.set_callback(menu_callback_func, this);
    m_network_config_dialog.set_callback(menu_callback_func, this);

    // Clear the display
    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(0, 0, WIDTH, HEIGHT, SCREEN_BG);
}

void ConfigView::config_dialog_callback(const char *label, bool pressed)
{
    Serial.print("Config dialog callback got \"");
    Serial.print(label),
    Serial.print("\" ");
    Serial.println(pressed?"PRESSED":"RELEASED");
}

void ConfigView::config_dialog_callback_func(const char *label, bool pressed, void *user_data)
{
    ConfigView *cv = reinterpret_cast<ConfigView*>(user_data);
    cv->config_dialog_callback(label, pressed);
}

void ConfigView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
#ifdef DEBUG_TOUCH
    Serial.print("ConfigView got touch callback. x: ");
    Serial.print(x);
    Serial.print(", y: ");
    Serial.println(y);
    Serial.println("checking buttons");
#endif

    m_current_dialog->check_touch(x, y, pressed);

}

void ConfigView::set_state(ConfigState cs)
{
    m_state = cs;
    switch (m_state)
    {
        case COS_Offer:
            m_current_dialog = &m_offer_config_dialog;
            break;
        case COS_Thresholds:
            m_current_dialog = &m_thresh_config_dialog;
           break;
        case COS_Network:
            m_current_dialog = &m_network_config_dialog;
            break;
        default:
            m_state = COS_Offer;
            m_current_dialog = &m_offer_config_dialog;
           break;
    }
}

void ConfigView::menu_callback(const char *label, bool pressed)
{
    Serial.print("Config menu callback got \"");
    Serial.print(label),
    Serial.print("\" state = ");
    Serial.print(state_names[m_state]),
    Serial.print(" pressed = ");
    Serial.println(pressed?"PRESSED":"RELEASED");

    // enum ConfigState { COS_Offer, COS_Thresholds, COS_Network, COS_NUmStates };
    // Here we check m_state to see what state to switch to,
    // then deal with switching to that state
    if (!pressed)
    {
        TFT_eSPI &tft = m_display.get_tft();
        tft.fillRect(0, 0, tft.width(), tft.height(), SCREEN_BG);
        if (!strcmp(label, "CANCEL"))
        {
            // Reset state to offer
            set_state(COS_Offer);

            // Tell control to go back to state view
            m_change_cb("CANCEL", m_change_data);
        }
        else if (!strcmp(label, "THRESH"))
        {
            set_state(COS_Thresholds);
        }
        else if (!strcmp(label, "NET"))
        {
            set_state(COS_Network);
        }
        this->show();
    }
}

void ConfigView::menu_callback_func(const char *label, bool pressed, void *user_data)
{
    ConfigView *cv = reinterpret_cast<ConfigView*>(user_data);
    cv->menu_callback(label, pressed);
}


// Show the static part of the view
void ConfigView::show()
{
    Serial.println("ConfigView::show()");
    TFT_eSPI &tft = m_display.get_tft();

    // Show the initial dialog
    m_current_dialog->show();
}

bool ConfigView::update()
{
    Serial.println("ConfigView::update()");
    return true;
}

void ConfigView::loop()
{
#ifdef CALIBVIEW_LOOP_HAS_A_JOB
    Serial.println("ConfigView::loop()");
    TFT_eSPI &tft = m_display.get_tft();
    switch(m_state)
    {
        case COS_Offer:
            break;

        case  COS_Thresholds:
            break;

        case  COS_Filament:
            break;

        default:
            break;
    }
#endif
}


