
#ifndef __network_dialog_h
#define __network_dialog_h

#include "grid_dialog.h"

#include "adjuster.h"

class NetworkDialog: public GridDialog
{
public:
    NetworkDialog(Display&d, const Rect &rect);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    void show(void);
private:
    void add_buttons(void);
};


#endif
