// calib_view.cpp

#include <stdio.h>

#include "stock_buttons.h"
#include "calib_view.h"

#ifdef SMALL_DIALOG
#define DLG_X 18
#define DLG_Y 18
#define DLG_HEIGHT 160
#define DLG_WIDTH 270
#else
#define DLG_X 0
#define DLG_Y 0
#define DLG_HEIGHT 240
#define DLG_WIDTH 320
#endif

#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2

// static const char *state_names[] = { "CS_Ask", "CS_Zero", "CS_Gain", "CS_NUmStates" };

CalibView::CalibView(Display &d, FilaConfig *fc, ViewChangeCallback ccb, void *change_user_data, Scale &s) :
    View(d, fc, ccb, change_user_data),
    m_display(d),
    m_state(CS_Ask),
    m_ask_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Calibrate scale?",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS,
            Menu::O_Horiz, TFT_WHITE, TFT_DARKGREY),
    m_zero_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Remove all from scale",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS,
            Menu::O_Horiz, TFT_WHITE, TFT_DARKGREY),
    m_gain_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            "Put cal wt. on scale",
            "Press OK or CANCEL",
            ok_cancel_bd, NUM_OK_BUTTONS,
            Menu::O_Horiz, TFT_WHITE, TFT_DARKGREY),
    m_current_dialog(&m_ask_dialog),
    m_scale(s)
{
    m_ask_dialog.set_callback(menu_callback_func, this);
    m_zero_dialog.set_callback(menu_callback_func, this);
    m_gain_dialog.set_callback(menu_callback_func, this);
}

bool CalibView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
    return m_current_dialog->check_touch(x, y, pressed);
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
                m_change_cb("SETTINGS", m_change_data);
        }
        else if (!strcmp(label, "OK"))
        {
            // Switch to the next state
            switch(m_state)
            {
                case CS_Ask:
                    set_state(CS_Zero);
                    m_change_cb("SCALE", m_change_data);
                    break;

                case  CS_Zero:
                    m_scale.set_offset();
                    set_state(CS_Gain);
                    m_change_cb("SCALE", m_change_data);
                    break;
                case  CS_Gain:
                    m_scale.set_gain();

                    // Save calibration values in config
                    m_change_cb("SCALE", m_change_data);

                    set_state(CS_Ask);
                    // Tell control to go back to state view

                    if (m_change_cb)
                        m_change_cb("STATE", m_change_data);
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
    // Show the initial dialog
    m_current_dialog->show();
}

bool CalibView::update()
{
    return true;
}

void CalibView::loop()
{
}

