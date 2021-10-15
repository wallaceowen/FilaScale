
#ifndef __control_h
#define __control_h

#include <stdint.h>
#include <ArduinoJson.h>

#include "bme280_if.h"
#include "tag_protocol.h"
#include "scale.h"
#include "octo_protocol.h"
#include "thresholds.h"

#define MAX_JSON_SIZE 256
#define MAX_FILAMENT_TYPE_LEN 16

class Control
{
public:

    Control(Scale &scale,
            BME280_IF &bme280,
            TagProtocol &tag_protocol,
            OctoProtocol *op);
    void loop();

private:

    void form_up_and_send_status();
    void form_up_and_send_config();

    static void tag_handler_func(char tag[TAG_MSGLEN], void *user);
    void tag_handler(char tag[TAG_MSGLEN]);

    static void proto_handler_func(uint8_t _type, uint16_t len, char*msg, void *user);
    void proto_handler(uint8_t _type, uint16_t len, char*msg);

    static void thresh_cb_func(Thresholds::ThreshType, const Threshold*, float, void*);
    void thresh_cb(Thresholds::ThreshType, const Threshold*, float);

    char          m_tag_val[TAG_MSGLEN+1];
    Scale        &m_scale;
    BME280_IF    &m_bme280;
    OctoProtocol *m_op;
    TagProtocol  &m_tag_protocol;
    StaticJsonDocument<MAX_JSON_SIZE> m_config_json;
};

#endif

