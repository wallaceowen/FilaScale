#include "Averager.h"

int32_t Averager::average(int32_t new_sample)
{
    m_samples[m_index++] = new_sample;
    m_index %= NumSamples;
    int64_t result = 0ll;
    for (uint16_t i = 0; i < NumSamples; ++i)
        result += m_samples[i];
    m_average = result/NumSamples;
    return m_average;
}
