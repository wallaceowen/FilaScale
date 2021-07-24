#ifndef __menu_dialog_h
#define __menu_dialog_h

#include "dialog.h"
#include "menu.h"
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
            Menu::Orient o=Menu::O_Horiz,

            uint16_t fg = TFT_WHITE,
            uint16_t bg = TFT_DARKGREY,
            uint16_t title_font = TITLE_FONT,
            uint16_t prompt_font = PROMPT_FONT
            );

    void loop(void);
    void show(void);
    void set_callback(PressEventCB, void*);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);

private:
    Rect computeMenuRect(const Rect &in, uint16_t num_buttons, Menu::Orient o);

    Menu         m_menu;
};

#endif
