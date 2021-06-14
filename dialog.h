#ifndef __dialog_h
#define __dialog_h

#include "menu.h"

class Dialog
{
public:
    // enum DialogState { DS_Init, DS_Prompting, DS_Answered };

    Dialog(Display&,
            const Rect &rect,
            const char *title,
            const char *prompt,
            const ButtonData *button_data,
            unsigned num_buttons);

    // Returns true when dialog anwsered, false while dialog still running
    virtual bool loop(void);
    void show(void);

    void set_callback(MenuCB, void*);
    bool check_touch(Display *d, uint16_t x, uint16_t y, bool pressed);

private:
    Display &m_display;
    const Rect   m_rect;
    Menu         m_menu;
    const char  *m_title;
    const char  *m_prompt;
};

#endif
