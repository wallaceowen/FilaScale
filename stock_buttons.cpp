// Stock buttons

#include "stock_buttons.h"

// Should move this to something like stock_buttons.h
// so all share the common ones.
ButtonData ok_cancel_bd[2] = {
    ButtonData("OK", TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN),
    ButtonData("CANCEL", TFT_RED, TFT_WHITE, TFT_RED),
};
#define NUM_OK_BUTTONS (sizeof(ok_cancel_bd)/sizeof(ok_cancel_bd[0]))

