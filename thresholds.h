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
    float low;
    float high;
    float dry_temp;
};

extern Thresholds thresholds[MAX_THRESHOLDS];
extern uint16_t num_thresholds;

#endif
