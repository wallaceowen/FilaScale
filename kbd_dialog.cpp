
#include "kbd_dialog.h"

/*
 * +---------------+
 * | V A L U E     |
 * +---------------+
 * | 1  2  3  4  5 |
 * | 6  7  8  9  0 |
 * | .       DONE  |
 * +---------------+
 */

#define KBD_ROWS 3
#define KBD_COLS 5
#define KBD_HEIGHT 200

KbdDialog::KbdDialog(Display &d, const Rect &rect) :
    GridDialog(d, rect, 0, 0, 4, 3)
{
    // Clear the value
    memset(value, 0, sizeof(value));

    // Create the buttons
}

bool KbdDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    bool button_touched = this->GridDialog::check_touch(x, y, pressed)?1:0;
    // Serial.print("KbdDialog::check_touch(): ");
    // Serial.println(r?1:0);
    return button_touched;
}

void KbdDialog::show(void)
{
    this->GridDialog::show();
}
