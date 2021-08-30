// scale_data.h

#ifndef __scale_data
#define __scale_data

#include <inttypes.h>

struct ScaleData
{
    // ScaleData(uint32_t off, float g) : offset(off), gain(g) {}
    uint32_t offset;
    float gain;
};

#endif

