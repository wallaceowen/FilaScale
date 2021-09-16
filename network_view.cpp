// network_view.cpp

#include <stdio.h>

#include "stock_buttons.h"
#include "network_view.h"

#define DLG_X 0
#define DLG_Y 0
#define DLG_HEIGHT 240
#define DLG_WIDTH 320

static const char network_intro_title[] = "Network Settings";
static const char network_intro_body[] = "These dialogs are used for setting network parameters such as "
"whether to use dhcp, IP address, netmask, gateway, ntp server address, etc.";

static const char *state_names[] = { "NS_Introduce", "NS_Offer", "NS_Adjust", "NS_NUmStates" };

NetworkView::NetworkView(Display &d, FilaConfig *fc, ViewChangeCallback ccb, void *change_user_data) :
    View(d, fc, ccb, change_user_data),
    m_display(d),
    m_state(NS_Introduce),
    m_netintro_dialog(d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            network_intro_title,
            network_intro_body,
            ok_cancel_bd, NUM_OK_BUTTONS,
            Menu::O_Horiz,
            TFT_BLACK, TFT_LIGHTGREY, TITLE_FONT, PROMPT_FONT
            ),
    m_network_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT)),
    m_netadjust_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT), "DHCP"),
    m_current_dialog(&m_netintro_dialog)
{
    m_netintro_dialog.set_callback(menu_callback_func, this);
    m_network_dialog.set_callback(menu_callback_func, this);
    m_netadjust_dialog.set_callback(menu_callback_func, this);
}

bool NetworkView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
    return m_current_dialog->check_touch(x, y, pressed);
}

void NetworkView::set_state(NetworkState cs)
{
    m_state = cs;
    switch (m_state)
    {
        case NS_Introduce:
            m_current_dialog = &m_netintro_dialog;
            break;
        case NS_Offer:
            m_current_dialog = &m_network_dialog;
            break;
        case NS_Adjust:
            m_current_dialog = &m_netadjust_dialog;
           break;
        default:
            m_state = NS_Introduce;
            m_current_dialog = &m_netintro_dialog;
           break;
    }
}

const char *netnames[] = { "DHCP", "IP", "NETMASK", "GATEWAY", "NTP", "" };
#define NUM_NETANAMES (sizeof(netnames)/sizeof(netnames[0]))

bool valid_net_name(const char *label)
{
    for (unsigned i = 0; i < NUM_NETANAMES; ++i)
        if (!strcmp(label, netnames[i]))
            return true;
    return false;
}

void NetworkView::menu_callback(const char *label, bool pressed)
{
    if (!pressed)
    {
        if (!strcmp(label, "CANCEL"))
        {
            // Reset state to Ask
            set_state(NS_Introduce);

            // Tell control to go back to state view
            if (m_change_cb)
                m_change_cb("SETTINGS", m_change_data);
        }
        else if (!strcmp(label, "OK"))
        {
            // Switch to the next state
            switch(m_state)
            {
                case NS_Introduce:
                    set_state(NS_Offer);
                    m_change_cb("NETWORK", m_change_data);
                    break;

                // No-op - stay in this state
                case NS_Offer:
                    set_state(NS_Adjust);
                    m_change_cb("NETWORK", m_change_data);
                    break;

                // OK on NS_Adjust means user is done adjusting
                case  NS_Adjust:
                    set_state(NS_Introduce);
                    m_change_cb("NETWORK", m_change_data);
                    break;

                default:
                    break;
            }
        }

        else if (m_state == NS_Offer)
        {
            if (valid_net_name(label))
            {
                set_state(NS_Adjust);
                m_netadjust_dialog.set_parameter_name(label);
            }
            m_change_cb("NETWORK", m_change_data);
        }

        else if (m_state == NS_Adjust)
        {
            m_netadjust_dialog.set_parameter_name(label);
            m_change_cb("NETWORK", m_change_data);
        }
    }
}

void NetworkView::menu_callback_func(const char *label, bool pressed, void *user_data)
{
    NetworkView *cv = reinterpret_cast<NetworkView*>(user_data);
    cv->menu_callback(label, pressed);
}

void NetworkView::show()
{
    m_current_dialog->show();
}

bool NetworkView::update()
{
    return true;
}

void NetworkView::loop()
{
    m_current_dialog->loop();
}

