// scale_network_view.h

#ifndef __network_view_h
#define __network_view_h

#include "display.h"
#include "button.h"
#include "view.h"
#include "network_dialog.h"
#include "netadjust_dialog.h"
#include "menu_dialog.h"

#define NUM_BUTTONS 3


class NetworkView: public View
{
public:
    enum NetworkState { NS_Introduce, NS_Offer, NS_Adjust, NS_NUmStates };

    NetworkView(Display&, FilaConfig *fc, ViewChangeCallback ccb, void *change_user_data);
    // Call this often.  It drives the state machine.
    void loop();

    // Render the static parts of the view when commanded by Control objhect
    // (or after displaying a simple dialog, which doesn't do save-unders)
    void show();

    bool touch_callback(uint16_t x, uint16_t y, bool pressed);

    // To handle menu choices in dialogs we show
    void menu_callback(const char *label, bool pressed);
    static void menu_callback_func(const char *label, bool pressed, void *user_data);

protected:

    bool update();

private:

    void set_state(NetworkState cs);

    // static void network_dialog_callback_func(const char *label, bool pressed, void *user_data);
    // void network_dialog_callback(const char *label, bool pressed);

    Display         &m_display;
    NetworkState     m_state;
    MenuDialog       m_netintro_dialog;
    NetworkDialog    m_network_dialog;
    NetAdjustDialog  m_netadjust_dialog;
    Dialog          *m_current_dialog;
};

#endif

