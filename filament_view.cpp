// filament_view.cpp

#include <stdio.h>

#include "stock_buttons.h"
#include "filament_view.h"

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

#define FILINTRO_FONT 4

static const char filament_intro_title[] = "Filament Settings";
static const char filament_intro_body[] = "These dialogs are used for setting filament drying temperature, "
"humidity alert levels, and defining new filament types.";


// The currently imagined filament view, but incomlete needs nore attention to flesh it out and
// integrate it with configuration over the serial interface via octoprint
FilamentView::FilamentView(Display &d, FilaConfig *fc, ViewChangeCallback ccb, void *change_user_data) :
    View(d, fc, ccb, change_user_data),
    m_display(d),
    m_state(FS_Introduce),
    m_filintro_dialog(d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT),
            filament_intro_title,
            filament_intro_body,
            ok_cancel_bd, NUM_OK_BUTTONS,
            Menu::O_Horiz,
            TFT_BLACK, TFT_LIGHTGREY, TITLE_FONT, PROMPT_FONT
            ),
    m_filament_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT)),
    m_filadjust_dialog(
            d,
            Rect(DLG_X, DLG_Y, DLG_WIDTH, DLG_HEIGHT), "PLA"),
    m_current_dialog(&m_filintro_dialog)
{
    m_filintro_dialog.set_callback(menu_callback_func, this);
    m_filament_dialog.set_callback(menu_callback_func, this);
    m_filadjust_dialog.set_callback(menu_callback_func, this);
}

bool FilamentView::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
    return m_current_dialog->check_touch(x, y, pressed);
}

void FilamentView::set_state(FilamentState cs)
{
    m_state = cs;
    switch (m_state)
    {
        case FS_Introduce:
            m_current_dialog = &m_filintro_dialog;
            break;
        case FS_Offer:
            m_current_dialog = &m_filament_dialog;
            break;
        case FS_Adjust:
            m_current_dialog = &m_filadjust_dialog;
           break;
        default:
            m_state = FS_Introduce;
            m_current_dialog = &m_filintro_dialog;
           break;
    }
}

const char *filanames[] = { "PLA", "ABS", "ASA", "PETG", "Nylon", "TPU" };
#define NUM_FILANAMES (sizeof(filanames)/sizeof(filanames[0]))

bool valid_fila_name(const char *label)
{
    for (unsigned i = 0; i < NUM_FILANAMES; ++i)
        if (!strcmp(label, filanames[i]))
            return true;
    return false;
}

void FilamentView::menu_callback(const char *label, bool pressed)
{
    // Here we check m_state to see what state to switch to,
    // then deal with switching to that state
    if (!pressed)
    {
        if (!strcmp(label, "CANCEL"))
        {
            // Reset state to Ask
            set_state(FS_Introduce);

            // Tell control to go back to state view
            if (m_change_cb)
                m_change_cb("SETTINGS", m_change_data);
        }
        else if (!strcmp(label, "OK"))
        {
            // Switch to the next state
            switch(m_state)
            {
                case FS_Introduce:
                    set_state(FS_Offer);
                    m_change_cb("FILAMENT", m_change_data);
                    break;

                // No-op - stay in this state
                case FS_Offer:
                    set_state(FS_Adjust);
                    m_change_cb("FILAMENT", m_change_data);
                    break;

                // OK on FS_Adjust means user is done adjusting
                case  FS_Adjust:
                    set_state(FS_Offer);
                    m_change_cb("FILAMENT", m_change_data);
                    break;

                default:
                    break;
            }
        }

        else if (m_state == FS_Offer)
        {
            if (valid_fila_name(label))
            {
                set_state(FS_Adjust);
                m_filadjust_dialog.set_filament_name(label);
            }
            m_change_cb("FILAMENT", m_change_data);
        }

        else if (m_state == FS_Adjust)
        {
            m_filadjust_dialog.set_filament_name(label);
            m_change_cb("FILAMENT", m_change_data);
        }
    }
}

void FilamentView::menu_callback_func(const char *label, bool pressed, void *user_data)
{
    FilamentView *cv = reinterpret_cast<FilamentView*>(user_data);
    cv->menu_callback(label, pressed);
}


// Show the static part of the view
void FilamentView::show()
{
    // Show the initial dialog
    m_current_dialog->show();
}

bool FilamentView::update()
{
    return true;
}

void FilamentView::loop()
{
    m_current_dialog->loop();
}

