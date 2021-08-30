// fila_config.cpp

#include "fila_config.h"

#include "FS.h"
// #include "SD.h"
// #include "SPI.h"

#define PATH "/confdata.dat"

// using namespace fs;

#define DEBUG_CONFIG

FilaConfig::FilaConfig(SD_IF *sd_if) :
    m_sd_if(sd_if)
{
#ifdef DEBUG_CONFIG
    Serial.println("FilaConfig ctor");
    Serial.print("Constructing config (size is ");
    Serial.print(sizeof(ConfigData));
    Serial.print(")\nPrinterData (size is ");
    Serial.print(sizeof(PrinterData));
    Serial.print(")\r\nFilamentData (size is ");
    Serial.print(sizeof(FilamentData));
    Serial.print(")\r\nScreenData (size is ");
    Serial.print(sizeof(ScreenData));
    Serial.print(")\r\nScaleData (size is ");
    Serial.print(sizeof(ScaleData));
    Serial.print(")\r\n");
#endif

    memset(&m_config_data, 0, sizeof(ConfigData));
    if (m_sd_if->card_present())
    {
#ifdef DEBUG_CONFIG
        this->list_dir(PATH, 0);
#endif
        this->load();
    }
#ifdef DEBUG_CONFIG
    else
        Serial.println("Filaconfig told no SD card present.");
#endif
}

void FilaConfig::list_dir(const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = m_sd_if->fs().open("/");
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                this->list_dir(file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}


// return 1 in success, 0 if fail (weird, I know.)
int FilaConfig::save()
{
#ifdef DEBUG_CONFIG
    Serial.print("Saving config to ");
    Serial.println(PATH);
#endif
    File file = m_sd_if->fs().open(PATH, FILE_WRITE);
    int result = 0;
    if(!file)
    {
#ifdef DEBUG_CONFIG
        Serial.println("Failed to open file for writing");
#endif
    }
    else
    {
        if (file.write(reinterpret_cast<uint8_t*>(&m_config_data), sizeof(ConfigData)))
        {
#ifdef DEBUG_CONFIG
            Serial.println("FilaConfig data written");
#endif
            result = 1;
        } 
#ifdef DEBUG_CONFIG
        else 
            Serial.println("FilaConfig write failed");
#endif
    }
    file.close();

    return result;
}

int FilaConfig::load()
{
    int result = 0;
    File file = m_sd_if->fs().open(PATH);
#ifdef DEBUG_CONFIG
    Serial.println("Loading config");
#endif

    if(!file)
    {
#ifdef DEBUG_CONFIG
        Serial.println("open config failed, so saving it.");
#endif
        if (FilaConfig::save())
        {
#ifdef DEBUG_CONFIG
            Serial.println("Written.  Trying to read it again.");
#endif
            file = m_sd_if->fs().open(PATH);
            if(file)
            {
                if (file.read(reinterpret_cast<uint8_t*>(&m_config_data), sizeof(ConfigData)))
                {
#ifdef DEBUG_CONFIG
                    Serial.print("Retry read config. bits: ");
                    Serial.print(m_config_data.present_bits);
#endif
                    result = m_config_data.present_bits;
                }
            }
            else
                Serial.println("still can't read config");
        }
    }
    else
    {
        if (file.read(reinterpret_cast<uint8_t*>(&m_config_data), sizeof(ConfigData)))
        {
#ifdef DEBUG_CONFIG
            Serial.print("Read config. bits: ");
            Serial.println(m_config_data.present_bits);
#endif
            result = m_config_data.present_bits;
        }
    }

    if (file)
        file.close();

    return result;
};

void FilaConfig::set_printer_data(const PrinterData &printer)
{
    m_config_data.present_bits |= (1<<PB_Printer);
    memcpy(&m_config_data.printer_data, &printer, sizeof(PrinterData));
    this->save();
}

void FilaConfig::set_filament_data(const FilamentData &filament_data)
{
    m_config_data.present_bits |= (1<<PB_Filament);
    memcpy(&m_config_data.filament_data, &filament_data, sizeof(FilamentData));
    this->save();
}

void FilaConfig::set_screen_data(const ScreenData &screen_data)
{

#ifdef DEBUG_CONFIG
    Serial.print("size of screen data::");
    Serial.println(sizeof(ScreenData));
    Serial.print("screen data being saved:");
    for (unsigned j = 0; j < 5; ++j)
        Serial.println(screen_data.cal_params[j]);
#endif

    m_config_data.present_bits |= (1<<PB_Screen);
    memcpy(&m_config_data.screen_data, &screen_data, sizeof(ScreenData));

#ifdef DEBUG_CONFIG
    Serial.print("we now have:");
    for (unsigned j = 0; j < 5; ++j)
        Serial.println(m_config_data.screen_data.cal_params[j]);
#endif
    this->save();
}

void FilaConfig::set_scale_data(const ScaleData &scale_data)
{
    m_config_data.present_bits |= (1<<PB_Scale);
    memcpy(&m_config_data.scale_data, &scale_data, sizeof(ScaleData));
#ifdef DEBUG_CONFIG
    Serial.print("scale data being saved: ");
    Serial.print(scale_data.offset);
    Serial.print(", ");
    Serial.print(scale_data.gain);
#endif
    this->save();
}

bool FilaConfig::is_present(PresentBits b) const
{
#ifdef DEBUG_CONFIG
    Serial.print("is_present(");
    Serial.print((int) b);
    Serial.print("): value is ");
    Serial.println(m_config_data.present_bits);
#endif
    return m_config_data.present_bits & (1<<b);
}
