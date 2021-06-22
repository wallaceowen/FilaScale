// menu.h

#ifndef __menu_h
#define __menu_h

#include <stdint.h>

#include "button.h"

class Menu
{
public:
    enum Orient { O_Vert, O_Horiz };

    Menu(
            Display &d,

            // Size of one window in Menu; all same size
            const Rect &r,

            // array, num_buttons in len
            const ButtonData *bdata,
            const uint16_t num_buttons,

            // Vertial or horizontal
            Orient o);

    void show();
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void set_callback(PressEventCB m, void *user_data);

private:
    Display    &m_display;
    uint16_t    num_buttons;
    PressEventCB    m_bcb;
    void       *m_user_data;
    Button    **m_buttons;
};

#endif
