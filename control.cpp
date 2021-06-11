#include <Arduino.h>

#include "control.h"

static char state_buff[sizeof(StateView)];

void Control::disp_callback(Display *d)
{
    Serial.println("Control got display callback");
}

void Control::disp_callback_func(Display *d, void *user)
{
    Control *control = reinterpret_cast<Control*>(user);
    control->disp_callback(d);
}

void Control::touch_callback(Display *d, uint16_t x, uint16_t y)
{
    Serial.println("Control got touch callback");
}

void Control::touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y)
{
    Control *control = reinterpret_cast<Control*>(user);
    control->touch_callback(d, x, y);
}

Control::Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol) :
    m_mode(M_Show_State),
    m_scale(scale),
    m_bme280(bme280),
    m_display(display),
    m_state_view(*new(state_buff) StateView(m_display, m_scale, m_bme280)),
    m_protocol(protocol)
{
    CallbackData cd;
    cd.cb = touch_callback_func;
    cd.user = this;
    m_display.add_callback(cd);
}

// void Control::update_state()
// {
    // static auto future = 0;
    // auto now = millis();

    // m_state_view.loop();

    // if (now > future)
    // {
        // future = now+500;
        // m_state_view.render();
    // }
// }

void Control::show_menu()
{
    // ...
}

void Control::update_menu()
{
}

void Control::adjust_bme()
{
}

void Control::calibrate_scale()
{
}

void Control::show_errors()
{
}

void Control::loop()
{
    switch(m_mode)
    {
        case M_Show_State:
            m_state_view.show();
            m_mode = M_Update_State;
            break;

        case M_Update_State:
            m_state_view.loop();
            break;

        case M_Show_Menu:
            show_menu();
            m_mode = M_Update_Menu;
            break;

        case M_Update_Menu:
            update_menu();
            break;

        case M_Adjust_BME:
            adjust_bme();
            break;

        case M_CalibrateScale:
            calibrate_scale();
            break;

        case M_Error_State:
        default:
            show_errors();
            break;

            break;
    }
}

