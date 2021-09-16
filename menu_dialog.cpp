
#include "menu_dialog.h"

#define ADJ_OFFSET 50

// #define TITLE_MARGIN 50

MenuDialog::MenuDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        const ButtonData *button_data,
        unsigned num_buttons,
        Menu::Orient o,
        uint16_t fg,
        uint16_t bg,
        uint16_t title_font,
        uint16_t prompt_font) :
    Dialog(d, rect, title, prompt, fg, bg, title_font, prompt_font),
    m_menu(d, computeMenuRect(rect, num_buttons, o), button_data, num_buttons, o)
{
}

Rect MenuDialog::computeMenuRect(const Rect &in, uint16_t num_buttons, Menu::Orient o)
{

    if (o == Menu::O_Vert)
    {

        Rect r(
                in.x,
                in.y+this->button_y(),
                in.w,
                in.h-this->button_y());
        return r;
    }
    else
    {
        uint16_t needed_ht = m_display.get_tft().fontHeight(BUTTON_FONT)+BUTTON_PAD;
        Rect r(
                in.x,
                in.y+in.h-needed_ht,
                in.w,
                needed_ht);
        return r;
    }
}

void MenuDialog::set_callback(PressEventCB mcb, void *user_data)
{
    m_menu.set_callback(mcb, user_data);
}

bool MenuDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    return m_menu.check_touch(x, y, pressed);
}

void MenuDialog::show(void)
{
    this->Dialog::show();
    m_menu.show();
}

// Returns true when dialog anwsered, false while dialog still running
void MenuDialog::loop(void)
{
}

