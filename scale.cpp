/* main.cpp */

#include <Arduino.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include "scale.h"
#include "HX711.h"
#include "fila_config.h"

#define SCALE_OFFSET 36597L
#define SCALE_GAIN (2.5/1710.0)

float Scale::get_calibrated(void) const
{
    return (m_raw-m_scale_data.offset)*m_scale_data.gain;
}

void Scale::set_offset(void)
{
    m_scale_data.offset = m_raw;
}

void Scale::set_gain()
{
    uint32_t raw = this->get_raw();
    double gain = CAL_WEIGHT_GRAMS/
            double((raw-this->m_scale_data.offset));

    m_scale_data.gain = gain;
    m_fc->set_scale_data(m_scale_data);
}

Scale::Scale(FilaConfig *fc) :
    m_fc(fc),
    m_hx711(HX711::G_A128),
    m_raw(0)
{
    m_scale_data.offset = SCALE_OFFSET;
    m_scale_data.gain = SCALE_GAIN;
    if (!m_fc->is_present(FilaConfig::PB_Scale))
        m_fc->set_scale_data(m_scale_data);
    else
        memcpy(&m_scale_data, &m_fc->get_scale_data(), sizeof(ScaleData));
}

void Scale::loop(void)
{
    m_hx711.wait_for_ready();
    int32_t value = m_hx711.read();
    m_raw = m_averager.average(value);
}

