// text_box.h

#ifndef __text_button_h
#define __text_button_h

#include <stdint.h>

#include "display.h"
#include "rect.h"
#include "point.h"

#define TB_BUFFER_SIZE 500

class TextBox
{
public:
    // Provide a box into which we can render text.
    // wraps long lines.
    TextBox(Display &d, const Rect &w, uint16_t font, const char *txt, uint16_t fg, uint16_t bg);

    // Render it
    void show();

    // Get it's contents
    const char *buffer(void) const { return m_buffer; }

    // Get the last position written to
    Point last_pos(void) const { return Point(m_last_x, m_last_y); }
    uint16_t last_y(void) const { return m_last_y; }

private:
    // Figure out how much room we need for the text
    void reserve_space_for_text();
    // Helper for above
    int reserve_space_for_word(char *word, uint16_t &x, uint16_t &y);

    // Returns -1 if no more room, 1 if we've wrapped and 0 if we haven't
    int render_word(char *word, uint16_t &x, uint16_t &y, bool actual);
    void render_text();

    TFT_eSPI &m_tft;
    Rect      m_rect;
    uint16_t  m_font;
    uint16_t  m_max_rows;
    uint16_t  m_len;
    uint16_t  m_last_x;
    uint16_t  m_last_y;
    char      m_buffer[TB_BUFFER_SIZE];
    uint16_t  m_fg;
    uint16_t  m_bg;
};

#endif

