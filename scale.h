/* scale.h */

#ifndef __scale_h
#define __scale_h

#include "HX711.h"
#include "Averager.h"
#include "display_config.h"

class Scale
{
public:
    Scale();

    void loop();
    void report_weight(bool state);
    int32_t get_raw(void) const;
    float get_calibrated(void) const;
    void set_offset(void) { m_offset = m_raw; }
    void set_gain()
    {
        m_gain = CAL_WEIGHT_GRAMS/(float((this->get_raw()-this->m_offset)));
    }

private:
    HX711    m_hx711;
    bool     m_report_weight;
    int32_t  m_raw;
    int32_t  m_offset;
    double   m_gain;
    Averager m_averager;
};

#endif

