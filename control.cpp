#include <Arduino.h>

#include "control.h"

static char state_buff[sizeof(StateView)];
static char calib_buff[sizeof(CalibView)];

void Control::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
#ifdef DEBUG_TOUCH
    if (pressed)
        Serial.println("Control got touch callback pressed");
    else
        Serial.println("Control got touch callback released");
#endif

    if (m_view)
        m_view->touch_callback(x, y, pressed);
}

void Control::touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y, bool pressed)
{
    Control *control = reinterpret_cast<Control*>(user);
    control->touch_callback(x, y, pressed);
}

Control::Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol) :
    m_mode(M_Show),
    m_scale(scale),
    m_bme280(bme280),
    m_display(display),
    m_state_view(new(state_buff) StateView(m_display, change_view_func, this, m_scale, m_bme280)),
    m_calib_view(new(calib_buff) CalibView(m_display, calib_cb_func, this, m_scale)),
    m_protocol(protocol),
    m_view(m_state_view)
{
    CallbackData cd;
    cd.cb = touch_callback_func;
    cd.user = this;
    m_display.add_callback(cd);
}

void Control::calib_cb(const char *result)
{
    Serial.print("Control::calib_cb got ");
    Serial.println(result);
}

void Control::calib_cb_func(const char *viewname, void *user)
{ Control *c = reinterpret_cast<Control*>(user); c->calib_cb(viewname); }

void Control::change_view(const char *view_name)
{
    Serial.print("Control::change_view got ");
    Serial.println(view_name);

    if (!strcmp(view_name, "CAL"))
    {
        m_view = m_calib_view;
        m_mode = M_Show;
    }
}
void Control::change_view_func(const char *viewname, void *user)
{ Control *c = reinterpret_cast<Control*>(user); c->change_view(viewname); }


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
            break;
    }
}

