// text_box.cpp

#include "text_box.h"

TextBox::TextBox(Display &d, const Rect &r, uint16_t font, const char *txt, uint16_t fg, uint16_t bg) :
    m_tft(d.get_tft()),
    m_rect(r),
    m_font(font),
    m_max_rows(m_rect.w/m_tft.fontHeight(m_font)),
    m_len(strlen(txt)), m_last_x(0), m_last_y(0),
    m_fg(fg),
    m_bg(bg),
    m_buffer(new char[m_len+1])
{
    memset(m_buffer, 0, m_len);
    strcpy(m_buffer, txt);

    // pass false to compute how much height we need
    render_text(false);
}

TextBox::~TextBox(void)
{
    delete [] m_buffer;
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
int TextBox::render_word(char *word, uint16_t &x, uint16_t &y, bool actual)
{
    auto twidth = m_tft.textWidth(word, m_font);
    int result = 1;

    if (x-m_rect.x+twidth < m_rect.w)
    {
        if (actual)
            m_tft.drawString(word, x, y, m_font);
        x += twidth;
        result = 0;
    }
    else
    {
        x = 0;
        y += m_tft.fontHeight(m_font);
        if (y + m_tft.fontHeight(m_font) < m_rect.h)
        {
            if (actual)
                m_tft.drawString(word, x, y, m_font);
            x += twidth;
            result = 1;
        }
        else
            result = -1;
    }

    return result;
}

void TextBox::render_text(bool actual)
{
    char *cur = m_buffer;
    auto x = m_rect.x;
    auto y = m_rect.y;

    if (actual)
        m_tft.setTextColor(m_fg, m_bg);

    while (y < (m_rect.y+m_rect.h))
    {
        char *v = 0;

        // If we have more to render
        if (*cur)
        {
            // terminate the next word and point v at it
            v = terminate_next(cur);

            // Returns with -1 if no room left,
            // 0 if no y change, 1 if y change.
            // (if y changed, we wrapped, so don't
            // need to render the space.)
            int rw_result = render_word(cur, x, y, actual);

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

                rw_result = render_word(const_cast<char*>(" "), x, y, true);
                if (rw_result == -1)
                    break;
            }
            else
                break;

        }
        else
            break;
    }

    m_last_x = x-m_rect.x;
    m_last_y = y + m_tft.fontHeight(m_font);
}

void TextBox::show(void)
{
    render_text(true);
}

