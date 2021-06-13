// thresholds.cpp
//
// Thresholds for monitored values
//

#include "thresholds.h"

Threshold Thresholds::ms_thresholds[MAX_THRESHOLDS] = {
    {"PLA-DRYTEMP", 0.0, 90.0, 50.0 },
    {"ASA-DRYTEMP", 0.0, 90.0, 90.0 },
    {"Nylon-DRYTEMP", 0.0, 90.0, 40.0 },
};

#define NUM_THRESHOLDS (sizeof(ms_thresholds)/sizeof(ms_thresholds[0]))

Thresholds::Thresholds()
    : m_num_thresholds(NUM_THRESHOLDS)
{
}

bool Thresholds::add_threshold(const Threshold& rhs)
{
    if (m_num_thresholds < (NUM_THRESHOLDS-1))
    {
        // struct assignment
        ms_thresholds[m_num_thresholds] = rhs;
        ++m_num_thresholds;
        return true;
    }
    return false;
}
