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

#define SCALE_OFFSET 37545L
#define SCALE_GAIN (2.5/1710.0)

int32_t Scale::get_raw(void) const
{
    // return m_averager.average();
    return m_raw;
}

float Scale::get_calibrated(void) const
{
    return (m_raw-m_offset)*m_gain;
}

Scale::Scale(void) :
    m_hx711(HX711::G_B32),
    m_report_weight(false),
    m_raw(0),
    m_offset(SCALE_OFFSET),
    // m_gain(1.0)
    m_gain(SCALE_GAIN)
    // m_gain(1.0)
{
}

void Scale::loop(void)
{
    size_t cmd_offset = 0;
    char cmd_buffer[MSGLEN];

    m_hx711.wait_for_ready();
    int32_t value = m_hx711.read();
    m_raw = m_averager.average(value);
}

