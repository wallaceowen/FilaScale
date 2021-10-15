/* main.cpp */

#include <Arduino.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include "scale.h"
#include "HX711.h"

/* "FACTORY RESET" values */
#define SCALE_OFFSET 36597L
#define SCALE_GAIN (2.5/1710.0)

float Scale::get_calibrated(void) const
{
    return (m_raw-m_scale_data.offset)*m_scale_data.gain;
}

void Scale::set_offset(int32_t offset)
{
    m_scale_data.offset = offset;
    // Serial.print("set_offset(");
    // Serial.print(offset);
    // Serial.println(")");
}

void Scale::set_gain(float gain)
{
    m_scale_data.gain = gain;
}

Scale::Scale() :
    m_hx711(HX711::G_A128),
    m_raw(0)
{
    m_scale_data.offset = SCALE_OFFSET;
    m_scale_data.gain = SCALE_GAIN;
}

void Scale::loop(void)
{
    m_hx711.wait_for_ready();
    int32_t value = m_hx711.read();
    m_raw = m_averager.average(value);
}

