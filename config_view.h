// config_view.h

#ifndef __config_view_h
#define __config_view_h

#include "display.h"
#include "button.h"
#include "view.h"
#include "menu.h"
#include "dialog.h"

class ConfigView: public View
{
public:
    enum ConfigState { COS_Offer, COS_Thresholds, COS_Network, COS_NUmStates };

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

    bool update();

private:

    void set_state(ConfigState cs);

    void add_threshold_buttons();

    static void config_dialog_callback_func(const char *label, bool pressed, void *user_data);
    void config_dialog_callback(const char *label, bool pressed);

    void draw_config();

    Display        &m_display;
    ConfigState     m_state;
    Dialog          m_offer_config_dialog;
    // Dialog       m_thresh_config_dialog;
    NewDialog       m_thresh_config_dialog;
    Dialog          m_network_config_dialog;
    DialogBase     *m_current_dialog;
};

#endif
