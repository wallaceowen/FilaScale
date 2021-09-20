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
    char filament[16];
    float low;
    float high;
    float optimal;
};

typedef void (*Threshold_CB)(const Threshold*, float value, void *user);

class Thresholds
{
public:
    Thresholds(Threshold_CB cb, void *udata);

    void check_threshold(
            const char *threshold_name,
            const char *filament_name,
            float value);

    bool set_threshold(
            const char *threshold_name,
            const char *filament_name,
            float low, float high, float optimal);

    void loop();

private:
    Threshold *find_threshold(const char *thresh_name, const char *filament);

    static Threshold ms_thresholds[];
    Threshold_CB     m_cb;
    void            *m_user_data;
};

#endif
