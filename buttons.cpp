// buttons.cpp

#include "buttons.h"

Buttons::Buttons(Display &d, const Rect &w, uint16_t r, uint16_t c) :
    tft(d.get_tft()),
    m_rect(w),
    rows(r),
    columns(c),
    bcb(0),
    user_data(0)
{
    if ((rows == 0) || (columns == 0))
    {
        Serial.print("Bad number of rows (");
        Serial.print(rows);
        Serial.print(" or columns (");
        Serial.print(columns);
        Serial.println(")");
        buttons = 0;
    }
    else
    {
        unsigned count = rows*columns;
        // Serial.print("Allocing bytes for ");
        // Serial.print(count);
        // Serial.println(" buttons");
        buttons = new BPTR[count];
        if (buttons)
            memset(buttons, 0, (count)*sizeof(BPTR));
        else
            Serial.println("CANT ALLOC BUTTONS");
    }
}

bool Buttons::add_button(const ButtonData &bd, uint16_t row, uint16_t col)
{
    return true;

    if ((row < rows) && (col < columns))
    {
        uint16_t width = m_rect.w/columns;
        uint16_t height = m_rect.h/rows;
        uint16_t x = m_rect.x+width*col;
        uint16_t y = m_rect.y+height*row;

        Button *b = new Button(bd, Rect(x, y, width, height));

        buttons[(row*columns)+col] = b;

        return true;
    }
    else
    {
        Serial.println("CANT PLACE BUTTON");
        return false;
    }
}

bool Buttons::add_button(
        const ButtonData &bd,
        uint16_t row, uint16_t col,
        uint16_t width, uint16_t height)
{
    // {
        // static char buff[72];
        // sprintf(buff, "Adding button at row %u col %u width %u height %u", row, col, width, height);
        // Serial.println(buff);
    // }

    if ((row < rows) && (col < columns))
    {
        uint16_t x = m_rect.x+width*col;
        uint16_t y = m_rect.y+height*row;

        // {
            // char buff[65];
            // sprintf(buff, "Adding button %s at x:%u y:%u w:%u h:%u",
                    // bd.label, x, y, width, height);
            // Serial.println(buff);
        // }

        Button *b = new Button(bd, Rect(x, y, width, height));

        buttons[(row*columns)+col] = b;

        return true;
    }
    else
    {
        static char buff[75];
        sprintf(buff, "ERROR: %s wanted r %u c %u but only %u rows %u height",
                bd.label, row, col, rows, columns);
        Serial.println(buff);
    }
    return false;
}

Button *Buttons::get_button(uint16_t row, uint16_t col)
{
    if (row < rows && col < columns)
        return buttons[row*columns+col];
    return 0;
}

void Buttons::show()
{
    for (unsigned column = 0; column < columns; ++column)
    {
        for (unsigned row = 0; row < rows; ++row)
        {
            Button *b = buttons[row*columns+column];
            if (b)
            {
#ifdef DEBUG
                Serial.print("showing button ");
                Serial.print(row);
                Serial.print(", ");
                Serial.println(column);
#endif
                b->draw(tft);
            }
        }
    }
}


void Buttons::set_callback(PressEventCB m, void *u)
{
    bcb = m;
    user_data = u;
}

bool Buttons::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    for (unsigned column = 0; column < columns; ++column)
    {
        for (unsigned row = 0; row < rows; ++row)
        {
            Button *b = buttons[row*columns+column];
            if (b)
            {
                if (b->within(x, y))
                {
                    if (bcb)
                        bcb(b->label(), pressed, user_data);
                    return true;
                }
            }
        }
    }
    return false;
}

