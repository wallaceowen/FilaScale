#include <Arduino.h>

#include "control.h"

static char state_buff[sizeof(StateView)];

static void control_button_callback_func(const char *label, void *user, bool pressed);

#ifdef BOZO
void Control::disp_callback(Display *d)
{
    Serial.println("Control got display callback");
}

void Control::disp_callback_func(Display *d, void *user)
{
    Control *control = reinterpret_cast<Control*>(user);
    control->disp_callback(d);
}

void Control::button_callback(const char *label, bool pressed)
{
    if (pressed)
        Serial.println("Control got touch callback pressed");
    else
        Serial.println("Control got touch callback released");
}

void Control::button_callback_func(const char *label, void *user, bool pressed)
{
    Control *control = reinterpret_cast<Control*>(user);
    control->button_callback(label, pressed);
}
#endif

Control::Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol) :
    m_mode(M_Show),
    m_scale(scale),
    m_bme280(bme280),
    m_display(display),
    m_state_view(new(state_buff) StateView(m_display, m_scale, m_bme280)),
    m_view(m_state_view),
    m_protocol(protocol)
{
    // CallbackData cd;
    // cd.cb = touch_callback_func;
    // cd.user = this;
    // m_display.add_callback(cd);

    // m_buttons.add_callback(control_button_callback_func);
}

void Control::show_errors()
{
}

void Control::loop()
{
    switch(m_mode)
    {
        case M_Show:
            m_view->show();
            m_mode = M_Update;
            break;

        case M_Update:
            m_view->loop();
            break;

        case M_Error:
        default:
            show_errors();
            break;
    }
}

