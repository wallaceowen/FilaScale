// calib_view.cpp

#include <stdio.h>

#include "calib_view.h"

#define CALIB_Y 10
#define DLG_X 18
#define DLG_Y 18
#define DLG_HEIGHT 160
#define DLG_WIDTH 270

#define WIDTH 320
#define HEIGHT 240
#define VALUES_X 148
#define CALIB_FONT 4
#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2

#define SCREEN_BG TFT_BLACK

static ButtonData ok_cancel_bd[] = {
    ButtonData("OK", TFT_DARKGREEN, TFT_WHITE, TFT_DARKGREEN),
    ButtonData("CANCEL", TFT_RED, TFT_WHITE, TFT_RED),
};
#define NUM_OK_BUTTONS (sizeof(ok_cancel_bd)/sizeof(ok_cancel_bd[0]))

static const char *state_names[] = { "CS_Ask", "CS_Zero", "CS_Gain", "CS_NUmStates" };

CalibView::CalibView(Display &d, ViewChangeCallback ccb, void *change_user_data, Scale &s) :
    View(d, ccb, change_user_data),
    m_display(d),
    m_state(CS_Ask),
    m_ask_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Calibrate scale?",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS),
    m_zero_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Remove all from scale",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS),
    m_gain_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Put cal wt. on scale",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS),
    m_current_dialog(&m_ask_dialog),
    m_scale(s)
{
    m_ask_dialog.set_callback(menu_callback_func, this);
    m_zero_dialog.set_callback(menu_callback_func, this);
    m_gain_dialog.set_callback(menu_callback_func, this);

    // Clear the display
    TFT_eSPI &tft = m_display.get_tft();
    tft.fillRect(0, 0, WIDTH, HEIGHT, SCREEN_BG);
}

void CalibView::calib_dialog_callback(const char *label, bool pressed)
{
    Serial.print("Calib dialog callback got \"");
    Serial.print(label),
    Serial.print("\" ");
    Serial.println(pressed?"PRESSED":"RELEASED");
}

void CalibView::calib_dialog_callback_func(const char *label, bool pressed, void *user_data)
{
    CalibView *cv = reinterpret_cast<CalibView*>(user_data);
    cv->calib_dialog_callback(label, pressed);
}

void CalibView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
#ifdef DEBUG_TOUCH
    Serial.print("CalibView got touch callback. x: ");
    Serial.print(x);
    Serial.print(", y: ");
    Serial.println(y);
    Serial.println("checking buttons");
#endif

    m_current_dialog->check_touch(x, y, pressed);

}

void CalibView::set_state(CalibState cs)
{
    m_state = cs;
    switch (m_state)
    {
        case CS_Ask:
            m_current_dialog = &m_ask_dialog;
            break;
        case CS_Zero:
            m_current_dialog = &m_zero_dialog;
           break;
        case CS_Gain:
            m_current_dialog = &m_gain_dialog;
            break;
        default:
            m_state = CS_Ask;
            m_current_dialog = &m_ask_dialog;
           break;
    }
}

void CalibView::menu_callback(const char *label, bool pressed)
{
    Serial.print("Calib menu callback got \"");
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
            // Reset state to Ask
            set_state(CS_Ask);
            // Tell control to go back to state view
            if (m_change_cb)
                m_change_cb(label, m_change_data);
        }
        else if (!strcmp(label, "OK"))
        {
            // Switch to the next state
            switch(m_state)
            {
                case CS_Ask:
                    set_state(CS_Zero);
                    this->show();
                    break;

                case  CS_Zero:
                    m_scale.set_offset();
                    set_state(CS_Gain);
                    this->show();
                    break;
                case  CS_Gain:
                    m_scale.set_gain();

                    set_state(CS_Ask);
                    // Tell control to go back to state view
                    if (m_change_cb)
                        m_change_cb("DONE", m_change_data);
                    break;

                default:
                    break;
            }
        }
    }
}

void CalibView::menu_callback_func(const char *label, bool pressed, void *user_data)
{
    CalibView *cv = reinterpret_cast<CalibView*>(user_data);
    cv->menu_callback(label, pressed);
}


// Show the static part of the view
void CalibView::show()
{
    Serial.println("CalibView::show()");
    TFT_eSPI &tft = m_display.get_tft();

    // Show the initial dialog
    m_current_dialog->show();
}

bool CalibView::update()
{
    Serial.println("CalibView::update()");
    return true;
}

void CalibView::loop()
{
#ifdef CALIBVIEW_LOOP_HAS_A_JOB
    Serial.println("CalibView::loop()");
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


