#ifndef __menu_dialog_h
#define __menu_dialog_h

#include "dialog.h"
#include "buttons.h"

class MenuDialog: public Dialog
{
public:
    // enum DialogState { DS_Init, DS_Prompting, DS_Answered };

    MenuDialog(Display&,
            const Rect &rect,
            const char *title,
            const char *prompt,
            const ButtonData *button_data,
            unsigned num_buttons,
            Menu::Orient o=Menu::O_Horiz);

    // Returns true when dialog anwsered, false while dialog still running
    void loop(void);
    void show(void);
    void set_callback(PressEventCB, void*);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);

private:
    Rect computeMenuRect(const Rect &in, uint16_t num_buttons, Menu::Orient o);
    Menu         m_menu;
};

#endif
