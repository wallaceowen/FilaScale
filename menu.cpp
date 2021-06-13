// menu.cpp

#include <string>

#include "menu.h"

#define BUTTON_GAP 2
#define BUTTONS_START 4
#define BUTTONS_Y 190
#define BUTTONS_HEIGHT 40
#define BUTTONS_START 4

typedef Button *BPTR;

Menu::Menu(Display &d,
        const Rect &rect,
        const ButtonData bdata[],
        const uint16_t num,
        Orient o) :
    num_buttons(num),
    m_mcb(0),
    m_user_data(0)
{
    m_buttons = new BPTR[num_buttons];

    // Make the buttons
    for (int i = 0; i < num_buttons; ++i)
    {
        uint16_t button_w, button_h;
        uint16_t button_x, button_y;
        switch (o)
        {
            case O_Vert:
            {
                button_w = rect.w;
                button_h = rect.h / num_buttons;
                button_x = rect.x;
                button_y = rect.y+button_w*i;
                break;
            }
            case O_Horiz:
            {
                button_w = rect.w / num_buttons;
                button_h = rect.h;
                button_x = rect.x+button_w*i;
                button_y = rect.y;
                break;
            }
            default:
                break;
        }

        Button *b = new Button(
                bdata[i].label,
                Rect(button_x, button_y, button_w, button_h),
                bdata[i].color, bdata[i].txt_color);

        m_buttons[i] = b;
    }
}

void Menu::set_callback(MenuCB m, void *user_data)
{
    m_mcb = m;
    m_user_data = user_data;
}

void Menu::show(Display &d)
{
    for (unsigned i = 0; i < num_buttons; ++i)
        m_buttons[i]->draw(d);
}

bool Menu::check_touch(Display *d, uint16_t x, uint16_t y, bool pressed)
{
    for (int i = 0; i < num_buttons; ++i)
    {
        if (m_buttons[i]->within(x, y))
        {
            // Serial.print("Button ");
            // Serial.print(m_buttons[i]->label());
            // if (pressed)
                // Serial.println(" pressed");
            // else
                // Serial.println(" released");
            if (m_mcb)
                m_mcb(m_buttons[i]->label(), pressed, m_user_data);
            return true;
        }
    }
    return false;
}
