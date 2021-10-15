/* scale.h */

#ifndef __scale_h
#define __scale_h

#include "HX711.h"
#include "Averager.h"

struct ScaleData
{
    int32_t offset;
    float gain;
};

class Scale
{
public:
    Scale();

    void loop();
    int32_t get_raw(void) const { return m_raw; }
    float get_calibrated(void) const;
    int32_t get_offset() const { return m_scale_data.offset; }
    void set_offset(int32_t);
    float get_gain() const { return m_scale_data.gain; }
    void set_gain(float);

private:
    HX711        m_hx711;
    int32_t      m_raw;
    ScaleData    m_scale_data;
    Averager     m_averager;
};

#endif

