// buttons.h

#ifndef __buttons_h
#define __buttons_h

#include <stdint.h>

#include "display.h"
#include "rect.h"
#include "button.h"

class Buttons
{
public:
    // Create a grid structure to hold a 2D
    // array of pointers to buttons
    Buttons(Display &d, const Rect &w, uint16_t r, uint16_t c);
    bool add_button(const ButtonData &bd, uint16_t r,  uint16_t c);
    void set_callback(PressEventCB m, void *user_data);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void show();

private:
    TFT_eSPI   &tft;
    Rect        where;
    uint16_t    rows;
    uint16_t    columns;
    Button     **buttons;
    PressEventCB    bcb;
    void       *user_data;
};

#endif
