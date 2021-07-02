
#include "filadjust_dialog.h"
#include "stock_buttons.h"

#define ADJ_OFFSET 50

// #define TITLE_MARGIN 50

FilAdjustDialog::FilAdjustDialog(Display &d, const Rect &rect, const char *filament) :
    MenuDialog(d,
            rect,
            "FILAMENT SETTINGS",
            "Adjust filament thresholds",
            ok_cancel_bd, NUM_OK_BUTTONS)
{
    set_filament_name(filament);
}

void FilAdjustDialog::set_filament_name(const char *filament)
{
    memset(m_filament, 0, MAX_FILAMENT_NAMELEN);
    if (strlen(filament) < MAX_FILAMENT_NAMELEN)
        strcpy(m_filament, filament);
    else
        Serial.println("filament name too long for filadjust dialog");
}

void FilAdjustDialog::show(void)
{
    this->MenuDialog::show();

    // FilamentData *fildata = fetch_fildata(m_filament);

    // Let's stretch and put up multiple Adjusts?
    // render_filata(fildata);
}

// Returns true when dialog anwsered, false while dialog still running
void FilAdjustDialog::loop(void)
{
    this->MenuDialog::loop();
    // Serial.println("FilAdjustDialog::loop()");
}

