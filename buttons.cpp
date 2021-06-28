// buttons.cpp

#include "buttons.h"

static char debug_buff[75];
Buttons::Buttons(Display &d, const Rect &w, uint16_t r, uint16_t c) :
    tft(d.get_tft()),
    m_rect(w),
    m_rows(r),
    m_columns(c),
    bcb(0),
    user_data(0)
{
    if ((m_rows == 0) || (m_columns == 0))
    {
        Serial.print("Bad number of rows (");
        Serial.print(m_rows);
        Serial.print(" or columns (");
        Serial.print(m_columns);
        Serial.println(")");
        buttons = 0;
    }
    else
    {
        unsigned count = m_rows*m_columns;
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

    if ((row < m_rows) && (col < m_columns))
    {
        uint16_t width = m_rect.w/m_columns;
        uint16_t height = m_rect.h/m_rows;
        uint16_t x = m_rect.x+width*col;
        uint16_t y = m_rect.y+height*row;

        Button *b = new Button(bd, Rect(x, y, width, height));

        buttons[(row*m_columns)+col] = b;

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
#ifdef DEBUG
    {
        sprintf(debug_buff, "Buttons::add_button() %s at row %u col %u width %u height %u",
                bd.label, row, col, width, height);
        Serial.println(debug_buff);
    }
#endif

    if ((row < m_rows) && (col < m_columns))
    {
        uint16_t x = m_rect.x+width*col;
        uint16_t y = m_rect.y+height*row;

        Button *b = new Button(bd, Rect(x, y, width, height));

        buttons[(row*m_columns)+col] = b;

        return true;
    }
    else
    {
        sprintf(debug_buff, "ERROR: %s wanted r %u c %u but only %u rows %u height",
                bd.label, row, col, m_rows, m_columns);
        Serial.println(debug_buff);
    }
    return false;
}

bool Buttons::add_grid_button(
        const ButtonData &bd,
        uint16_t row, uint16_t col,
        uint16_t row_span, uint16_t column_span)
{
#ifdef DEBUG
    {
        sprintf(debug_buff, "Adding grid button \"%s\" at row %u col %u row_span %u column_span %u",
                bd.label, row, col, row_span, column_span);
        Serial.println(debug_buff);

        sprintf(debug_buff, "Buttons::rect = x:%u y:%u w:%u h:%u",
                m_rect.x, m_rect.y, m_rect.w, m_rect.h);
        Serial.println(debug_buff);

        sprintf(debug_buff, "rows:%u columns:%u", m_rows, m_columns);
        Serial.println(debug_buff);
    }
#endif

    if ((row < m_rows) && (col < m_columns))
    {
        uint16_t cellwidth = (m_rect.w/m_columns)*column_span;
        uint16_t cellheight = (m_rect.h/m_rows)*row_span;

        uint16_t x = m_rect.x+cellwidth*col;
        uint16_t y = m_rect.y+cellheight*row;

#ifdef DEBUG
        // if (0 && (strlen(bd.label) > 1))
        {
            sprintf(debug_buff, "Therefore adding grid button %s at x:%u y:%u cellw:%u cellh:%u",
                    bd.label, x, y, cellwidth, cellheight);
            Serial.println(debug_buff);
        }
#endif

        Button *b = new Button(bd, Rect(x, y, cellwidth, cellheight));

        buttons[(row*m_columns)+col] = b;

        return true;
    }
    else
    {
        sprintf(debug_buff, "ERROR: %s wanted r %u c %u but only %u rows %u height",
                bd.label, row, col, m_rows, m_columns);
        Serial.println(debug_buff);
    }
    return false;
}

Button *Buttons::get_button(uint16_t row, uint16_t col)
{
    if (row < m_rows && col < m_columns)
        return buttons[row*m_columns+col];
    return 0;
}

void Buttons::show()
{
    for (unsigned column = 0; column < m_columns; ++column)
        for (unsigned row = 0; row < m_rows; ++row)
        {
            Button *b = buttons[row*m_columns+column];
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


void Buttons::set_callback(PressEventCB m, void *u)
{
    bcb = m;
    user_data = u;
}

bool Buttons::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    for (unsigned column = 0; column < m_columns; ++column)
        for (unsigned row = 0; row < m_rows; ++row)
        {
            Button *b = buttons[row*m_columns+column];
            if (b)
                if (b->within(x, y))
                {
                    if (bcb)
                        bcb(b->label(), pressed, user_data);
                    return true;
                }
        }
    return false;
}

