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

void show_rect(const char *label, const Rect &r)
{
    char rbuf[60];
    sprintf(rbuf, "%s rect: %u %u %u %u",
            label, r.x, r.y, r.w, r.h);
    Serial.println(rbuf);
}
DialogBase::DialogBase(Display &d, const Rect &rect, const char *title, const char *prompt) :
    m_display(d),
    m_rect(rect),
    m_title(title),
    m_prompt(prompt)
{
}

Rect computeMenuRect(const Rect &in, uint16_t num_buttons, Menu::Orient o)
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
    DialogBase(d, rect, title, prompt),
    m_menu(d, computeMenuRect(rect, num_buttons, o),
            button_data, num_buttons, o)
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

void Dialog::set_callback(ButtonCB mcb, void *user_data)
{
    m_menu.set_callback(mcb, user_data);
}

bool Dialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    return m_menu.check_touch(x, y, pressed);
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
    m_menu.show();
}

// Returns true when dialog anwsered, false while dialog still running
bool Dialog::loop(void)
{
    Serial.println("Dialog::loop()");
}

NewDialog::NewDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        uint16_t rows, uint16_t columns) :
    DialogBase(d, rect, title, prompt),
    m_buttons(d, Rect(rect.x, rect.y, rect.w, rect.h), rows, columns)
{
    char dbuf[65];
    sprintf(dbuf, "dialog \"%s\" rect: %u %u %u %u",
            m_title,
            m_rect.x,
            m_rect.y,
            m_rect.w,
            m_rect.h);
    Serial.println(dbuf);

    this->show();
}

void NewDialog::set_callback(ButtonCB mcb, void *user_data)
{
    m_buttons.set_callback(mcb, user_data);
}

bool NewDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    return m_buttons.check_touch(x, y, pressed);
}

void NewDialog::show(void)
{
    Serial.println("NewDialog::show()");

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
    m_buttons.show();
}

// Returns true when dialog anwsered, false while dialog still running
bool NewDialog::loop(void)
{
    Serial.println("NewDialog::loop()");
}
