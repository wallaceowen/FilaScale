#ifndef __grid_dialog_h
#define __grid_dialog_h

#include "dialog.h"
#include "buttons.h"

#define MAX_GRIDBUTTON_LABEL_LEN 32

struct GridButtonData
{
    GridButtonData(const char *l,
            uint16_t r, uint16_t c,
            uint16_t rs, uint16_t cs,
            uint16_t f, uint16_t b)
        : row(r), col(c), column_span(cs), row_span(rs), fg(f), bg(b)
    {
        if (strlen(label) < MAX_GRIDBUTTON_LABEL_LEN)
            strcpy(label, l);
    }
    char label[MAX_GRIDBUTTON_LABEL_LEN];
    unsigned row, col;
    uint16_t column_span, row_span;
    uint16_t fg, bg;
};

class GridDialog: public Dialog
{
public:
    // enum DialogState { DS_Init, DS_Prompting, DS_Answered };

    GridDialog(Display&,
            const Rect &rect,
            const char *title,
            const char *prompt,
            uint16_t fg,
            uint16_t bg,
            uint16_t rows,
            uint16_t cols,
            uint16_t button_start);

    // Returns true when dialog anwsered, false while dialog still running
    virtual void loop(void);
    virtual void show(void);

    virtual void set_callback(PressEventCB, void*);
    virtual bool check_touch(uint16_t x, uint16_t y, bool pressed);
    bool add_grid_button(const GridButtonData &gbd);

protected:
    uint16_t m_button_start;
    Buttons  m_buttons;
    uint16_t m_rows;
    uint16_t m_cols;

};

#endif
