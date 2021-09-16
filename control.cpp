#include <Arduino.h>

#include <iostream>

#include "control.h"

bool Control::touch_callback(uint16_t x, uint16_t y, bool pressed)
{
#ifdef DEBUG_TOUCH
    if (pressed)
        Serial.println("Control got touch callback pressed");
    else
        Serial.println("Control got touch callback released");
#endif

    if (m_view)
        return m_view->touch_callback(x, y, pressed);
    else
        return false;
}

bool Control::touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y, bool pressed)
{
    Control *control = reinterpret_cast<Control*>(user);
    return control->touch_callback(x, y, pressed);
}

Control::Control(Scale &scale, Display &display, BME280_IF &bme280, TagProtocol &tag_protocol, FilaConfig *fc) :
    m_mode(M_Show),
    m_scale(scale),
    m_bme280(bme280),
    m_display(display),

    m_state_view(StateView(m_display, fc, change_view_func, this, m_scale, m_bme280)),
    m_scale_calib_view(CalibView(m_display, fc, change_view_func, this, m_scale)),
    m_config_view(ConfigView(m_display, fc, change_view_func, this)),
    m_filament_view(FilamentView(m_display, fc, change_view_func, this)),
    m_network_view(NetworkView(m_display, fc, change_view_func, this)),

    m_view(&m_state_view),
    m_tag_protocol(tag_protocol)
{
    CallbackData cd;
    cd.cb = touch_callback_func;
    cd.user = this;
    m_display.add_callback(cd);
    tag_protocol.set_tag_cb(tag_handler_func, this);
}

void Control::change_view(const char *view_name)
{
    if (!strcmp(view_name, "SETTINGS"))
        m_view = &m_config_view;
    else if (!strcmp(view_name, "STATE"))
        m_view = &m_state_view;
    else if (!strcmp(view_name, "SCALE"))
        m_view = &m_scale_calib_view;
    else if (!strcmp(view_name, "FILAMENT"))
        m_view = &m_filament_view;
    else if (!strcmp(view_name, "NETWORK"))
        m_view = &m_network_view;
    else
        m_view = &m_state_view;

    m_display.get_tft().fillScreen(TFT_BLACK);

    m_mode = M_Show;
}

void Control::change_view_func(const char *viewname, void *user)
{ Control *c = reinterpret_cast<Control*>(user); c->change_view(viewname); }


void Control::tag_handler(char tag[TAG_MSGLEN])
{
    uint64_t tag_val = strtoull(tag, 0, 16);
    std::cout << "RECEIVED TAG " << std::hex << tag_val << std::endl;
}

void Control::tag_handler_func(char tag[TAG_MSGLEN], void *user)
{ Control *c = reinterpret_cast<Control*>(user); c->tag_handler(tag); }


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

