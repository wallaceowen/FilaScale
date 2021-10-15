#include <Arduino.h>
#include <string.h>
#include <stdio.h>

#include "control.h"

// We send STATUS
// {"spool_id": %s, "temp": %s, "humidity": %s, "weight": %llu}

// We receive CONFIG
// example:
// {"scale": { "offset": 10744, "gain": 3.221}}
// #define CONFIG_FMT "{\"%s\": {\"%s\": {\"min\": %3.3f, \"max\": %3.3f, \"optimal\": %3.3f}}}"
// #define THRESH_ALERT_FMT "{\"filament\": \"%s\", \"limit\": \"%s\", \"min\": %4.4f, \"max\": %4.4f, \"value\": %4.4f}"

Control::Control(
        Scale &scale,
        BME280_IF &bme280,
        TagProtocol &tag_protocol,
        OctoProtocol *op) :
    m_scale(scale),
    m_bme280(bme280),

    m_op(op),
    m_tag_protocol(tag_protocol)
{
    strcpy(m_tag_val, "0");

    tag_protocol.set_tag_cb(tag_handler_func, this);

    if (m_op)
        m_op->set_handler(proto_handler_func, this);
}

void Control::tag_handler(char tag[TAG_MSGLEN])
{
    // uint64_t tag_val = strtoull(tag, 0, 16);
    // std::cout << "RECEIVED TAG " << std::hex << tag_val << std::endl;
    // m_tag_val = tag_val;
    strncpy(m_tag_val, tag, TAG_MSGLEN);
}

void Control::tag_handler_func(char tag[TAG_MSGLEN], void *user)
{ Control *c = reinterpret_cast<Control*>(user); c->tag_handler(tag); }

// Buffer for holding replies sent to octoprint-filamon
static char json_buffer[OctoProtocol::MAX_BODY_SIZE];

void add_clause(const char *key, float val, char *buffer)
{
    char formbuff[16];
    *formbuff='\0';
    sprintf(formbuff, "\"%s\": ", key);
    strcat(buffer, formbuff);
    *formbuff='\0';
    dtostrf(val, 4, 3, formbuff);
    strcat(buffer, formbuff);
}

void add_clause(const char *key, char *val, char *buffer)
{
    char formbuff[24];
    sprintf(formbuff, "\"%s\": %s", key, val);
    strcat(buffer, formbuff);
}

// Form up a message to tell FilaMon that a threshold has been exceeded
void Control::form_up_and_send_status()
{
    // We received a request for status.  Read the sensors and form up a response
    // in json.
    if (m_op)
    {
        strcpy(json_buffer, "{");
        // {'spool_id': 0, 'temp': 0.0, 'humidity': 0, 'weight': 2.6968924e+16}
        add_clause("spool_id", m_tag_val, json_buffer);
        strcat(json_buffer, ", ");
        add_clause("temp", m_bme280.temp(), json_buffer);
        strcat(json_buffer, ", ");
        add_clause("humidity", m_bme280.humid(), json_buffer);
        strcat(json_buffer, ", ");
        add_clause("weight", m_scale.get_calibrated(), json_buffer);
        strcat(json_buffer, "}");
        m_op->send_msg(OctoProtocol::MT_STATUS, strlen(json_buffer), json_buffer);
    }
}

// Form up a message to tell FilaMon that a threshold has been exceeded
void Control::form_up_and_send_config()
{
    // We received a request for status.  Read the sensors and form up a response
    // in json.
    if (m_op)
    {
        char gainstr[10];
        float gain = m_scale.get_gain();
        dtostrf(gain, 4, 3, gainstr);
        sprintf(json_buffer, "{\"scale\": {\"offset\": %ld, \"gain\": %s}",
            m_scale.get_offset(), gainstr);
        m_op->send_msg(OctoProtocol::MT_CONFIG, strlen(json_buffer), json_buffer);
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

        case OctoProtocol::MT_CONFIG:
        {
            DeserializationError dse = deserializeJson(m_config_json, body);
            if (dse)
                return;

            // Save scale calibration values "offset" and "gain"
            m_scale.set_offset(m_config_json["scale"]["offset"]);
            m_scale.set_gain(m_config_json["scale"]["gain"]);

            // Tell plugin what we've set
            form_up_and_send_config();
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

void Control::loop()
{
    if (m_op)
        m_op->loop();

    // form_up_and_send_status();
}

