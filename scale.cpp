/* main.cpp */

#include <Arduino.h>
#include <inttypes.h>
#include <string.h>

#include "scale.h"
#include "HX711.h"
#include "fila_config.h"

#define SCALE_OFFSET 37545L
#define SCALE_GAIN (2.5/1710.0)

int32_t Scale::get_raw(void) const
{
    return m_raw;
}

float Scale::get_calibrated(void) const
{
    return (m_raw-m_scale_data.offset)*m_scale_data.gain;
}

Scale::Scale(FilaConfig *fc) :
    m_fc(fc),
    m_hx711(HX711::G_B32),
    m_report_weight(false),
    m_raw(0)
{
    m_scale_data.offset = SCALE_OFFSET;
    m_scale_data.gain = SCALE_GAIN;
    if (!m_fc->is_present(FilaConfig::PB_Scale))
        m_fc->set_scale_data(m_scale_data);
    else
    {
        Serial.print("restoring scale parameters from SD card");
        memcpy(&m_scale_data, &m_fc->get_scale_data(), sizeof(ScaleData));
    }

    Serial.print("Scale offset: ");
    Serial.println(m_scale_data.offset);
    Serial.print("Scale gain: ");
    Serial.println(m_scale_data.gain);
}

void Scale::loop(void)
{
    m_hx711.wait_for_ready();
    int32_t value = m_hx711.read();
    m_raw = m_averager.average(value);
}

