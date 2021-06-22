// screencal_view.cpp

#include <stdio.h>

#include "stock_buttons.h"
#include "screencal_view.h"

#define SCREENCAL_Y 10
#define DLG_X 18
#define DLG_Y 18
#define DLG_HEIGHT 160
#define DLG_WIDTH 270

#define WIDTH 320
#define HEIGHT 240
#define VALUES_X 148
#define SCREENCAL_FONT 4
#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2

#define SCREEN_BG TFT_BLACK

static const char *state_names[] = { "CS_Ask", "CS_Zero", "CS_Gain", "CS_NUmStates" };

ScreenCalView::ScreenCalView(Display &d, ViewChangeCallback ccb, void *change_user_data) :
    View(d, ccb, change_user_data),
    m_display(d),
    m_state(CS_Ask),
    m_ask_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Calibrate screen?",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS)
{
    m_ask_dialog.set_callback(menu_callback_func, this);

    // Clear the display
    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(0, 0, WIDTH, HEIGHT, SCREEN_BG);
}

void ScreenCalView::screencal_dialog_callback(const char *label, bool pressed)
{
    Serial.print("ScreenCal dialog callback got \"");
    Serial.print(label),
    Serial.print("\" ");
    Serial.println(pressed?"PRESSED":"RELEASED");
}

void ScreenCalView::screencal_dialog_callback_func(const char *label, bool pressed, void *user_data)
{
    ScreenCalView *cv = reinterpret_cast<ScreenCalView*>(user_data);
    cv->screencal_dialog_callback(label, pressed);
}

void ScreenCalView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
#ifdef DEBUG_TOUCH
    Serial.print("ScreenCalView got touch callback. x: ");
    Serial.print(x);
    Serial.print(", y: ");
    Serial.println(y);
    Serial.println("checking buttons");
#endif

    m_ask_dialog.check_touch(x, y, pressed);

}

void ScreenCalView::menu_callback(const char *label, bool pressed)
{
    Serial.print("ScreenCal menu callback got \"");
    Serial.print(label),
    Serial.print("\" state = ");
    Serial.print(state_names[m_state]),
    Serial.print(" pressed = ");
    Serial.println(pressed?"PRESSED":"RELEASED");


    // Here we check m_state to see what state to switch to,
    // then deal with switching to that state
    if (!pressed)
    {
        if (!strcmp(label, "CANCEL"))
        {
            // Tell control to go back to state view
            if (m_change_cb)
                m_change_cb("CANCEL", m_change_data);
        }

        else if (!strcmp(label, "OK"))
        {
            this->show();
        }
    }
}

void ScreenCalView::menu_callback_func(const char *label, bool pressed, void *user_data)
{
    ScreenCalView *cv = reinterpret_cast<ScreenCalView*>(user_data);
    cv->menu_callback(label, pressed);
}


// Show the static part of the view
void ScreenCalView::show()
{
    Serial.println("ScreenCalView::show()");
    // TFT_eSPI &tft = m_display.get_tft();

    // Show the initial dialog
    m_ask_dialog.show();
}

bool ScreenCalView::update()
{
    Serial.println("ScreenCalView::update()");
    return true;
}

void ScreenCalView::loop()
{
#ifdef SCREENCALVIEW_LOOP_HAS_A_JOB
    Serial.println("ScreenCalView::loop()");
    TFT_eSPI &tft = m_display.get_tft();
    switch(m_state)
    {
        case CS_Init:
            break;

        case CS_Ask:
            break;

        case  CS_Zero:
            break;

        case  CS_Gain:
            break;

        default:
            break;
    }
#endif
}


