
#include "grid_dialog.h"

#define ADJ_OFFSET 50

static char dbg[98];

GridDialog::GridDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        uint16_t fg,
        uint16_t bg,
        uint16_t rows,
        uint16_t columns,
        uint16_t button_start) :
    Dialog(d, rect, title, prompt, fg, bg),
    m_button_start(button_start),
    m_buttons(d, Rect(rect.x, rect.y+m_button_start, rect.w, rect.h-m_button_start), rows, columns),
    m_rows(rows),
    m_cols(columns)
{
    // sprintf(dbg, "GridDialog ctor got title %s, rows %hu, cols %hu, button_start %hu]",
            // m_title, rows, columns, button_start);
    // Serial.println(dbg);
    // sprintf(dbg, "    ^--rect [%hu, %hu, %hu, %hu]", m_rect.x, m_rect.y, m_rect.w, m_rect.h);
    // Serial.println(dbg);
}

void GridDialog::set_callback(PressEventCB mcb, void *user_data)
{
    m_buttons.set_callback(mcb, user_data);
}

bool GridDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    return m_buttons.check_touch(x, y, pressed);
    // bool r = m_buttons.check_touch(x, y, pressed);
    // Serial.print("GridDialog::check_touch(): ");
    // Serial.println(r);
    // return r?1:0;
}

void GridDialog::show(void)
{
    // Serial.println("GridDialog::show() calling base show");
    this->Dialog::show();
    // Serial.print("back from GridDialog::show() calling base show");

    // Show the buttons
    // Serial.println("GridDialog::show() showing buttons");
    m_buttons.show();
    // Serial.print("back from GridDialog::show() showing buttons");
}

// Returns true when dialog anwsered, false while dialog still running
void GridDialog::loop(void)
{
    Serial.println("GridDialog::loop()");
}

// bool GridDialog::add_button(const ButtonData &bd, uint16_t row, uint16_t col)
// {
    // return m_buttons.add_button(bd, row, col);
// }

bool GridDialog::add_grid_button(const GridButtonData &gbd)
{
    {
        // sprintf(dbg, "grid_dialog.add_grid_button - m_buttons is at %p", &m_buttons);
        // Serial.println(dbg);
        sprintf(dbg, "------------ grid button data = [label %s, row %hu, col %hu, row_span %hu, col_span %hu]",
                gbd.label, gbd.row, gbd.col, gbd.row_span, gbd.column_span);
        Serial.println(dbg);
        sprintf(dbg, "grid_dialog.add_grid_button(): grid rows = %hu grid cols = %hu", m_rows, m_cols);
        Serial.println(dbg);
    }

    // Cell widths are a multiple of cell size;
    // cell size is dialog width/column_span, dialog column_span/row_span

    return m_buttons.add_grid_button(
            ButtonData(gbd.label, gbd.fg, gbd.bg, TL_DATUM),
            gbd.row, gbd.col, gbd.row_span, gbd.column_span);
    return true;
}
