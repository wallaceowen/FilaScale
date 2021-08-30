/* scale.h */

#ifndef __scale_h
#define __scale_h

#include "HX711.h"
#include "Averager.h"
#include "display_config.h"
#include "fila_config.h"

class Scale
{
public:
    Scale(FilaConfig *fc);

    void loop();
    void report_weight(bool state);
    int32_t get_raw(void) const;
    float get_calibrated(void) const;
    void set_offset(void) { m_scale_data.offset = m_raw; }
    void set_gain()
    {
        m_scale_data.gain = CAL_WEIGHT_GRAMS/(float(
                    (this->get_raw()-this->m_scale_data.offset)
                    ));
    }

private:
    FilaConfig  *m_fc;
    HX711        m_hx711;
    bool         m_report_weight;
    int32_t      m_raw;
    ScaleData    m_scale_data;
    // int32_t      m_offset;
    // double       m_gain;
    Averager     m_averager;
};

#endif

