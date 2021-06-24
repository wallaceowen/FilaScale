// config_view.cpp

#include <stdio.h>

#include "config_view.h"
#include "stock_buttons.h"

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

#define CONF_OFFER_ROWS 5
#define CONF_OFFER_COLS 2

static ButtonData network_bd[] = {
    ButtonData("IP", TFT_WHITE, TFT_BLUE, TL_DATUM),
    ButtonData("NETMASK", TFT_WHITE, TFT_GREEN, TL_DATUM),
    ButtonData("GATEWAY", TFT_WHITE, TFT_BLUE, TL_DATUM),
    ButtonData("CANCEL", TFT_WHITE, TFT_RED, TL_DATUM),
};
#define NUM_NET_BUTTONS (sizeof(network_bd)/sizeof(network_bd[0]))

#ifdef DEBUG_MENU_CALLBACK
static const char *state_names[] = {
    "COS_Offer",
    "COS_Filament",
    "COS_Screen",
    "COS_Network",
    "COS_NUmStates"
};
#endif

// Here is where we configure our filament temperature and humidity thresholds, for each of
// the types of filament defined
ConfigView::ConfigView(Display &d, ViewChangeCallback ccb, void *change_user_data) :
    View(d, ccb, change_user_data),
    m_display(d),
    m_state(COS_Offer),
    m_offer_config_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT)),
    m_filament_config_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT)),
#ifdef NETCONF_IS_KBD_DIALOG
    m_keypad_dialog(d, Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT), ""),
#else
    m_network_config_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Config Network",
            "Press OK or CANCEL",
            network_bd, NUM_NET_BUTTONS,
            Menu::O_Vert),
#endif

    m_current_dialog(&m_offer_config_dialog)
{
    m_offer_config_dialog.set_callback(menu_callback_func, this);
    m_filament_config_dialog.set_callback(menu_callback_func, this);
#ifdef NETCONF_IS_KBD_DIALOG
    m_keypad_dialog.set_callback(menu_callback_func, this);
#else
    m_network_config_dialog.set_callback(menu_callback_func, this);
#endif

    // Clear the display
    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(0, 0, tft.width(), tft.height(), SCREEN_BG);
}

#ifdef CONF_DILAOG_CB_NEEDED
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
#endif

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

// Deal with a state change (mostly by switching to a different dialog)
void ConfigView::set_state(ConfigState cs)
{
    m_state = cs;
    switch (m_state)
    {
        case COS_Offer:
            m_current_dialog = &m_offer_config_dialog;
            break;
        case COS_Filament:
            m_current_dialog = &m_filament_config_dialog;
           break;
        case COS_Network:
#ifdef NETCONF_IS_KBD_DIALOG
            m_current_dialog = &m_keypad_dialog;
#else
            m_current_dialog = &m_network_config_dialog;
#endif
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
                else if (!strcmp(label, "FILAMENT"))
                {
                    set_state(COS_Filament);
                    this->show();
                }
                else if (!strcmp(label, "NETWORK"))
                {
                    set_state(COS_Network);
                    this->show();
                }
                else if (!strcmp(label, "SCALE"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);

                    // Tell control to make the SCALE view the current view
                    m_change_cb("SCALE", m_change_data);
                }
                else if (!strcmp(label, "SCREEN"))
                {
                    set_state(COS_Screen);
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

            case  COS_Filament:
                if (!strcmp(label, "CANCEL"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);

                    this->show();
                }
                else
                {
                    Serial.println("Do something with this filament config request");
                    // this->handle_filament_cfg_request(label);

                    // Reset state to offer
                    set_state(COS_Offer);
                    this->show();
                }
                break;


            case  COS_Network:

                if (!strcmp(label, "CANCEL"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);

                    this->show();
                }
                else
                {
                    Serial.println("Do something with this network config request");
                    // this->handle_network_cfg_request(label);

                    // Reset state to offer
                    set_state(COS_Offer);
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


