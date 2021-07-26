
#include "filament_dialog.h"

#define FILAMENT_ROWS 3
#define FILAMENT_COLS 3

static GridButtonData filament_bd[] = {
    GridButtonData("PLA",    0, 0, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("ABS",    0, 1, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("ASA",    0, 2, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("Nylon",  0, 3, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("PETG",   1, 0, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("TPU",    1, 1, 1, 1, TFT_WHITE, TFT_DARKGREY),
    GridButtonData("CANCEL", 2, 0, 1, 4, TFT_WHITE, TFT_RED),
};
#define NUM_FILAMENT_BUTTONS (sizeof(filament_bd)/sizeof(filament_bd[0]))

FilamentDialog::FilamentDialog(Display &d, const Rect &rect) :
    GridDialog(
            d,
            rect,
            "FILAMENTS",
            "Select filament type",
            TFT_BLACK, TFT_LIGHTGREY, 3, 4)
{
    this->add_buttons();
}

void FilamentDialog::add_buttons()
{
    for (int i = 0; i < NUM_FILAMENT_BUTTONS; ++i)
        this->add_grid_button(filament_bd[i]);
}

bool FilamentDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    bool button_touched = this->GridDialog::check_touch(x, y, pressed)?1:0;
#ifdef FILAMENTS_HAS_ADJUSTER
    bool slider_touched = m_adj.check_touch(x, y, pressed)?1:0;
    if (slider_touched && pressed)
    {
        bool result = false;
        TFT_eSPI &tft = m_display.get_tft();
        do
        {
            uint16_t x, y;
            result = tft.getTouch(&x, &y);
            if (result)
                m_adj.check_touch(x, y, pressed);
        } while (result);
    }
#endif
    return button_touched
#ifdef FILAMENTS_HAS_ADJUSTER
        || slider_touched
#endif
        ;
}

void FilamentDialog::show(void)
{
    this->GridDialog::show();

#ifdef FILAMENTS_HAS_ADJUSTER
    // Show the adjuster
    m_adj.show();
#endif
}
