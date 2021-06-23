
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
        uint16_t bs) :
    Dialog(d, rect, title, prompt, fg, bg),
    m_button_start(bs),
    m_buttons(d, Rect(rect.x, rect.y+m_button_start, rect.w, rect.h-m_button_start), rows, columns)
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
    // Serial.println("GridDialog::show() calling base show");
    this->Dialog::show();
    // Serial.print("back from GridDialog::show() calling base show");

    // Show the buttons
    // Serial.println("GridDialog::show() checking buttons");
    m_buttons.show();
    // Serial.print("back from GridDialog::show() checking buttons");
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

bool GridDialog::add_grid_button(const GridButtonData &gbd, uint16_t rows, uint16_t cols)
{
    // Cell widths are a multiple of cell size;
    // cell size is dialog width/num_cols, dilaog height/num_rows
    uint16_t w = m_rect.w/cols*gbd.width;
    uint16_t h = m_rect.h/rows*gbd.height;

    return m_buttons.add_button(
            ButtonData(gbd.label, gbd.fg, gbd.bg),
            gbd.row, gbd.col, w, h);
}



ThreshDialog::ThreshDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        uint16_t rows, uint16_t columns) :
    GridDialog(d, rect, title, prompt, TFT_WHITE, TFT_BLACK, rows, columns, 180),
    m_adj(d, Rect(rect.x, rect.y+ADJ_OFFSET, rect.w, 40), "VARNAME", 20, 500)
{
    // m_adj.show();
}

bool ThreshDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    bool button_touched = this->GridDialog::check_touch(x, y, pressed)?1:0;
    // Serial.print("GridDialog::check_touch(): ");
    // Serial.println(r?1:0);
    bool slider_touched = m_adj.check_touch(x, y, pressed)?1:0;
    // Serial.print("ThreshDialog::check_touch(): ");
    // Serial.println(r?1:0);
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
    return button_touched || slider_touched;
}

void ThreshDialog::show(void)
{
    this->GridDialog::show();

    // Show the adjuster
    m_adj.show();
}
