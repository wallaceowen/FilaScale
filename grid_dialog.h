#ifndef __grid_dialog_h
#define __grid_dialog_h

#include "dialog.h"
#include "buttons.h"

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
    bool add_button(const ButtonData &bd, uint16_t row, uint16_t col);
    bool add_grid_button(const GridButtonData &gbd, uint16_t rows, uint16_t cols);

protected:
    uint16_t m_button_start;
    Buttons  m_buttons;

};

#include "adjuster.h"

class ThreshDialog: public GridDialog
{
public:
    ThreshDialog(Display&d,
            const Rect &rect,
            const char *title,
            const char *prompt,
            uint16_t rows,
            uint16_t cols);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void show(void);
private:
    Adjuster m_adj;
};


#endif
