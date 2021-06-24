#ifndef __config_dialog_h
#define __config_dialog_h

#include "grid_dialog.h"

class ConfigDialog: public GridDialog
{
public:
    ConfigDialog(Display&d, const Rect &rect);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void show(void);
    void set_callback(PressEventCB, void*);

private:
    void config_button_callback(const char *label, bool pressed);
    static void config_button_callback_func(const char *label, bool pressed, void *user_data);
    void add_buttons(void);

    PressEventCB m_cb;
    void        *m_user_data;
};



#endif
