#include "Averager.h"

int32_t Averager::average(int32_t new_sample)
{
    m_samples[m_index++] = new_sample;
    m_index %= NumSamples;
    int32_t result = 0L;
    for (uint16_t i = 0; i < NumSamples; ++i)
        result += m_samples[i];
    return result/NumSamples;
}

int32_t Averager::average() const
{
    int32_t result = 0L;
    for (uint16_t i = 0; i < NumSamples; ++i)
        result += m_samples[i];
    return result/NumSamples;
}
