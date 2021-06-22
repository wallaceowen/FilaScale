#ifndef __kbd_dialog_h
#define __kbd_dialog_h

#include "menu.h"
#include "dialog.h"

#define MAX_VAL_LEN 25

class KbdDialog: public GridDialog
{
public:
    KbdDialog(Display&d, const Rect &rect);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void show(void);
    const char *get_value(void) const { return value; }
private:
    void kbd_menu_callback(const char *label, bool pressed);
    static void kbd_menu_callback_func(const char *label, bool pressed, void *user_data);
    void make_buttons(void);
    char value[MAX_VAL_LEN];
};



#endif
