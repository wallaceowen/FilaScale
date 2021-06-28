#ifndef __filament_dialog_h
#define __filament_dialog_h

#include "grid_dialog.h"

#include "adjuster.h"

class FilamentDialog: public GridDialog
{
public:
    FilamentDialog(Display&d, const Rect &rect);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void show(void);
private:
    void add_buttons(void);
};


#endif
