
#include "menu_dialog.h"

#define ADJ_OFFSET 50

#define TITLE_MARGIN 50

MenuDialog::MenuDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        const ButtonData *button_data,
        unsigned num_buttons, Menu::Orient o) :
    Dialog(d, rect, title, prompt),
    m_menu(d, computeMenuRect(rect, num_buttons, o),
            button_data, num_buttons, o)
{
    this->show();
}

Rect MenuDialog::computeMenuRect(const Rect &in, uint16_t num_buttons, Menu::Orient o)
{
    TFT_eSPI &tft = m_display.get_tft();

    if (o == Menu::O_Vert)
    {
        Rect r(
                in.x,
                in.y+TITLE_MARGIN,
                in.w,
                in.h-TITLE_MARGIN);
        return r;
    }
    else
    {
        Rect r(
                in.x,
                in.y+in.h-tft.fontHeight(BUTTON_FONT),
                in.w,
                tft.fontHeight(BUTTON_FONT));
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
    Serial.println("MenuDialog::loop()");
}

