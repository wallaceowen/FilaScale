// screen_data.h

#ifndef __screen_data
#define __screen_data

#include <inttypes.h>

struct ScreenData
{
    // Calibration values for touch
    uint16_t cal_params[5];

    // Screen rotation
    uint8_t rotation;
};

#endif

