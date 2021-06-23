// config_view.cpp

#include <stdio.h>

#include "config_view.h"
#include "stock_buttons.h"

#define CALIB_Y 10
#define DLG_X 0
#define DLG_Y 0
#define DLG_HEIGHT 240
#define DLG_WIDTH 320

#define WIDTH 320
#define HEIGHT 240
#define VALUES_X 148
#define CALIB_FONT 4
#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2

#define SCREEN_BG TFT_BLACK

#define DEBUG_MENU_CALLBACK

static ButtonData config_offer_bd[] = {
    ButtonData("FIL", TFT_WHITE, TFT_BLUE),
    ButtonData("SCAL", TFT_WHITE, TFT_GREEN),
    ButtonData("SCR", TFT_WHITE, TFT_BLUE),
    ButtonData("NET", TFT_WHITE, TFT_GREEN),
    ButtonData("CANC", TFT_WHITE, TFT_RED),
};
#define NUM_CO_BUTTONS (sizeof(config_offer_bd)/sizeof(config_offer_bd[0]))

#define THRESH_ROWS 2
#define  THRESH_COLS 3
static ButtonData thresh_bd[] = {
    ButtonData("PLA", TFT_BLACK, TFT_YELLOW),
    ButtonData("ABS", TFT_WHITE, TFT_DARKCYAN),
    ButtonData("ASA", TFT_WHITE, TFT_DARKCYAN),
    ButtonData("Nylon", TFT_WHITE, TFT_MAROON),
    ButtonData("PETG", TFT_WHITE, TFT_DARKGREY),
    ButtonData("CANCEL", TFT_WHITE, TFT_RED),
};
#define NUM_THRESH_BUTTONS (sizeof(thresh_bd)/sizeof(thresh_bd[0]))

static ButtonData network_bd[] = {
    ButtonData("IP", TFT_WHITE, TFT_BLUE),
    ButtonData("NETMASK", TFT_WHITE, TFT_GREEN),
    ButtonData("GATEWAY", TFT_WHITE, TFT_BLUE),
    ButtonData("CANCEL", TFT_WHITE, TFT_RED),
};
#define NUM_NET_BUTTONS (sizeof(network_bd)/sizeof(network_bd[0]))


#ifdef DEBUG_MENU_CALLBACK
static const char *state_names[] = { "COS_Offer", "COS_Thresholds", "COS_Network", "COS_NUmStates" };
#endif


void ConfigView::add_threshold_buttons()
{
    for (int i = 0; i < NUM_THRESH_BUTTONS; ++i)
    {
        uint16_t row = i/THRESH_COLS;
        uint16_t column = i%THRESH_COLS;
        m_thresh_config_dialog.add_button(thresh_bd[i], row, column);
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
    m_keypad_dialog(d, Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT), ""),

    m_current_dialog(&m_offer_config_dialog)
{
    add_threshold_buttons();

    m_offer_config_dialog.set_callback(menu_callback_func, this);
    m_thresh_config_dialog.set_callback(menu_callback_func, this);
    m_network_config_dialog.set_callback(menu_callback_func, this);
    m_keypad_dialog.set_callback(menu_callback_func, this);

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
            // m_current_dialog = &m_network_config_dialog;
            m_current_dialog = &m_keypad_dialog;
            Serial.println("keypad dialog selected");
            break;
        default:
            m_state = COS_Offer;
            m_current_dialog = &m_offer_config_dialog;
           break;
    }
}

// Callback that is invoked as a side-effect of either the Menu or Buttons class
// being asked to check_touch().
void ConfigView::menu_callback(const char *label, bool pressed)
{
#ifdef DEBUG_MENU_CALLBACK
    Serial.print("Config menu callback got \"");
    Serial.print(label),
    Serial.print("\" state = ");
    Serial.print(state_names[m_state]),
    Serial.print(" pressed = ");
    Serial.println(pressed?"PRESSED":"RELEASED");
#endif

    // Only check the state when button released
    if (!pressed)
    {
        // Here we check m_state to see what state to switch to,
        // then deal with switching to that state
        switch(m_state)
        {
            case COS_Offer:
            {
                TFT_eSPI &tft = m_display.get_tft();
                tft.fillRect(0, 0, tft.width(), tft.height(), SCREEN_BG);
                if (!strcmp(label, "CANCEL"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);

                    // Tell control to go back to state view
                    m_change_cb("STATE", m_change_data);
                }
                else if (!strcmp(label, "FIL"))
                {
                    set_state(COS_Thresholds);
                    this->show();
                }
                else if (!strcmp(label, "NET"))
                {
                    set_state(COS_Network);
                    this->show();
                }
                else if (!strcmp(label, "SCR"))
                {
                    set_state(COS_Network);
                    this->show();
                }
                else
                {
                    Serial.print("ConfigView::MenuCallback got ");
                    Serial.print(label);
                    Serial.println(" in state COS_Offer");
                }
                break;
            }

            case  COS_Thresholds:
                if (!strcmp(label, "CANCEL"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);

                    // Tell control to go back to state view
                    m_change_cb("STATE", m_change_data);
                }
                else
                {
                    Serial.print("ConfigView::MenuCallback got ");
                    Serial.print(label);
                    Serial.println(" in state COS_Thresholds");
                    Serial.println("Add code to handle Thresh selections");
                    this->show();
                }
                break;

            case  COS_Network:
                if (!strcmp(label, "CANCEL"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);

                    // Tell control to go back to state view
                    m_change_cb("STATE", m_change_data);
                }
                if (!strcmp(label, "ENTER"))
                {
                    Serial.println("Network dialog returned ENTER");

                    // Reset state to offer
                    set_state(COS_Offer);
                    this->show();
                }
                else
                {
                    Serial.println("Add code to handle Network selections");
                    Serial.print("ConfigView::MenuCallback got ");
                    Serial.print(label);
                    Serial.println(" in state COS_Network");
                    this->show();
                }
                break;

            default:
                break;
        }

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
    // TFT_eSPI &tft = m_display.get_tft();

    // Show the initial dialog
    m_current_dialog->show();
    Serial.println("ConfigView::show() done.");
}

bool ConfigView::update()
{
    // Serial.println("ConfigView::update()");
    return false;
}

#define CONFIG_UPDATE_INTERVAL 500
void ConfigView::loop()
{
#ifdef CALIBVIEW_LOOP_HAS_A_JOB
    Serial.println("ConfigView::loop()");
    // TFT_eSPI &tft = m_display.get_tft();
    switch(m_state)
    {
        case COS_Offer:
            break;

        case  COS_Thresholds:
            break;

        case  COS_Network:
            break;

        default:
            break;
    }
#endif
    static auto scheduled = millis()+CONFIG_UPDATE_INTERVAL;

    auto now = millis();
    if (now > scheduled)
    {
        if (update())
            m_current_dialog->show();
        scheduled = now+CONFIG_UPDATE_INTERVAL;
    }

}


