
#include "grid_dialog.h"

GridDialog::GridDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        uint16_t fg,
        uint16_t bg,
        uint16_t rows,
        uint16_t columns) :
    Dialog(d, rect, title, prompt, fg, bg),
    m_buttons(d, Rect(rect.x, rect.y+button_y(), rect.w, rect.h-button_y()), rows, columns),
    m_rows(rows),
    m_cols(columns)
{
}

void GridDialog::set_callback(PressEventCB mcb, void *user_data)
{
    m_buttons.set_callback(mcb, user_data);
}

bool GridDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    return m_buttons.check_touch(x, y, pressed);
}

void GridDialog::show(void)
{
    this->Dialog::show();

    // Show the buttons
    m_buttons.show();
}

// Returns true when dialog anwsered, false while dialog still running
void GridDialog::loop(void)
{
    // Serial.println("GridDialog::loop()");
}

bool GridDialog::add_grid_button(const GridButtonData &gbd)
{
    // Cell widths are a multiple of cell size;
    // cell size is dialog width/column_span, dialog column_span/row_span
    return m_buttons.add_grid_button(
            ButtonData(gbd.label, gbd.fg, gbd.bg, CC_DATUM),
            gbd.row, gbd.col, gbd.row_span, gbd.column_span);
    return true;
}
