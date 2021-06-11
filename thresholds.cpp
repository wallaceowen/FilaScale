// thresholds.cpp
//
// Thresholds for monitored values
//

#include "thresholds.h"

const Thresholds thresholds[MAX_THRESHOLDS] = {
    {"PLA", 4.0, 33.09, 50.0, 40.0 },
    {"ASA", 4.0, 33.09, 90, 40.0 },
    {"Nylon", 4.0, 33.09, 90, 40.0 },
};

uint16_t num_thresholds = 10;

