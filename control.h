
#ifndef __control_h
#define __control_h

#include "bme280_if.h"
#include "display.h"
#include "protocol.h"
#include "scale.h"

class Control
{
public:
    Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol);
    void loop();
private:
    Scale &m_scale;
    Display &m_display;
    BME280_IF &m_bme280;
    Protocol &m_protocol;
};

#endif
