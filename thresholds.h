// thresholds.h
//
// Thresholds for monitored values
//

#ifndef __thresholds_h
#define __thresholds_h

#include <stdint.h>

#define MAX_THRESHOLDS 25

struct Threshold
{
    char name[16];
    float low;
    float high;
    float optimal;
};

class Thresholds
{
public:
    Thresholds();
    bool add_threshold(const Threshold& rhs);

private:
    static Threshold ms_thresholds[MAX_THRESHOLDS];
    uint16_t m_num_thresholds;
};

#endif
