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
};

typedef void (*Threshold_CB)(const Threshold*, float value, void *user);

class Thresholds
{
public:
    enum ThreshType { TT_Humid, TT_DryTemp };

    Thresholds(Threshold_CB cb, void *udata);

    void check_threshold( const char *threshold_name, float value);

    void set_threshold(const char *threshold_name, float low, float high);

    void loop();

private:
    static Threshold ms_thresholds[];
    Threshold_CB     m_cb;
    void            *m_user_data;
};

#endif
