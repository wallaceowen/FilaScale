#ifndef __dialog_h
#define __dialog_h

#include "display.h"
#include "events.h"
#include "rect.h"

class Dialog
{
public:
    Dialog(
            Display&d,
            const Rect &rect,
            const char *title,
            const char *prompt,
            uint16_t fg = TFT_WHITE,
            uint16_t bg = TFT_BLACK);

    virtual void show(void);

    virtual void loop(void) = 0;
    virtual void set_callback(PressEventCB, void*) = 0;
    virtual bool check_touch(uint16_t x, uint16_t y, bool pressed) = 0;

protected:
    Display &m_display;
    const Rect   m_rect;
    const char  *m_title;
    const char  *m_prompt;
    uint16_t     m_fg;
    uint16_t     m_bg;
};

#ifdef OLD_WAY
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


#endif
