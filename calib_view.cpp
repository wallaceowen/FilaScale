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

ButtonData ok_cancel_bd[] = {
    ButtonData("OK", TFT_BLUE, TFT_BLACK),
    ButtonData("CANCEL", TFT_DARKGREY, TFT_WHITE),
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
            "Put calibration weight on scale",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS),
    m_current_dialog(&m_ask_dialog),
    m_active(false),
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

    m_current_dialog->check_touch(&m_display, x, y, pressed);

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
    if (pressed)
    {
        if (!strcmp(label, "CANCEL"))
        {
            // Tell control to go back to state view
            if (m_change_cb)
                m_change_cb(label, m_change_data);
        }
        else if (!strcmp(label, "OK"))
        {
            switch(m_state)
            {
                case CS_Ask:
                    m_current_dialog = &m_zero_dialog;
                    this->show();
                    m_state = CS_Zero;
                    break;

                case  CS_Zero:
                    m_scale.set_offset();

                    // Now show the gain dialog
                    m_current_dialog = &m_gain_dialog;
                    this->show();
                    m_state = CS_Gain;;
                    break;
                case  CS_Gain:
                    m_scale.set_gain();
                    m_current_dialog = &m_ask_dialog;
                    m_state = CS_Ask;
                    // Tell control to go back to state view
                    if (m_change_cb)
                        m_change_cb(label, m_change_data);
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

// Call this often.  It only does something every UPDATE_INTERVAL.
void CalibView::loop()
{
    // Serial.println("CalibView::loop()");
    TFT_eSPI &tft = m_display.get_tft();
    switch(m_state)
    {
        // case CS_Init:
            // // Clear the display, prompt for operator to clear the scale
            // // for zero
            // // Fill screen with blackness
            // tft.fillRect(0, 0, WIDTH, HEIGHT, SCREEN_BG);
            // m_state = CS_Ask;
            // m_current_dialog = &m_ask_dialog;
            // this->show();
            // // switch state to CS_Ask

        case CS_Ask:
            // m_current_dialog = &m_zero_dialog;
            // m_state = CS_Zero;
            // this->show();
            break;

        case  CS_Zero:
            // Read the scale for a few seconds, save the last value as offset
            // and then show the gain command ("Please set the calibrated weight on the scale")
            // and change m_state to CS_Gain
            break;

        case  CS_Gain:
            // Read the scale for a few seconds, Compute the gain from the last value
            // then switch to CS_Ask
            break;

        default:
            break;
    }
}


