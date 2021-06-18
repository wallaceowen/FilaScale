#ifndef __dialog_h
#define __dialog_h

#include "menu.h"
#include "buttons.h"

class DialogBase
{
public:
    DialogBase(Display&, const Rect &rect, const char *title, const char *prompt);

    virtual bool loop(void) = 0;
    virtual void show(void) = 0;
    virtual void set_callback(ButtonCB, void*) = 0;
    virtual bool check_touch(uint16_t x, uint16_t y, bool pressed) = 0;

protected:
    Display &m_display;
    const Rect   m_rect;
    const char  *m_title;
    const char  *m_prompt;
};




class Dialog: public DialogBase
{
public:
    // enum DialogState { DS_Init, DS_Prompting, DS_Answered };

    Dialog(Display&,
            const Rect &rect,
            const char *title,
            const char *prompt,
            const ButtonData *button_data,
            unsigned num_buttons,
            Menu::Orient o=Menu::O_Horiz);

    // Returns true when dialog anwsered, false while dialog still running
    bool loop(void);
    void show(void);
    void set_callback(ButtonCB, void*);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);

private:
    Menu         m_menu;
};

class NewDialog: public DialogBase
{
public:
    // enum DialogState { DS_Init, DS_Prompting, DS_Answered };

    NewDialog(Display&,
            const Rect &rect,
            const char *title,
            const char *prompt,
            uint16_t rows,
            uint16_t cols
            );

    // Returns true when dialog anwsered, false while dialog still running
    virtual bool loop(void);
    void show(void);

    void set_callback(ButtonCB, void*);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    bool add_button(const ButtonData &bd, uint16_t row, uint16_t col);


private:
    Buttons      m_buttons;
};

#endif
