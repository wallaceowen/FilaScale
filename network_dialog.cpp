
#include "network_dialog.h"

#define NETWORK_ROWS 3
#define NETWORK_COLS 2

static GridButtonData network_bd[] = {
    GridButtonData("DHCP",   0, 0, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("IP",     0, 1, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("MASK",   1, 0, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("GATEWAY",1, 1, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("CANCEL", 2, 0, 1, 2, TFT_WHITE, TFT_RED),
};
#define NUM_NETWORK_BUTTONS (sizeof(network_bd)/sizeof(network_bd[0]))

NetworkDialog::NetworkDialog(Display &d, const Rect &rect) :
    GridDialog(
            d,
            rect,
            "NETWORK",
            "Select net setting",
            TFT_BLACK, TFT_LIGHTGREY, NETWORK_ROWS, NETWORK_COLS)
{
    this->add_buttons();
}

void NetworkDialog::add_buttons()
{
    for (int i = 0; i < NUM_NETWORK_BUTTONS; ++i)
        this->add_grid_button(network_bd[i]);
}

bool NetworkDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    bool button_touched = this->GridDialog::check_touch(x, y, pressed)?1:0;
#ifdef NETWORKS_HAS_ADJUSTER
    bool slider_touched = m_adj.check_touch(x, y, pressed)?1:0;
    if (slider_touched && pressed)
    {
        bool result = false;
        TFT_eSPI &tft = m_display.get_tft();
        do
        {
            uint16_t x, y;
            result = tft.getTouch(&x, &y);
            if (result)
                m_adj.check_touch(x, y, pressed);
        } while (result);
    }
#endif
    return button_touched
#ifdef NETWORKS_HAS_ADJUSTER
        || slider_touched
#endif
        ;
}

void NetworkDialog::show(void)
{
    this->GridDialog::show();

#ifdef NETWORKS_HAS_ADJUSTER
    // Show the adjuster
    m_adj.show();
#endif
}
