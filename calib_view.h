// calib_view.h

#ifndef __calib_view_h
#define __calib_view_h

#include "display.h"
#include "button.h"
#include "scale.h"
#include "view.h"
#include "menu.h"
#include "dialog.h"
#include "bme280_if.h"

#define NUM_BUTTONS 3


class CalibView: public View
{
public:
    enum CalibState { CS_Ask, CS_Zero, CS_Gain, CS_NUmStates };

    CalibView(Display&, ViewChangeCallback ccb, void *change_user_data, Scale &s);
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

    void set_state(CalibState cs);

    static void calib_dialog_callback_func(const char *label, bool pressed, void *user_data);
    void calib_dialog_callback(const char *label, bool pressed);

    void draw_calib();

    Display   &m_display;
    CalibState m_state;
    Dialog     m_ask_dialog;
    Dialog     m_zero_dialog;
    Dialog     m_gain_dialog;
    Dialog    *m_current_dialog;
    Scale     &m_scale;
};

#endif
