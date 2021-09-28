#include <Arduino.h>

#include <iostream>

#include "control.h"

// We send STATUS
#define STATUS_FMT "{\"spool_id\": %llu, \"temp\": %3.3f, \"humidity\": %3.3f, \"weight\": %3.3f \"thresholds\": \"%s\"}"

// We receive CONFIG
// example:
// {"PLA": { "Humidity": { "min": 10.0, "max": 50.0, "optimal": 30.0} } }
// #define CONFIG_FMT "{\"%s\": {\"%s\": {\"min\": %3.3f, \"max\": %3.3f, \"optimal\": %3.3f}}}"
// #define THRESH_ALERT_FMT "{\"filament\": \"%s\", \"limit\": \"%s\", \"min\": %4.4f, \"max\": %4.4f, \"value\": %4.4f}"

char thresh_state[96];

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

Control::Control(
        Scale &scale,
        Display &display,
        BME280_IF &bme280,
        TagProtocol &tag_protocol,
        FilaConfig *fc,
        OctoProtocol *op) :
    m_tag_val(0UL),
    m_mode(M_Show),
    m_scale(scale),
    m_bme280(bme280),
    m_display(display),

    m_state_view(StateView(m_display, fc, change_view_func, this, m_scale, m_bme280)),
    m_scale_calib_view(CalibView(m_display, fc, change_view_func, this, m_scale)),
    m_config_view(ConfigView(m_display, fc, change_view_func, this)),
    m_filament_view(FilamentView(m_display, fc, change_view_func, this)),
    m_network_view(NetworkView(m_display, fc, change_view_func, this)),

    m_op(op),
    m_tag_protocol(tag_protocol),
    m_thresholds(thresh_cb_func, this)
{
    CallbackData cd;
    cd.cb = touch_callback_func;
    cd.user = this;
    m_display.add_callback(cd);
    m_view = &m_state_view;
    tag_protocol.set_tag_cb(tag_handler_func, this);

    // Terminate thresh_state
    strcpy(thresh_state, "OK");

    if (m_op)
        m_op->set_handler(proto_handler_func, this);
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
    m_tag_val = tag_val;
    m_state_view.set_tag(m_tag_val);
}

void Control::tag_handler_func(char tag[TAG_MSGLEN], void *user)
{ Control *c = reinterpret_cast<Control*>(user); c->tag_handler(tag); }

void Control::thresh_cb(Thresholds::ThreshType tt, const Threshold *thresh, float val)
{
    // Here is where we should alter the display of the view to show we're out-of-bounds
    sprintf(thresh_state, "{\"threshold\": \"%s\" \"val\": %f \"min\": %f \"max\": %f",
            Thresholds::threshold_type_to_name(tt), val, thresh->low, thresh->high);
}

// Buffer for holding replies sent to octoprint-filamon
static char json_buffer[OctoProtocol::MAX_BODY_SIZE];

// Form up a message to tell FilaMon that a threshold has been exceeded
void Control::form_up_and_send_status()
{
    // We received a request for status.  Read the sensors and form up a response
    // in json.
    if (m_op)
    {
        float temp = m_bme280.temp();
        float humidity = m_bme280.humid();
        float grams = m_scale.get_calibrated();
        sprintf(json_buffer, STATUS_FMT, m_tag_val, temp, humidity, grams, thresh_state);
        m_op->send_msg(OctoProtocol::MT_STATUS, strlen(json_buffer), json_buffer);
    }
}

void Control::proto_handler(uint8_t _type, uint16_t len, char*body)
{
    switch(_type)
    {
        case OctoProtocol::MT_STATUS:
        {
            // We received a request for status.  Read the sensors and form up a response
            // in json.
            form_up_and_send_status();
            break;
        }

        // Config configures a single threshold.
        // The config data is json that looks like:
        /*
        {
            "filament_type": "Nylon",
            "Humidity": {"min": 10.0, "max": 30},
            "DryingTemp": {"min": 0.0, "max": 20.0}
        }
        */
        case OctoProtocol::MT_CONFIG:
        {
            StaticJsonDocument<MAX_JSON_SIZE> config_json;
            DeserializationError dse = deserializeJson(config_json, body);
            if (dse)
                return;
            // Save filament type
            strncpy(m_filament_type,
                    config_json["filament_type"],
                    MAX_FILAMENT_TYPE_LEN);

            // Save thresholds
            m_thresholds.set_threshold("Humidity",
                    config_json["Humidity"]["min"],
                    config_json["Humidity"]["max"]);

            m_thresholds.set_threshold("DryingTemp",
                    config_json["DryingTemp"]["min"],
                    config_json["DryingTemp"]["max"]);

            m_thresholds.set_threshold("Weight",
                    config_json["Weight"]["min"],
                    config_json["Weight"]["max"]);
            break;
      }

        // These two don't do qanything _yet_.
        case OctoProtocol::MT_START:
            break;
        case OctoProtocol::MT_STOP:
            break;

        default:
            break;
    }
}

void Control::proto_handler_func(uint8_t _type, uint16_t len, char *body, void *user)
{
    Control *c = reinterpret_cast<Control*>(user);
    c->proto_handler(_type, len, body);
}

void Control::thresh_cb_func(Thresholds::ThreshType t_type, const Threshold *th, float val, void *user)
{
    Control *c = reinterpret_cast<Control*>(user);
    c->thresh_cb(t_type, th, val);
}

void Control::check_thresholds(void)
{
    float temp = m_bme280.temp();
    float humidity = m_bme280.humid();
    float grams = m_scale.get_calibrated();
    m_thresholds.check_threshold("DryingTemp", temp);
    m_thresholds.check_threshold("Humidity", humidity);
    m_thresholds.check_threshold("Weight", grams);
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
            break;
    }

    if (m_op)
        m_op->loop();

    check_thresholds();
}

