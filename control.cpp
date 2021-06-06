
#include "control.h"

Control::Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol) :
    m_scale(scale),
    m_display(display),
    m_bme280(bme280),
    m_protocol(protocol)
{
}

void Control::loop()
{
}

