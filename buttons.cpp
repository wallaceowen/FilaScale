// buttons.cpp

#include "buttons.h"

static char buttons_debug_buff[75];
Buttons::Buttons(Display &d, const Rect &w, uint16_t r, uint16_t c) :
    m_tft(d.get_tft()),
    m_rect(w),
    m_rows(r),
    m_columns(c),
    m_num_buttons(0),
    m_bcb(0),
    m_user_data(0)
{
    if ((m_rows == 0) || (m_columns == 0))
    {
        Serial.print("Bad number of rows (");
        Serial.print(m_rows);
        Serial.print(" or columns (");
        Serial.print(m_columns);
        Serial.println(")");
        m_buttons = 0;
    }
    else
    {
        m_num_buttons = m_rows*m_columns;
        m_buttons = new BPTR[m_num_buttons];
        if (m_buttons)
            memset(m_buttons, 0, (m_num_buttons)*sizeof(BPTR));
        else
            Serial.println("CANT ALLOC BUTTONS STORAGE");
    }
}

Buttons::~Buttons(void)
{
    for (uint16_t i = 0U; i < m_num_buttons; ++i)
    {
        Button *b = m_buttons[i];
        if (b)
            delete [] b;
    }
    delete [] m_buttons;
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

        m_buttons[(row*m_columns)+col] = b;

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
    if ((row < m_rows) && (col < m_columns))
    {
        uint16_t x = m_rect.x+width*col;
        uint16_t y = m_rect.y+height*row;

        Button *b = new Button(bd, Rect(x, y, width, height));

        m_buttons[(row*m_columns)+col] = b;

        return true;
    }
    else
    {
        sprintf(buttons_debug_buff, "ERROR: %s wanted r %u c %u but only %u rows %u height",
                bd.label, row, col, m_rows, m_columns);
        Serial.println(buttons_debug_buff);
    }
    return false;
}

bool Buttons::add_grid_button(
        const ButtonData &bd,
        uint16_t row, uint16_t col,
        uint16_t row_span, uint16_t column_span)
{
    if ((row < m_rows) && (col < m_columns))
    {
        uint16_t one_cellwidth = m_rect.w/m_columns;
        uint16_t one_cellheight = m_rect.h/m_rows;

        uint16_t x = m_rect.x+one_cellwidth*col;
        uint16_t y = m_rect.y+one_cellheight*row;

        uint16_t cellwidth = one_cellwidth*column_span;
        uint16_t cellheight = one_cellheight*row_span;

        Button *b = new Button(bd, Rect(x, y, cellwidth, cellheight));
        m_buttons[(row*m_columns)+col] = b;

        return true;
    }
    else
    {
        sprintf(buttons_debug_buff, "ERROR: %s wanted r %u c %u but only %u rows %u height",
                bd.label, row, col, m_rows, m_columns);
        Serial.println(buttons_debug_buff);
    }
    return false;
}

Button *Buttons::get_button(uint16_t row, uint16_t col)
{
    if (row < m_rows && col < m_columns)
        return m_buttons[row*m_columns+col];
    return 0;
}

void Buttons::show()
{
    for (unsigned column = 0; column < m_columns; ++column)
        for (unsigned row = 0; row < m_rows; ++row)
        {
            Button *b = m_buttons[row*m_columns+column];
            if (b)
            {
                b->draw(m_tft);
            }
        }
}


void Buttons::set_callback(PressEventCB m, void *u)
{
    m_bcb = m;
    m_user_data = u;
}

bool Buttons::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    for (unsigned column = 0; column < m_columns; ++column)
        for (unsigned row = 0; row < m_rows; ++row)
        {
            Button *b = m_buttons[row*m_columns+column];
            if (b)
                if (b->within(x, y))
                {
                    if (m_bcb)
                        m_bcb(b->label(), pressed, m_user_data);
                    return true;
                }
        }
    return false;
}

