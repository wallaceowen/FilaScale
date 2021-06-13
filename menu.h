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
            const Rect &r,
            const ButtonData *bdata,
            const uint16_t num_buttons, Orient o);
    void show(Display &d);
    bool check_touch(Display *d, uint16_t x, uint16_t y, bool pressed);

private:
    uint16_t num_buttons;
    Button **m_buttons;
};

#endif
