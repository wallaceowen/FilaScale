#ifndef __netadjust_dialog_h
#define __netadjust_dialog_h

#include "dialog.h"
#include "menu_dialog.h"
#include "buttons.h"

#define MAX_NETPARAM_NAMELEN 32

class NetAdjustDialog: public MenuDialog
{
public:
    // enum DialogState { DS_Init, DS_Prompting, DS_Answered };

    NetAdjustDialog(Display&, const Rect &rect, const char *filament);

    void loop(void);
    void show(void);

    void set_parameter_name(const char *label);

private:
    char m_parameter[MAX_NETPARAM_NAMELEN];
};

#endif
