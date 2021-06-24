#ifndef __thresh_dialog_h
#define __thresh_dialog_h

#include "grid_dialog.h"

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
