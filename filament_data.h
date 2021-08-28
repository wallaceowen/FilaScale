// filament_data.h

#ifndef __filament_data
#define __filament_data

#include <inttypes.h>

enum FilamentTypes {
    FT_PLA,
    FT_PETG,
    FT_abs,
    FT_TPU,
    FT_CF_PETG,
    FT_Nylon6,
    FT_CF_Nylon,
    FT_GF_Nylon,
    FT_PC,
    FT_CF_PC
};

struct FilamentData
{
    uint8_t filement_type;
    float max_temp;
    float max_humid;
};

#endif

