// printer_data.h

#ifndef __printer_data
#define __printer_data

#include <inttypes.h>

struct PrinterData
{
    char printer_name[32];
    float max_nozzle_temp;
    float max_bed_temp;
    float max_chamber_temp;
    unsigned width; // x size
    unsigned depth; // y size
    unsigned height;// z size
};

#endif
