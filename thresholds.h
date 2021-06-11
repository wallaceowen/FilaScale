// thresholds.h
//
// Thresholds for monitored values
//

#ifndef __thresholds_h
#define __thresholds_h

#include <stdint.h>

#define MAX_THRESHOLDS 5

struct Thresholds
{
    char name[16];
    float high_temp;
    float high_humidity;
    float dry_temp;
    float low_filament_grams;
};

extern const Thresholds thresholds[MAX_THRESHOLDS];
extern uint16_t num_thresholds;

#endif
