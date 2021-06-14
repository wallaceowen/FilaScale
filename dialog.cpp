#include "dialog.h"

#define TITLE_X
#define TITLE_Y

#define PROMPT_X
#define PROMPT_Y

#define DIALOG_MENU_HEIGHT 32
#define TITLE_FONT 4
#define DIALOG_FONT 2

#ifdef FOR_CLIENT
ButtonData dialog_button_data[] = {
    ButtonData("CFG", TFT_PURPLE, TFT_WHITE),
    ButtonData("CAL", TFT_GREEN, TFT_BLACK),
    ButtonData("CONN", TFT_ORANGE, TFT_WHITE),
    ButtonData("STOP", TFT_RED, TFT_WHITE)
};
#endif

Dialog::Dialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        const ButtonData *button_data,
        unsigned num_buttons) :
    m_display(d),
    m_rect(rect),
    m_menu(d,
            Rect(m_rect.x, m_rect.y+m_rect.h-DIALOG_MENU_HEIGHT, m_rect.w, DIALOG_MENU_HEIGHT),
            button_data, num_buttons,
            Menu::O_Horiz),
    m_title(title),
    m_prompt(prompt)
{
    // m_menu.set_callback(d_menu_cb, this);
    this->show();
}

void Dialog::set_callback(MenuCB mcb, void *user_data)
{
    m_menu.set_callback(mcb, user_data);
}

bool Dialog::check_touch(Display *d, uint16_t x, uint16_t y, bool pressed)
{
    return m_menu.check_touch(d, x, y, pressed);
}


void Dialog::show(void)
{
    Serial.println("Dialog::show()");

    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(
            m_rect.x, m_rect.y,
            m_rect.w, m_rect.h,
            TFT_WHITE);
    // Show the title
    tft.drawString(m_title, m_rect.x+tft.width()/2, m_rect.y, DIALOG_FONT);
    // Show the prompt
    // Show the menu
    m_menu.show(m_display);
}

// Returns true when dialog anwsered, false while dialog still running
bool Dialog::loop(void)
{
    Serial.println("Dialog::loop()");
}

