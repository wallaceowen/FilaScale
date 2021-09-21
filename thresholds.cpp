// thresholds.cpp
//
// Thresholds for monitored values
//

#include <string.h>

#include "thresholds.h"

Threshold Thresholds::ms_thresholds[MAX_THRESHOLDS] = {
    {"PLA", "DryingTemp", 0.0, 60.0, 50.0 },
    {"ABS", "DryingTemp", 0.0, 90.0, 90.0 },
    {"ASA", "DryingTemp", 0.0, 90.0, 90.0 },
    {"PETG", "DryingTemp", 0.0, 90.0, 40.0 },
    {"Nylon", "DryingTemp", 0.0, 90.0, 40.0 },
    {"PLA", "Humidity", 0.0, 50.0, 90.0 },
    {"ASA", "Humidity", 0.0, 25.0, 90.0 },
    {"ABS", "Humidity", 0.0, 20.0, 40.0 },
    {"PETG", "Humidity", 0.0, 25.0, 40.0 },
    {"Nylon", "Humidity", 0.0, 20.0, 40.0 },
};
#define NUM_THRESHOLDS (sizeof(ms_thresholds)/sizeof(ms_thresholds[0]))

int foo;

Thresholds::Thresholds(Threshold_CB cb, void *user_data) :
    m_cb(cb),
    m_user_data(user_data)
{
}

Threshold *Thresholds::find_threshold(
        const char *filament,
        const char *threshold_name)
{
    for (unsigned i = 0; i < NUM_THRESHOLDS; ++i)
    {
        if ((!strcmp(threshold_name, ms_thresholds[i].name))
                && (!strcmp(filament, ms_thresholds[i].filament)))
        {
            return ms_thresholds+i;
        }
    }

    return 0;
}

void Thresholds::check_threshold(
        const char *filament,
        const char *threshold_name,
        float value)
{
    Threshold *thresh_entry = find_threshold(threshold_name, filament);
    if (thresh_entry)
    {
        if ((thresh_entry->low >= value)
            || (thresh_entry->high <= value))
        {
            m_cb(thresh_entry, value, m_user_data);
        }
    }
}

bool Thresholds::set_threshold(
        const char *filament,
        const char *threshold_name,
        float low, float high, float optimal)
{
    Threshold *thresh_entry = find_threshold(threshold_name, filament);
    if (thresh_entry)
    {
        thresh_entry->low = low;
        thresh_entry->high = high;
        thresh_entry->optimal = optimal;

        return true;
    }

    return false;
}

