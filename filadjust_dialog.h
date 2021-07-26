#ifndef __filadjust_dialog_h
#define __filadjust_dialog_h

#include "dialog.h"
#include "menu_dialog.h"
#include "buttons.h"

#define MAX_FILAMENT_NAMELEN 32

class FilAdjustDialog: public MenuDialog
{
public:
    FilAdjustDialog(Display&, const Rect &rect, const char *filament);

    // Returns true when dialog anwsered, false while dialog still running
    void loop(void);
    void show(void);

    void set_filament_name(const char *label);

private:
    char m_filament[MAX_FILAMENT_NAMELEN];
};

#endif
