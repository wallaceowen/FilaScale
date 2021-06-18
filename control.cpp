#include <Arduino.h>

#include "control.h"

static char state_buff[sizeof(StateView)];
static char calib_buff[sizeof(CalibView)];
static char config_buff[sizeof(ConfigView)];

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
    m_config_view(new(config_buff) ConfigView(m_display, config_cb_func, this)),
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
    // Serial.print("Control::calib_cb got ");
    // Serial.println(result);
    // Not currently doing anything differently, but we might
    // want to let them know they cancelled calibration and the
    // ramifications
    if ((!strcmp(result, "CANCEL")) || (!strcmp(result, "DONE")))
    {
        this->change_view("STATE");
    }
}

void Control::config_cb(const char *result)
{
    // Serial.print("Control::config_cb got ");
    // Serial.println(result);
    // Not currently doing anything differently, but we might
    // want to let them know they cancelled calibration and the
    // ramifications
    if ((!strcmp(result, "CANCEL")) || (!strcmp(result, "DONE")))
    {
        this->change_view("STATE");
    }
}

void Control::calib_cb_func(const char *viewname, void *user)
{ Control *c = reinterpret_cast<Control*>(user); c->calib_cb(viewname); }

void Control::config_cb_func(const char *viewname, void *user)
{ Control *c = reinterpret_cast<Control*>(user); c->config_cb(viewname); }

void Control::change_view(const char *view_name)
{
    // Serial.print("Control::change_view got ");
    // Serial.println(view_name);

    if (!strcmp(view_name, "CAL"))
    {
        m_view = m_calib_view;
        m_mode = M_Show;
    }
    else if (!strcmp(view_name, "CFG"))
    {
        m_view = m_config_view;
        m_mode = M_Show;
    }
    else if (!strcmp(view_name, "STATE"))
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

