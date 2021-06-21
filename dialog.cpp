
#include "dialog.h"
#include "config.h"

DialogBase::DialogBase(Display &d, const Rect &rect, const char *title, const char *prompt) :
    m_display(d),
    m_rect(rect),
    m_title(title),
    m_prompt(prompt)
{
}


void DialogBase::show(void)
{
    // Serial.println("DialogBase::show()");
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

    // Show the title at the top
    tft.setTextDatum(TC_DATUM);
    // tft.drawString(m_title, m_rect.x+(m_rect.w/2), m_rect.y, TITLE_FONT);
    tft.drawString(m_title, m_rect.x+(m_rect.w/2), m_rect.y, TITLE_FONT);

    // Show the prompt just below it
    tft.setTextDatum(TL_DATUM);

    // tft.drawString(m_prompt, m_rect.x+PROMPT_X, m_rect.y+PROMPT_Y, DIALOG_FONT);
    tft.drawString(m_prompt, m_rect.x+PROMPT_X, m_rect.y+tft.fontHeight(TITLE_FONT), DIALOG_FONT);
}

Rect computeMenuRect(const Rect &in, uint16_t num_buttons, Menu::Orient o)
{
    if (o == Menu::O_Vert)
    {
        Rect r(
                in.x,
                in.y,
                in.w,
                in.h);
        return r;
    }
    else
    {
        Rect r(
                in.x,
                in.h-DIALOG_MENU_HEIGHT,
            in.w/num_buttons, DIALOG_MENU_HEIGHT);
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
#ifdef DEBUG
    char dbuf[65];
    sprintf(dbuf, "dialog \"%s\" rect: %u %u %u %u",
            m_title,
            m_rect.x,
            m_rect.y,
            m_rect.w,
            m_rect.h);
    Serial.println(dbuf);
#endif

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
    this->DialogBase::show();

    // Show the menu
    m_menu.show();
}

// Returns true when dialog anwsered, false while dialog still running
void Dialog::loop(void)
{
    Serial.println("Dialog::loop()");
}

#define BUTTONS_Y 180
GridDialog::GridDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        uint16_t rows, uint16_t columns) :
    DialogBase(d, rect, title, prompt),
    m_buttons(d, Rect(rect.x, BUTTONS_Y, rect.w, rect.h-BUTTONS_Y), rows, columns)
{ this->show(); }

void GridDialog::set_callback(ButtonCB mcb, void *user_data)
{
    m_buttons.set_callback(mcb, user_data);
}

bool GridDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    // return m_buttons.check_touch(x, y, pressed);
    bool r = m_buttons.check_touch(x, y, pressed);
    Serial.print("GridDialog::check_touch(): ");
    Serial.println(r);
    return r?1:0;
}

void GridDialog::show(void)
{
    this->DialogBase::show();

    // Show the buttons
    m_buttons.show();
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

MyGridDialog::MyGridDialog(Display &d,
        const Rect &rect,
        const char *title,
        const char *prompt,
        uint16_t rows, uint16_t columns) :
    GridDialog(d, rect, title, prompt, rows, columns),
    m_adj(d, Rect(rect.x, rect.y+80, rect.w, 40), "TITLE", "PROMPT", "VARNAME", 20, 500)
{
    m_adj.show();
}

bool MyGridDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    bool r = this->GridDialog::check_touch(x, y, pressed);
    Serial.print("GridDialog::check_touch(): ");
    Serial.println(r?1:0);
    r = m_adj.check_touch(x, y, pressed);
    Serial.print("MyGridDialog::check_touch(): ");
    Serial.println(r?1:0);
    return r?1:0;
}
