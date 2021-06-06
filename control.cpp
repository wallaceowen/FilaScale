#include <Arduino.h>

#include "control.h"

void Control::disp_callback(Display *d)
{
    Serial.println("Control got display callback");
}

void Control::disp_callback_func(Display *d, void *user)
{
    Control *control = reinterpret_cast<Control*>(user);
    control->disp_callback(d);
}

Control::Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol) :
    m_mode(M_Show_State),
    m_scale(scale),
    m_display(display),
    m_bme280(bme280),
    m_protocol(protocol)
{
    CallbackData cd;
    cd.cb = disp_callback_func;
    cd.user = this;
    m_display.add_callback(cd);
}

void Control::show_state()
{
}

void Control::show_menu()
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
            show_state();
            break;

        case M_Show_Menu:
            show_menu();
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

