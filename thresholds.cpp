// thresholds.cpp
//
// Thresholds for monitored values
//

#include <string.h>

#include "thresholds.h"

Threshold Thresholds::ms_thresholds[] = {
    {"Humidity", 0.0, 60.0 },
    {"DryingTemp", 0.0, 60.0 },
};
#define NUM_THRESHOLDS (sizeof(ms_thresholds)/sizeof(ms_thresholds[0]))

Thresholds::Thresholds(Threshold_CB cb, void *user_data) :
    m_cb(cb),
    m_user_data(user_data)
{
}

void Thresholds::check_threshold(const char *threshold_name, float value)
{
    Threshold *thresh_entry;
    if (!strcmp(threshold_name, "Humidity"))
        thresh_entry = &ms_thresholds[TT_Humid];
    else
        thresh_entry = &ms_thresholds[TT_DryTemp];

    if ((thresh_entry->low >= value)
        || (thresh_entry->high <= value))
    {
        m_cb(thresh_entry, value, m_user_data);
    }
}

void Thresholds::set_threshold(const char *threshold_name, float low, float high)
{
    Threshold *thresh_entry;
    if (!strcmp(threshold_name, "Humidity"))
        thresh_entry = &ms_thresholds[TT_Humid];
    else
        thresh_entry = &ms_thresholds[TT_DryTemp];

    thresh_entry->low = low;
    thresh_entry->high = high;
}
