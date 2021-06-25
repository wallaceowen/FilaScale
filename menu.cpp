// menu.cpp

#include <string>

#include "menu.h"

// #define DEBUG_MENU

int widest_button(TFT_eSPI &tft, const ButtonData bdata[], uint16_t num)
{
    int widest = 0;
    for (uint16_t i = 0; i < num; ++i)
    {
        int l = tft.textWidth(bdata[i].label, BUTTON_FONT);
        if (l > widest) 
            widest = l;
    }

    return widest;
}

Menu::Menu(Display &d,
        const Rect &rect,
        const ButtonData bdata[],
        const uint16_t num,
        Orient o) :
    m_display(d),
    num_buttons(num),
    m_bcb(0),
    m_user_data(0)
{
    m_buttons = new BPTR[num_buttons];
    TFT_eSPI &tft = d.get_tft();

    int widest = widest_button(tft, bdata, num);
#ifdef DEBUG_MENU
    Serial.print("--> menu widest = ");
    Serial.println(widest);
#endif

    // Make the buttons
    for (int i = 0; i < num_buttons; ++i)
    {
        uint16_t button_w=0, button_h=0;
        uint16_t button_x=0, button_y=0;
        switch (o)
        {
            case O_Vert:
            {
#ifdef DEBUG_MENU
                Serial.print("--------------------- reect.h "); Serial.println(rect.h);
                Serial.print("--------------------- num buttons "); Serial.println(num_buttons);
#endif
                button_w = widest;
                button_h = rect.h/num_buttons;
#ifdef DEBUG_MENU
                Serial.print("--------------------- button h "); Serial.println(button_h);
#endif
                button_x = rect.x;
                button_y = rect.y+button_h*i;
                break;
            }
            case O_Horiz:
            {
                button_w = rect.w/num_buttons;
                button_h = rect.h;
                button_x = rect.x+button_w*i;
                button_y = rect.y;
                break;
            }
            default:
                break;
        }

        Button *b = new Button(bdata[i], Rect(button_x, button_y, button_w, button_h));
        m_buttons[i] = b;
    }
}

void Menu::set_callback(PressEventCB m, void *user_data)
{
    m_bcb = m;
    m_user_data = user_data;
}

void Menu::show()
{
    for (unsigned i = 0; i < num_buttons; ++i)
    {
#ifdef DEBUG_MENU
        Serial.print("Showing button ");
        Serial.println(m_buttons[i]->label());
#endif
        m_buttons[i]->draw(m_display);
    }
}

bool Menu::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    for (int i = 0; i < num_buttons; ++i)
    {
        if (m_buttons[i]->within(x, y))
        {
            if (m_bcb)
                m_bcb(m_buttons[i]->label(), pressed, m_user_data);
            return true;
        }
    }
    return false;
}

