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
    // Create a grid structure to hold a 2D array of pointers to buttons
    Buttons(Display &d, const Rect &w, uint16_t r, uint16_t c);
    ~Buttons(void);

    bool add_button(const ButtonData &bd, uint16_t r,  uint16_t c);
    bool add_button(const ButtonData &bd,
            uint16_t r,  uint16_t c,
            uint16_t width, uint16_t height);
    bool add_grid_button(const ButtonData &bd,
            uint16_t r,  uint16_t c,
            uint16_t rows, uint16_t columns);
    void set_callback(PressEventCB m, void *user_data);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    Button *get_button(uint16_t row, uint16_t col);
    void show();

private:
    TFT_eSPI    &m_tft;
    Rect         m_rect;
    uint16_t     m_rows;
    uint16_t     m_columns;
    uint16_t     m_num_buttons;
    Button     **m_buttons;
    PressEventCB m_bcb;
    void        *m_user_data;
};

#endif
