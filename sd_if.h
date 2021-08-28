// SD card interface for the espre-2.8 screen board

#ifndef __sd_if_h
#define __sd_if_h

#include <SPI.h>
#include <FS.h>

// using namespace fs;

class SD_IF
{
    public:
        SD_IF();
        int card_present();
        fs::FS &fs();
    private:
        bool m_available;
};

#endif
