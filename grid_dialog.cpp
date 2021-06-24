
#include "grid_dialog.h"

#define ADJ_OFFSET 50

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
    // this->show();
}

void GridDialog::set_callback(PressEventCB mcb, void *user_data)
{
    m_buttons.set_callback(mcb, user_data);
}

bool GridDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    // return m_buttons.check_touch(x, y, pressed);
    bool r = m_buttons.check_touch(x, y, pressed);
    // Serial.print("GridDialog::check_touch(): ");
    // Serial.println(r);
    return r?1:0;
}

void GridDialog::show(void)
{
    Serial.println("GridDialog::show() calling base show");
    this->Dialog::show();
    Serial.print("back from GridDialog::show() calling base show");

    // Show the buttons
    Serial.println("GridDialog::show() showing buttons");
    m_buttons.show();
    Serial.print("back from GridDialog::show() showing buttons");
}

// Returns true when dialog anwsered, false while dialog still running
void GridDialog::loop(void)
{
    Serial.println("GridDialog::loop()");
}

bool GridDialog::add_button(const ButtonData &bd, uint16_t row, uint16_t col)
{
    return m_buttons.add_button(bd, row, col);
}

bool GridDialog::add_grid_button(const GridButtonData &gbd)
{
    {
        static char dbg[68];
        // sprintf(dbg, "add_grid_button - m_buttons is at %p", &m_buttons);
        // Serial.println(dbg);
        sprintf(dbg, "grid buitton data = [label %s, row %hu, col %hu, w %hu, h %hu fg %u bg %u]",
                gbd.label, gbd.row, gbd.col, gbd.row_span, gbd.column_span, gbd.fg, gbd.bg);
        Serial.println(dbg);
        sprintf(dbg, "add_grid_button(): rows = %hu cols = %hu", m_rows, m_cols);
        Serial.println(dbg);
    }
    // Serial.println("add_grid_button");

    // Cell widths are a multiple of cell size;
    // cell size is dialog width/column_span, dialog column_span/row_span

    //// uint16_t w = m_rect.w/m_cols*gbd.rows;
    //// uint16_t h = (m_rect.h-m_button_start)/m_rows*gbd.column_span;

    uint16_t w = (m_rect.w/m_cols)*gbd.row_span;
    uint16_t h = ((m_rect.h-m_button_start)/m_rows)*gbd.column_span;


    return m_buttons.add_button(
            ButtonData(gbd.label, gbd.fg, gbd.bg),
            gbd.row, gbd.col, w, h);
    // return m_buttons.add_button(
            // ButtonData("foobar", 65535, 63488),
            // 0, 0, 1, 1);
    return true;
}
