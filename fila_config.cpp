// fila_config.cpp

#include "fila_config.h"

#include "FS.h"
// #include "SD.h"
// #include "SPI.h"

#define PATH "confdata.dat"

// using namespace fs;

FilaConfig::FilaConfig(SD_IF *sd_if) :
    m_sd_if(sd_if)
{
    memset(&m_config_data, 0, sizeof(ConfigData));
}

// return 1 in success, 0 if fail (weird, I know.)
int FilaConfig::save()
{
    File file = m_sd_if->fs().open(PATH, FILE_WRITE);
    int result = 0;
    if(!file){
        Serial.println("Failed to open file for writing");
    }
    else
    {
        if (file.write(reinterpret_cast<uint8_t*>(&m_config_data), sizeof(ConfigData)))
        {
            Serial.println("FilaConfig data written");
            result = 1;
        } 
        else 
        {
            Serial.println("FilaConfig write failed");
        }
    }
    file.close();

    return result;
}

int FilaConfig::load()
{
    File file = m_sd_if->fs().open(PATH);
    int result = 0;
    if(!file){
        Serial.println("Failed to open config file for reading");
    }

    if (file.read(reinterpret_cast<uint8_t*>(&m_config_data), sizeof(ConfigData)))
    {
        Serial.print("Read config. bits: ");
        Serial.print(m_config_data.init_bits);
        result = 1;
    }
    else
        Serial.print("Read config failed");

    file.close();

    return result;
};

void FilaConfig::set_printer_data(const PrinterData &printer)
{
    m_config_data.init_bits |= (1<<IB_Printer);
    memcpy(&m_config_data.printer_data, &printer, sizeof(PrinterData));
}

void FilaConfig::set_filament_data(const FilamentData &filament_data)
{
    m_config_data.init_bits |= (1<<IB_Filament);
    memcpy(&m_config_data.filament_data, &filament_data, sizeof(FilamentData));
}

void FilaConfig::set_screen_data(const ScreenData &screen_data)
{
    m_config_data.init_bits |= (1<<IB_Screen);
    memcpy(&m_config_data.screen_data, &screen_data, sizeof(ScreenData));
}

void FilaConfig::set_scale_data(const ScaleData &scale_data)
{
    m_config_data.init_bits |= (1<<IB_Scale);
    memcpy(&m_config_data.scale_data, &scale_data, sizeof(ScaleData));
}

