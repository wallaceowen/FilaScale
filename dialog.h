#ifndef __dialog_h
#define __dialog_h

#include "display.h"
#include "events.h"
#include "rect.h"
#include "text_box.h"

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
    uint16_t button_y(void) const { return m_prompt.last_y(); }

protected:
    Display &m_display;
    const Rect   m_rect;
    const char  *m_title;
    TextBox      m_prompt;
    uint16_t     m_fg;
    uint16_t     m_bg;
};

#endif
