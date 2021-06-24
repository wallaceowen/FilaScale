// config_view.h

#ifndef __config_view_h
#define __config_view_h

#include "display.h"
#include "button.h"
#include "view.h"
#include "menu_dialog.h"
#include "grid_dialog.h"
#include "config_dialog.h"
#include "filament_dialog.h"
#include "kbd_dialog.h"

#define CONF_DILAOG_CB_NEEDED

class ConfigView: public View
{
public:
    enum ConfigState {
        COS_Offer,
        COS_Filament,
        COS_Screen,
        COS_Network,
        COS_NUmStates
    };

    ConfigView(Display&, ViewChangeCallback ccb, void *change_user_data);

    // Call this often.  It drives the state machine.
    // or does it??
    void loop();

    // Render the static parts of the view when commanded by Control objhect
    // (or after displaying a simple dialog, which doesn't do save-unders)
    void show();

    void touch_callback(uint16_t x, uint16_t y, bool pressed);

    // To handle menu choices in dialogs we show
    void menu_callback(const char *label, bool pressed);
    static void menu_callback_func(const char *label, bool pressed, void *user_data);

protected:

    bool update(void);

private:

    void set_state(ConfigState cs);

#ifdef CONF_DILAOG_CB_NEEDED
    static void config_dialog_callback_func(const char *label, bool pressed, void *user_data);
    void config_dialog_callback(const char *label, bool pressed);
#endif

    Display        &m_display;
    ConfigState     m_state;
    ConfigDialog    m_offer_config_dialog;
    FilamentDialog  m_filament_config_dialog;
#ifdef NETCONF_IS_KBD_DIALOG
    KbdDialog       m_keypad_dialog;
#else
    MenuDialog      m_network_config_dialog;
#endif
    Dialog         *m_current_dialog;
};

#endif
