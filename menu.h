// menu.h

#ifndef __menu_h
#define __menu_h

#include <stdint.h>

#include "button.h"

typedef void (*MenuCB)(const char *label, bool pressed, void *user_data);

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

    void show(Display &d);
    bool check_touch(Display *d, uint16_t x, uint16_t y, bool pressed);
    void set_callback(MenuCB m, void *user_data);

private:
    uint16_t num_buttons;
    MenuCB   m_mcb;
    void    *m_user_data;
    Button **m_buttons;
};

#endif
