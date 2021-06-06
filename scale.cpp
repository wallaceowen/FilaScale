/* main.cpp */

#include <Arduino.h>
#include <inttypes.h>
#include <string.h>

#include "scale.h"
#include "HX711.h"
#include "version.h"

// #include <avr/interrupt.h>
// #include <util/delay.h>

#define MSGLEN 16

int32_t Scale::get_value(void) const
{
    return m_averager.average();
}

Scale::Scale(void) :
    m_hx711(HX711::G_B32),
    m_report_weight(false),
    m_scale_offset(0),
    m_scale_gain(1.0)
{
}

void Scale::loop(void)
{
    size_t cmd_offset = 0;
    char cmd_buffer[MSGLEN];

    m_hx711.wait_for_ready();
    int32_t value = m_hx711.read();
    m_averager.average(value);
}

