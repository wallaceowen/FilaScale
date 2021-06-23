// screencal_view.h

#ifndef __screencal_view_h
#define __screencal_view_h

#include "display.h"
#include "button.h"
#include "view.h"
// #include "menu.h"
#include "menu_dialog.h"
#include "bme280_if.h"

#define NUM_BUTTONS 3


class ScreenCalView: public View
{
public:
    enum ScreenCalState { CS_Ask, CS_Zero, CS_Gain, CS_NUmStates };

    ScreenCalView(Display&, ViewChangeCallback ccb, void *change_user_data);
    // Call this often.  It drives the state machine.
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

    void set_state(ScreenCalState cs);

    static void screencal_dialog_callback_func(const char *label, bool pressed, void *user_data);
    void screencal_dialog_callback(const char *label, bool pressed);

    void draw_screencal();

    Display     &m_display;
    ScreenCalState   m_state;
    MenuDialog   m_ask_dialog;
};

#endif
