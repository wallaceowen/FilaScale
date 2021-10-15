#ifndef __Averager_h
#define __Averager_h

#include <inttypes.h>
#include <string.h>

class Averager
{
public:
    enum Constants { NumSamples=32 };
    Averager() : m_average(0), m_index(0) { memset(m_samples, 0, sizeof(m_samples)); }
    int32_t average(int32_t new_sample);
    int32_t average() const { return m_average; }

private:
    int32_t  m_average;
    uint16_t m_index;
    int32_t  m_samples[NumSamples];
};

#endif
