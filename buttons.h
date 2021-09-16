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
    enum ButtonErrors { BE_BAD_ROWS, BE_BAD_COLS, BE_ALLOC_FAILED, BE_BOUNDS_ERROR};

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
    uint16_t     buttons_errors(void) const { return m_buttons_errors; }
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
    uint16_t     m_buttons_errors;
};

#endif
