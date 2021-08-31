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
    int32_t get_raw(void) const { return m_raw; }
    float get_calibrated(void) const;
    void set_offset(void);
    void set_gain();

private:
    FilaConfig  *m_fc;
    HX711        m_hx711;
    int32_t      m_raw;
    ScaleData    m_scale_data;
    Averager     m_averager;
};

#endif

