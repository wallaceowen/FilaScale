/* scale.h */

#ifndef __scale_h
#define __scale_h

#include "HX711.h"
#include "Averager.h"

#define CAL_WEIGHT_GRAMS 200.0

class Scale
{
public:
    Scale();

    void loop();
    void report_weight(bool state);
    int32_t get_value(void) const;
    void set_offset(int32_t off) { m_scale_offset = off; }
    void set_gain()
    {
        m_scale_gain = CAL_WEIGHT_GRAMS/(float((this->get_value()-this->m_scale_offset)));
    }

private:
    HX711 m_hx711;
    bool m_report_weight;
    int32_t m_scale_offset;
    double m_scale_gain;
    Averager m_averager;
};

#endif

