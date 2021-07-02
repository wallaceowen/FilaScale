// scale_filament_view.h

#ifndef __filament_view_h
#define __filament_view_h

#include "display.h"
#include "button.h"
#include "view.h"
#include "filament_dialog.h"
#include "filadjust_dialog.h"
#include "menu_dialog.h"

#define NUM_BUTTONS 3


class FilamentView: public View
{
public:
    enum FilamentState { FS_Introduce, FS_Offer, FS_Adjust, FS_NUmStates };

    FilamentView(Display&, ViewChangeCallback ccb, void *change_user_data);
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

    void set_state(FilamentState cs);

    // static void filament_dialog_callback_func(const char *label, bool pressed, void *user_data);
    // void filament_dialog_callback(const char *label, bool pressed);

    void draw_filament();

    Display         &m_display;
    FilamentState    m_state;
    // MenuDialog       m_filintro_dialog;
    FilamentDialog   m_filament_dialog;
    // FilAdjustDialog  m_filadjust_dialog;
    Dialog          *m_current_dialog;
    const char      *m_current_filament;
};

#endif

