#include <Arduino.h>

#include "control.h"

static char state_buff[sizeof(StateView)];
static char config_buff[sizeof(ConfigView)];
static char calib_buff[sizeof(CalibView)];
static char filament_buff[sizeof(FilamentView)];
static char network_buff[sizeof(NetworkView)];

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
    m_scale_calib_view(new(calib_buff) CalibView(m_display, change_view_func, this, m_scale)),
    m_config_view(new(config_buff) ConfigView(m_display, change_view_func, this)),
    m_filament_view(new(filament_buff) FilamentView(m_display, change_view_func, this)),
    m_network_view(new(network_buff) NetworkView(m_display, change_view_func, this)),
    m_view(m_state_view),
    m_protocol(protocol)
{
    CallbackData cd;
    cd.cb = touch_callback_func;
    cd.user = this;
    m_display.add_callback(cd);
}

void Control::change_view(const char *view_name)
{
    Serial.print("Control::change_view got ");
    Serial.println(view_name);

    if (!strcmp(view_name, "SETTINGS"))
    {
        m_view = m_config_view;
        m_mode = M_Show;
    }
    else if (!strcmp(view_name, "STATE"))
    {
        m_view = m_state_view;
        m_mode = M_Show;
    }
    else if (!strcmp(view_name, "SCALE"))
    {
        m_view = m_scale_calib_view;
        m_mode = M_Show;
    }
    else if (!strcmp(view_name, "FILAMENT"))
    {
        m_view = m_filament_view;
        m_mode = M_Show;
    }
    else if (!strcmp(view_name, "NETWORK"))
    {
        m_view = m_network_view;
        m_mode = M_Show;
    }

    // This is the other button on the state screen.
    // I don't know what 'stop' means.  So it just basically
    // gets ignored for now, just some code here that catches
    // it and sets the view to the state view, which is the
    // only view with the stop button.  So duh.
    else if (!strcmp(view_name, "STOP"))
    {
        m_view = m_state_view;
        m_mode = M_Show;
    }
    // Default to state view
    else
    {
        m_view = m_state_view;
        m_mode = M_Show;
    }

    TFT_eSPI &tft = m_display.get_tft();
    tft.fillScreen(TFT_BLACK);

    m_view->show();

    // Serial.println("Control::change_view back from show()");
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

