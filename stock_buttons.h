// Stock buttons

#ifndef __stock_buttons_h
#define __stock_buttons_h

#include "button.h"

// Should move this to something like stock_buttons.h
// so all share the common ones.
extern ButtonData ok_cancel_bd[2];
#define NUM_OK_BUTTONS (sizeof(ok_cancel_bd)/sizeof(ok_cancel_bd[0]))

#endif
