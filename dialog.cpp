#include "dialog.h"

#define TITLE_X
#define TITLE_Y

#define PROMPT_X
#define PROMPT_Y

#define DIALOG_MENU_HEIGHT 32
#define TITLE_FONT 4
#define DIALOG_FONT 4

#define PROMPT_X 20
#define PROMPT_Y 60

#ifdef FOR_CLIENT
ButtonData dialog_button_data[] = {
    ButtonData("CFG", TFT_PURPLE, TFT_WHITE),
    ButtonData("CAL", TFT_GREEN, TFT_BLACK),
    ButtonData("CONN", TFT_ORANGE, TFT_WHITE),
    ButtonData("STOP", TFT_RED, TFT_WHITE)
};
#endif

void show_rect(const char *label, const Rect &r)
{
    char rbuf[60];
    sprintf(rbuf, "%s rect: %u %u %u %u",
            label, r.x, r.y, r.w, r.h);
    Serial.println(rbuf);
}

Rect computeMenuRect(const Rect &in,
        // uint16_t dialog_x,
        // uint16_t dialog_y,
        // uint16_t dialog_w,
        // uint16_t dialog_h,
        uint16_t num_buttons,
        Menu::Orient o)
{
    if (o == Menu::O_Vert)
    {
        // return Rect(
        Rect r(
                in.x,
                in.y,
                in.w/num_buttons,
                in.h);
        show_rect("vert", r);
        return r;
    }
    else
    {
        // return Rect(
        Rect r(
                in.x,
                in.y+in.h,
            in.w/num_buttons, DIALOG_MENU_HEIGHT);
        show_rect("horiz", r);
        return r;
    }
}

Dialog::Dialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        const ButtonData *button_data,
        unsigned num_buttons, Menu::Orient o) :
    m_display(d),
    m_rect(rect),
    m_menu(d,
            // Rect(m_rect.x, m_rect.y+m_rect.h-DIALOG_MENU_HEIGHT, m_rect.w/num_buttons, DIALOG_MENU_HEIGHT),
            computeMenuRect(rect, num_buttons, o),
            button_data, num_buttons, o),
    m_title(title),
    m_prompt(prompt)
{
    char dbuf[65];
    sprintf(dbuf, "dialog \"%s\" rect: %u %u %u %u",
            m_title,
            m_rect.x,
            m_rect.y,
            m_rect.w,
            m_rect.h);
    Serial.println(dbuf);

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
    tft.drawRect(
            m_rect.x+1, m_rect.y+1,
            m_rect.w-2, m_rect.h-2,
            TFT_ORANGE);
    tft.setTextColor(TFT_BLACK);
    // Show the title
    tft.setTextDatum(TC_DATUM);
    tft.drawString(m_title, m_rect.x+(m_rect.w/2), m_rect.y, TITLE_FONT);
    // Show the prompt
    tft.setTextDatum(TL_DATUM);
    tft.drawString(m_prompt, m_rect.x+PROMPT_X, m_rect.y+PROMPT_Y, DIALOG_FONT);
    // Show the menu
    m_menu.show(m_display);
}

// Returns true when dialog anwsered, false while dialog still running
bool Dialog::loop(void)
{
    Serial.println("Dialog::loop()");
}

