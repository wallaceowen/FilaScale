// config_view.cpp

#include <stdio.h>

#include "config_view.h"
#include "stock_buttons.h"

#define DLG_X 0
#define DLG_Y 0
#define DLG_HEIGHT 240
#define DLG_WIDTH 320
// #define NET_MENU_Y 50

// #define WIDTH 320
// #define HEIGHT 240
// #define VALUES_X 148
// #define CALIB_FONT 4
// #define BORDER_THICKNESS 5
// #define MARGIN BORDER_THICKNESS+2

// #define CONF_OFFER_ROWS 5
// #define CONF_OFFER_COLS 2

// Here is where we configure our filament temperature and humidity thresholds, for each of
// the types of filament defined
ConfigView::ConfigView(Display &d, ViewChangeCallback ccb, void *change_user_data) :
    View(d, ccb, change_user_data),
    m_display(d),
    m_state(COS_Offer),
    m_offer_config_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT)),
    m_screencal_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Calibrate screen?",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS,
            Menu::O_Horiz, TFT_BLACK, TFT_LIGHTGREY),
    m_current_dialog(&m_offer_config_dialog)
{
    m_offer_config_dialog.set_callback(menu_callback_func, this);
    m_screencal_dialog.set_callback(menu_callback_func, this);

    // Clear the display
    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(0, 0, tft.width(), tft.height(), TFT_LIGHTGREY);
}

bool ConfigView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
    return m_current_dialog->check_touch(x, y, pressed);
}

// Callback that is invoked as a side-effect of either the Menu or Buttons class
// being asked to check_touch().
void ConfigView::menu_callback(const char *label, bool pressed)
{
    Serial.print("ConfigView::MenuCallback got ");
    Serial.print(label);
    Serial.print(" ");
    Serial.print(pressed?"pressed":"released");
    Serial.print(" in state ");
    Serial.println(m_state);

    if (!pressed)
    {
        // Here we check m_state to see what state to switch to,
        // then deal with switching to that state
        switch(m_state)
        {
            case COS_Offer:
            {
                if (!strcmp(label, "BACK"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);
                    m_current_dialog = &m_offer_config_dialog;

                    // Tell control to go back to state view
                    m_change_cb("STATE", m_change_data);
                }
                else if (!strcmp(label, "FILAMENT"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);
                    m_current_dialog = &m_offer_config_dialog;

                    // Tell control to go to filament view
                    m_change_cb("FILAMENT", m_change_data);
                }
                else if (!strcmp(label, "NETWORK"))
                {
                    set_state(COS_Offer);
                    m_current_dialog = &m_offer_config_dialog;
                    m_change_cb("NETWORK", m_change_data);
                }
                else if (!strcmp(label, "SCALE"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);
                    m_current_dialog = &m_offer_config_dialog;

                    // Tell control to make the SCALE view the current view
                    m_change_cb("SCALE", m_change_data);
                }
                else if (!strcmp(label, "SCREEN"))
                {
                    set_state(COS_Screen);
                    m_current_dialog = &m_screencal_dialog;
                    m_change_cb("SETTINGS", m_change_data);
                    // this->show();
                }
                else
                {
                    Serial.print("ConfigView::MenuCallback got ");
                    Serial.print(label);
                    Serial.println(" in state COS_Offer");
                }
                break;
            }

            case  COS_Screen:

                if (!strcmp(label, "CANCEL"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);
                    m_current_dialog = &m_offer_config_dialog;
                    m_change_cb("SETTINGS", m_change_data);
                    // this->show();
                }
                else
                {
                    Serial.println("User said yes to SCREEN CAL");

                    m_display.calibrate();

                    // Reset state to offer
                    set_state(COS_Offer);
                    m_current_dialog = &m_offer_config_dialog;
                    m_change_cb("SETTINGS", m_change_data);
                    // this->show();
                }
                break;

            case  COS_Network:

                if (!strcmp(label, "CANCEL"))
                {
                    // Reset state to offer
                    set_state(COS_Offer);
                    m_current_dialog = &m_offer_config_dialog;
                    // this->show();
                    m_change_cb("SETTINGS", m_change_data);
                }
                else
                {
                    Serial.println("Do something with this network config request");
                    // this->handle_network_cfg_request(label);

                    // Reset state to offer
                    set_state(COS_Offer);
                    m_current_dialog = &m_offer_config_dialog;
                    m_change_cb("SETTINGS", m_change_data);
                    // this->show();
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
    // Show the initial dialog
    m_current_dialog->show();
}

bool ConfigView::update()
{
    return false;
}

#define CONFIG_UPDATE_INTERVAL 500
void ConfigView::loop()
{
    static auto scheduled = millis()+CONFIG_UPDATE_INTERVAL;

    auto now = millis();
    if (now > scheduled)
    {
        if (update())
            m_current_dialog->show();
        scheduled = now+CONFIG_UPDATE_INTERVAL;
    }
}


