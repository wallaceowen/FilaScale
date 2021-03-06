// config_view.h

#ifndef __config_view_h
#define __config_view_h

#include "display.h"
#include "button.h"
#include "view.h"
#include "menu_dialog.h"
#include "grid_dialog.h"
#include "config_dialog.h"
// #include "filament_dialog.h"
// #include "kbd_dialog.h"

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

    ConfigView(Display&, FilaConfig *fc, ViewChangeCallback ccb, void *change_user_data);

    // Call this often.  It drives the state machine.
    // or does it??
    void loop();

    // Render the static parts of the view when commanded by Control objhect
    // (or after displaying a simple dialog, which doesn't do save-unders)
    void show();

    bool touch_callback(uint16_t x, uint16_t y, bool pressed);

    // To handle menu choices in dialogs we show
    void menu_callback(const char *label, bool pressed);
    static void menu_callback_func(const char *label, bool pressed, void *user_data);

protected:

    // Deal with a state change (mostly by switching to a different dialog)
    void set_state(ConfigState cs) { m_state = cs; }

    bool update(void);

private:

    Display        &m_display;
    FilaConfig     *m_fc;
    ConfigState     m_state;
    ConfigDialog    m_offer_config_dialog;
    MenuDialog      m_screencal_dialog;
    Dialog         *m_current_dialog;
};

#endif
