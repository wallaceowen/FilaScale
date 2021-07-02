
#include "netadjust_dialog.h"
#include "stock_buttons.h"

#define ADJ_OFFSET 50

// #define TITLE_MARGIN 50

NetAdjustDialog::NetAdjustDialog(Display &d, const Rect &rect, const char *parameter) :
    MenuDialog(d,
            rect,
            "NETWORK SETTINGS",
            "Adjust network parameters",
            ok_cancel_bd, NUM_OK_BUTTONS)
{
    set_parameter_name(parameter);
}

void NetAdjustDialog::set_parameter_name(const char *parameter)
{
    memset(m_parameter, 0, MAX_NETPARAM_NAMELEN);
    if (strlen(parameter) < MAX_NETPARAM_NAMELEN)
        strcpy(m_parameter, parameter);
    else
        Serial.println("parameter name too long for network adjust dialog");
}

void NetAdjustDialog::show(void)
{
    this->MenuDialog::show();
}

// Returns true when dialog anwsered, false while dialog still running
void NetAdjustDialog::loop(void)
{
    this->MenuDialog::loop();
    // Serial.println("NetAdjustDialog::loop()");
}

