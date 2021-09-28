// thresholds.cpp
//
// Thresholds for monitored values
//

#include <string.h>

#include "thresholds.h"

Threshold Thresholds::ms_thresholds[] = {
    {"Humidity", 0.0, 60.0 },
    {"DryingTemp", 0.0, 60.0 },
    {"Weight", 200.0, 30000.0 },
};
#define NUM_THRESHOLDS (sizeof(ms_thresholds)/sizeof(ms_thresholds[0]))

Thresholds::Thresholds(Threshold_CB cb, void *user_data) :
    m_cb(cb),
    m_user_data(user_data)
{
}

Thresholds::ThreshType threshold_name_to_type(const char *threshold_name)
{
    Thresholds::ThreshType ttype = Thresholds::TT_Num;

    if (!strcmp(threshold_name, "Humidity"))
        ttype = Thresholds::TT_Humid;
    else if (!strcmp(threshold_name, "DryingTemp"))
        ttype = Thresholds::TT_DryTemp;
    else if (!strcmp(threshold_name, "Weight"))
        ttype = Thresholds::TT_Weight;

    return ttype;
}

void Thresholds::check_threshold(const char *threshold_name, float value)
{
    ThreshType ttype = threshold_name_to_type(threshold_name);
    if (ttype != Thresholds::TT_Num)
    {
        Threshold *thresh_entry = &ms_thresholds[ttype];

        if ((thresh_entry->low >= value)
            || (thresh_entry->high <= value))
        {
            m_cb(ttype, thresh_entry, value, m_user_data);
        }
    }
}

void Thresholds::set_threshold(const char *threshold_name, float low, float high)
{
    ThreshType ttype = threshold_name_to_type(threshold_name);
    if (ttype != Thresholds::TT_Num)
    {
        Threshold *thresh_entry = &ms_thresholds[ttype];
        thresh_entry->low = low;
        thresh_entry->high = high;
    }
}
