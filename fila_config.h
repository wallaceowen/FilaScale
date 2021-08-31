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
    uint8_t present_bits;
    PrinterData printer_data;
    FilamentData filament_data;
    ScreenData screen_data;
    ScaleData scale_data;
};

class FilaConfig
{
public:

    enum PresentBits { PB_Printer=0, PB_Filament, PB_Screen, PB_Scale };

    FilaConfig(SD_IF *sd_if);

    int save();
    int load();

    void set_printer_data(const PrinterData &);
    const PrinterData &get_printer_data(void) const { return m_config_data.printer_data; }
    void set_filament_data(const FilamentData &);
    const FilamentData &get_filament_data(void) const { return m_config_data.filament_data; }
    void set_screen_data(const ScreenData &);
    const ScreenData &get_screen_data(void) const { return m_config_data.screen_data; }
    void set_scale_data(ScaleData &);
    const ScaleData &get_scale_data(void) const { return m_config_data.scale_data; }

    // bool is_present(PresentBits b) const { return m_config_data.present_bits & (1<<b); }
    bool is_present(PresentBits b) const;
    void set_present(PresentBits b) { m_config_data.present_bits |= (1<<b); }
    void clr_present(PresentBits b) { m_config_data.present_bits &= ~(1<<b); }

private:
    void list_dir(const char * dirname, uint8_t levels);

    SD_IF *m_sd_if;
    ConfigData m_config_data;
};

#endif

