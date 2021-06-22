#ifndef __dialog_h
#define __dialog_h

#include "menu.h"
#include "buttons.h"

class DialogBase
{
public:
    DialogBase(Display&, const Rect &rect, const char *title, const char *prompt);

    virtual void show(void);

    virtual void loop(void) = 0;
    virtual void set_callback(PressEventCB, void*) = 0;
    virtual bool check_touch(uint16_t x, uint16_t y, bool pressed) = 0;

protected:
    Display &m_display;
    const Rect   m_rect;
    const char  *m_title;
    const char  *m_prompt;
};

class MenuDialog: public DialogBase
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

class GridDialog: public DialogBase
{
public:
    // enum DialogState { DS_Init, DS_Prompting, DS_Answered };

    GridDialog(Display&,
            const Rect &rect,
            const char *title,
            const char *prompt,
            uint16_t rows,
            uint16_t cols
            );

    // Returns true when dialog anwsered, false while dialog still running
    virtual void loop(void);
    virtual void show(void);

    virtual void set_callback(PressEventCB, void*);
    virtual bool check_touch(uint16_t x, uint16_t y, bool pressed);
    bool add_button(const ButtonData &bd, uint16_t row, uint16_t col);


private:
    Buttons      m_buttons;
};

#include "adjuster.h"

class ThreshDialog: public GridDialog
{
public:
    ThreshDialog(Display&d,
            const Rect &rect,
            const char *title,
            const char *prompt,
            uint16_t rows,
            uint16_t cols
            );
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void show(void);
private:
    Adjuster m_adj;
};


#endif
