#include "config_dialog.h"

#include <string.h>

// Top-level config choices
static GridButtonData config_offer_gbd[] = {
    {GridButtonData("FILAMENT", 0, 0, 1, 1, TFT_WHITE, TFT_BROWN)},
    {GridButtonData("SCALE",    1, 0, 1, 1, TFT_WHITE, TFT_GREEN)},
    {GridButtonData("SCREEN",   2, 0, 1, 1, TFT_WHITE, TFT_ORANGE)},
    {GridButtonData("NETWORK",  3, 0, 1, 1, TFT_WHITE, TFT_PURPLE)},
    {GridButtonData("CANCEL",   4, 0, 1, 1, TFT_WHITE, TFT_RED)},
};
#define NUM_CO_BUTTONS (sizeof(config_offer_gbd)/sizeof(config_offer_gbd[0]))


ConfigDialog::ConfigDialog(Display &d, const Rect &rect) :
    GridDialog(d, rect, "Config", "", TFT_LIGHTGREY, TFT_BLACK, NUM_CO_BUTTONS, 2, 60),
    m_cb(0),
    m_user_data(0)
{
    this->add_buttons();

    m_buttons.set_callback(config_button_callback_func, this);
}

// This will save the ptr to the callback func that's in the client.
// We're installing ConfigDialog::menu_callback_func to be invoked by the
// buttons instance, and our func wiil in turn fire off the callback
// we stash here.
void ConfigDialog::set_callback(PressEventCB cb, void*user_data)
{
    m_cb = cb;
    m_user_data = user_data;
}

void ConfigDialog::add_buttons()
{
    for (int i = 0; i < NUM_CO_BUTTONS; ++i)
        this->add_grid_button(config_offer_gbd[i]);
}

bool ConfigDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    int button_touched = this->GridDialog::check_touch(x, y, pressed)?1:0;
    return button_touched?true:false;
}

void ConfigDialog::show(void)
{
    this->GridDialog::show();
}

// Deal with the buttons that get pressed.
void ConfigDialog::config_button_callback(const char *label, bool pressed)
{
    Serial.print("ConfDialog::config_button_callback got \"");
    Serial.print(label);
    Serial.print("\", ");
    Serial.println(pressed?"PRESSED":"RELEASED");
    if (m_cb)
        m_cb(label, pressed, m_user_data);

}

void ConfigDialog::config_button_callback_func(const char *label, bool pressed, void *user_data)
{
    ConfigDialog *cd = reinterpret_cast<ConfigDialog*>(user_data);
    cd->config_button_callback(label, pressed);
}
