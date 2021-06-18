// buttons.cpp

#include "buttons.h"

Buttons::Buttons(Display &d, const Rect &w, uint16_t r, uint16_t c) :
    tft(d.get_tft()),
    where(w),
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
        buttons = new BPTR[rows*columns];
        memset(buttons, 0, (rows*columns)*sizeof(BPTR));
    }
}

bool Buttons::add_button(const ButtonData &bd, uint16_t row, uint16_t col)
{
    if ((row < rows) && (col < columns))
    {
        uint16_t width = where.w/columns;
        uint16_t height = where.h/rows;
        uint16_t x = where.x+width*columns;
        uint16_t y = where.y+height*row;

        Button *b = new Button(bd, Rect(x, y, width, height));

        buttons[(row*columns)+col] = b;

        return true;
    }
    return false;
}

void Buttons::show()
{
    for (unsigned column = 0; column < columns; ++column)
    {
        for (unsigned row = 0; row < rows; ++row)
        {
            Serial.print("showing button ");
            Serial.print(row);
            Serial.print(", ");
            Serial.println(column);
            Button *b = buttons[row*columns+column];
            if (b)
                b->draw(tft);
        }
    }
}


void Buttons::set_callback(ButtonCB m, void *u)
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

