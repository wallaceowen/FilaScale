#ifndef __kbd_dialog_h
#define __kbd_dialog_h

// #include "menu.h"
#include "grid_dialog.h"

#define MAX_VAL_LEN 25

class KbdDialog: public GridDialog
{
public:
    KbdDialog(Display&d, const Rect &rect, const char *title);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void show(void);
    const char *get_value(void) const { return value; }
    void set_callback(PressEventCB, void*);

private:
    void draw_value();
    void kbd_button_callback(const char *label, bool pressed);
    static void kbd_button_callback_func(const char *label, bool pressed, void *user_data);
    void make_buttons(void);
    PressEventCB m_cb;
    void    *m_user_data;
    char value[MAX_VAL_LEN];
};



#endif
