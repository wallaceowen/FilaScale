
#include "kbd_dialog.h"

#include <string.h>

/*
 * +-------+---------+
 * |value  | 7  8  9 |
 * +-------+ 4  5  6 |
 * | CANCL | 1  2  3 |
 * | ENTER | .  0 BS |
 * +-------+---------+
 */

#define KBD_ROWS 5
#define KBD_COLS 6

// #define BUTTON_WIDTH 20
// #define BUTTON_HEIGHT 20

#define KB_BG TFT_DARKGRAY
#define KB_FG TFT_WHITE

#define VALUE_FONT 4
#define VALUE_MARGIN_X 6
#define VALUE_MARGIN_Y 6
#define VALUE_WIDTH 200
#define VALUE_BOX_COLOR TFT_YELLOW
#define VALUE_COLOR_FG TFT_GREEN
#define VALUE_COLOR_BG TFT_BLACK

struct KB_Data
{
    KB_Data(const char *l, uint16_t r, uint16_t c, uint16_t w, uint16_t h, uint16_t f, uint16_t b)
        : row(r), col(c), width(w), height(h), fg(f), bg(b)
    {
        strcpy(label, l);
    }
    char label[8];
    unsigned row, col;
    uint16_t width, height;
    uint16_t fg, bg;
};


static KB_Data keyb_data[] = {
    {KB_Data("7",      1, 3, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("8",      1, 4, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("9",      1, 5, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("4",      2, 3, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("5",      2, 4, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("6",      2, 5, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("1",      3, 3, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("2",      3, 4, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("3",      3, 5, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data(".",      4, 3, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("0",      4, 4, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("BS",     4, 5, 1, 1, TFT_WHITE, TFT_DARKGREY)},
    {KB_Data("ENTER",  3, 0, 3, 1, TFT_WHITE, TFT_DARKGREEN)},
    {KB_Data("CANCEL", 4, 0, 3, 1, TFT_WHITE, TFT_RED)},
};
#define NUM_BUTTONS (sizeof(keyb_data)/sizeof(keyb_data[0]))

KbdDialog::KbdDialog(Display &d, const Rect &rect) :
    GridDialog(d, rect, 0, 0, KBD_ROWS, KBD_COLS),
    m_cb(0),
    m_user_data(0)
{
    // Clear the value
    memset(value, 0, sizeof(value));

    // TFT_eSPI &tft = m_display.get_tft();

    this->make_buttons();
    // this->set_callback(kbd_button_callback_func, this);
    m_buttons.set_callback(kbd_button_callback_func, this);
}

// void KbdDialog::menu_callback_func(const char *label, bool pressed, void *user_data)
// {
    // KbdDialog *kd = reinterpret_cast<KbdDialog*>(user_data);
    // kd->menu_callback(label, pressed);
// }

// This will save the func in the client.  We're installing KbdDialog::menu_callback_func
// to be invoked by the buttons instance, and our func wiil in turn fire off the callback
// we stash here.
void KbdDialog::set_callback(PressEventCB cb, void*user_data)
{
    m_cb = cb;
    m_user_data = user_data;
}

void KbdDialog::make_buttons(void)
{
    for (unsigned b = 0; b < NUM_BUTTONS; ++b)
    {
        // Cell widths are a multiple of cell size;
        // cell size is dialog width/num_cols, dilaog height/num_rows
        uint16_t w = m_rect.w/KBD_COLS*keyb_data[b].width;
        uint16_t h = m_rect.h/KBD_ROWS*keyb_data[b].height;
#ifdef DEBUG_KBD
        {
            char buff[65];
            sprintf(buff, "kbd_dialog making button %s at %u, %u, %u, %u",
                    keyb_data[b].label,
                    keyb_data[b].row,
                    keyb_data[b].col,
                    w, h);
            Serial.println(buff);
        }
#endif
        m_buttons.add_button(
                ButtonData(
                    keyb_data[b].label,
                    keyb_data[b].bg,
                    keyb_data[b].fg,
                    keyb_data[b].bg),
                keyb_data[b].row,
                keyb_data[b].col,
                w, h);
    }
}

void KbdDialog::draw_value()
{
    TFT_eSPI &tft = m_display.get_tft();

    tft.setTextColor(VALUE_COLOR_FG, VALUE_COLOR_BG);
    char buffer[40];
    sprintf(buffer, "%s   ", value);
    tft.drawString(buffer, m_rect.x+VALUE_MARGIN_X, m_rect.y+VALUE_MARGIN_Y, VALUE_FONT);

    // Draw the value frame
    tft.drawRect(m_rect.x, m_rect.y, VALUE_WIDTH, tft.fontHeight(VALUE_FONT)+4, VALUE_BOX_COLOR);
}

bool KbdDialog::check_touch(uint16_t x, uint16_t y, bool pressed)
{
    int button_touched = this->GridDialog::check_touch(x, y, pressed)?1:0;
#ifdef DEBUG_KBD
    Serial.print("KbdDialog::check_touch(): ");
    Serial.println(button_touched);
#endif
    return button_touched?true:false;
}

void KbdDialog::show(void)
{
#ifdef DEBUG_KBD
    Serial.print("KbdDialog::show() calling GridDialog::show");
#endif
    this->GridDialog::show();
    // draw_value();
#ifdef DEBUG_KBD
    Serial.print("back from  calling GridDialog::show()");
#endif
}

void KbdDialog::kbd_button_callback(const char *label, bool pressed)
{
    {
        char buff[65];
        sprintf(buff,
                "kbd_button_callback got \"%s\" %s",
                label, pressed?"pressed":"released");
        Serial.println(buff);
    }
    if ((isdigit(*label) || *label == '.') && !pressed)
    {
        strcat(value, label);
        draw_value();
        delay(200);
    }
    else if (!strcmp(label, "BS") && !pressed)
    {
        unsigned len = strlen(value);
        if (len)
        {
            value[len-1] = '\0';
            draw_value();
            delay(200);
        }
    }
    else if (!strcmp(label, "CANCEL") && !pressed)
    {
        if (m_cb)
            m_cb("STATE", pressed, m_user_data);
    }
    else if (!strcmp(label, "ENTER") && !pressed)
    {
        if (m_cb)
            m_cb("STATE", pressed, m_user_data);
    }
    
}

void KbdDialog::kbd_button_callback_func(const char *label, bool pressed, void *user_data)
{
    KbdDialog *kd = reinterpret_cast<KbdDialog*>(user_data);
    kd->kbd_button_callback(label, pressed);
}
