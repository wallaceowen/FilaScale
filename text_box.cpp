// text_box.cpp

#include "text_box.h"

#ifdef DEBUG_WITH_CERR
#include <iostream>
using namespace std;
#endif

char dbg[90];

TextBox::TextBox(Display &d, const Rect &r, uint16_t font, const char *txt) :
    m_tft(d.get_tft()),
    m_rect(r),
    m_font(font),
    m_max_rows(m_rect.w/m_tft.fontHeight(m_font)),
    m_len(strlen(txt)), m_last_x(0), m_last_y(0)
{
    memset(m_buffer, 0, TB_BUFFER_SIZE);
    if (m_len < TB_BUFFER_SIZE)
    {
        strcpy(m_buffer, txt);
    }
    else
    {
        strncpy(m_buffer, txt, TB_BUFFER_SIZE-1);
        m_buffer[TB_BUFFER_SIZE-1] = 0;
#ifdef DEBUG_WITH_CERR
        cerr << "TextBox::TextBox: buffer too small"
            << endl;
#endif
    }

#ifdef DEBUG_WITH_CERR
    cerr << "TextBox::TextBox now has "
        << m_buffer
        << endl;
#endif
    reserve_space_for_text();

    {
        sprintf(dbg, "text_box ctor: \"%s\" at x:%hu y:%hu w:%hu h:%hu",
                m_buffer, m_rect.x, m_rect.y, m_rect.w, m_rect.h);
        Serial.println(dbg);
        sprintf(dbg, "text_box ctor: \"%s\" last_y:%hu", m_buffer, m_last_y);
        Serial.println(dbg);
    }
}

char *terminate_next(char *txt)
{
    char *ptr = txt;
    for (; *ptr; ++ptr)
        if (*ptr == ' ')
        {
            *ptr = '\0';
            break;
        }
    return ptr;
}

// Returns -1 if no more room, 1 if we've wrapped and 0 if we haven't
int TextBox::render_word(char *word, uint16_t &x, uint16_t &y)
{
    auto twidth = m_tft.textWidth(word, m_font);
    int result = 1;

    if (x+twidth < m_rect.w)
    {
        m_tft.drawString(word, x, y, m_font);
        x += twidth;
        result = 0;
    }
    else
    {
        x = 0;
        y += m_tft.fontHeight(m_font);
        if (y + m_tft.fontHeight(m_font) < m_rect.w)
        {
            m_tft.drawString(word, x, y, m_font);
            x += twidth;
            result = 1;
        }
        else
            result = -1;
    }

    return result;
}

void TextBox::render_text()
{
    char *cur = m_buffer;
    auto x = m_rect.x;
    auto y = m_rect.y;

    m_tft.setTextColor(TFT_GREEN, TFT_RED);

    while (y < (m_rect.y+m_rect.h))
    {
        char *v = 0;

        // If we 
        if (*cur)
        {
            v = terminate_next(cur);

            // Returns with -1 if no room left,
            // 0 if no y change, 1 if y change.
            // (if y changed, we wrapped, so don't
            // need to render the space.)
            int rw_result = render_word(cur, x, y);

            // If there's no room for it, break out
            if (rw_result == -1)
                break;

            // If there's more to render, restore the space
            // char and move our current pointer forward to
            // the next word; Else break out: no more to render
            if (v < m_buffer+m_len)
            {
                *v = ' ';
                cur = v+1;

                // if there's room for the following space, send it
                if (rw_result == 0)
                {
                    rw_result = render_word(const_cast<char*>(" "), x, y);
                    if (rw_result == -1)
                        break;
                }
            }
            else
                break;

        }
        else
            break;
    }

    m_last_x = x;
    m_last_y = y;

    Serial.print("after render, last x, y = ");
    Serial.print(m_last_x);
    Serial.print(",");
    Serial.println(m_last_y);
}

void TextBox::show(void)
{
    Serial.print("text_box showing \"");
    Serial.print(m_buffer);
    Serial.println("\"");
    render_text();
}

// Returns -1 if no more room, 1 if we've wrapped and 0 if we haven't
int TextBox::reserve_space_for_word(char *word, uint16_t &x, uint16_t &y)
{
    auto twidth = m_tft.textWidth(word, m_font);
    int result = 1;

    if (x+twidth < m_rect.w)
    {
        x += twidth;
        result = 0;
    }
    else
    {
        x = 0;
        y += m_tft.fontHeight(m_font);
        if (y + m_tft.fontHeight(m_font) < m_rect.w)
        {
            x += twidth;
            result = 1;
        }
        else
            result = -1;
    }

    return result;
}

void TextBox::reserve_space_for_text()
{
    char *cur = m_buffer;
    auto x = m_rect.x;
    auto y = m_rect.y;

    while (y < (m_rect.y+m_rect.h))
    {
        char *v = 0;

        // If we 
        if (*cur)
        {
            v = terminate_next(cur);

            // Returns with -1 if no room left,
            // 0 if no y change, 1 if y change.
            // (if y changed, we wrapped, so don't
            // need to render the space.)
            int rw_result = reserve_space_for_word(cur, x, y);

            // If there's no room for it, break out
            if (rw_result == -1)
                break;

            // If there's more to render, restore the space
            // char and move our current pointer forward to
            // the next word; Else break out: no more to render
            if (v < m_buffer+m_len)
            {
                *v = ' ';
                cur = v+1;

                // if there's room for the following space, send it
                if (rw_result == 0)
                {
                    rw_result = reserve_space_for_word(const_cast<char*>(" "), x, y);
                    if (rw_result == -1)
                        break;
                }
            }
            else
                break;

        }
        else
            break;
    }

    m_last_x = x;
    m_last_y = y;
}

// Rect TextBox::bounds(void)
// {
// }
