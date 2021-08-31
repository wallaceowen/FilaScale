/* main.cpp */

#include <Arduino.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include "scale.h"
#include "HX711.h"
#include "fila_config.h"

// #define SCALE_OFFSET 37545L
#define SCALE_OFFSET 36597L
// #define SCALE_GAIN (2.5/1710.0)
#define SCALE_GAIN (2.5/1710.0)

float Scale::get_calibrated(void) const
{
    return (m_raw-m_scale_data.offset)*m_scale_data.gain;
}

void Scale::set_offset(void)
{
    // Serial.print("Setting scale offset to ");
    // String ostring(m_raw);
    // Serial.println(ostring.c_str());
    m_scale_data.offset = m_raw;
    // Serial.print("Scale offset set to ");
    // String gstring(m_scale_data.offset);
    // Serial.println(gstring.c_str());

    {
        char buff[50];
        sprintf(buff, "Scale offset set to %u", m_scale_data.offset);
        Serial.println(buff);
    }
}

void Scale::set_gain()
{
    uint32_t raw = this->get_raw();
    double gain = CAL_WEIGHT_GRAMS/
            double((raw-this->m_scale_data.offset));

    m_scale_data.gain = gain;
    // {
        // char buff[90];
    // uint32_t uv = static_cast<uint32_t>(m_scale_data.gain);
        // sprintf(buff, "WT (%u) / raw(%u)-offset(%d)",
                // uv,
                // this->get_raw(),
                // this->m_scale_data.offset);
        // Serial.println(buff);
    // }

    {
        char buff[50];
        sprintf(buff, "Scale gain set to %6.6f", m_scale_data.gain);
        Serial.println(buff);
    }

    m_fc->set_scale_data(m_scale_data);
}

Scale::Scale(FilaConfig *fc) :
    m_fc(fc),
    m_hx711(HX711::G_B32),
    m_raw(0)
{
    m_scale_data.offset = SCALE_OFFSET;
    m_scale_data.gain = SCALE_GAIN;
    if (!m_fc->is_present(FilaConfig::PB_Scale))
        m_fc->set_scale_data(m_scale_data);
    else
    {
        Serial.println("restoring scale parameters from SD card");
        memcpy(&m_scale_data, &m_fc->get_scale_data(), sizeof(ScaleData));
    }

    Serial.print("Scale offset: ");
    Serial.println(m_scale_data.offset);
    Serial.print("Scale gain: ");
    {
        char buff[30];
        sprintf(buff, "%6.6f", m_scale_data.gain);
        Serial.println(buff);
    }
}

void Scale::loop(void)
{
    m_hx711.wait_for_ready();
    int32_t value = m_hx711.read();
    m_raw = m_averager.average(value);
}

