// fila_config.h

#ifndef __fila_config_h
#define __fila_config_h

#include "filament_data.h"
#include "printer_data.h"
#include "scale_data.h"
#include "screen_data.h"

#include "sd_if.h"

struct ConfigData
{
    uint8_t init_bits;
    PrinterData printer_data;
    FilamentData filament_data;
    ScreenData screen_data;
    ScaleData scale_data;
};

class FilaConfig
{
public:

    enum InitBits { IB_Printer=0, IB_Filament, IB_Screen, IB_Scale };

    FilaConfig(SD_IF *sd_if);

    int save();
    int load();

    void set_printer_data(const PrinterData &);
    void set_filament_data(const FilamentData &);
    void set_screen_data(const ScreenData &);
    void set_scale_data(const ScaleData &);

private:

    SD_IF *m_sd_if;
    ConfigData m_config_data;
};

#endif

