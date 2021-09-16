#include "sd_if.h"

#include <FS.h>
#include <SD.h>
// #include <SPI.h>
#include "ports.h"

SD_IF::SD_IF() :
    m_available(false)
{
    if (!SD.begin(SD_CS))
    {
        // Serial.println("Card Mount Failed");
    }
    else
        m_available = true;
}

int SD_IF::card_present()
{
    if (!m_available)
        m_available = SD.begin(SD_CS) ? true : false;

    if (m_available)
    {
        sdcard_type_t card_type = SD.cardType();
        return card_type;
    }
    else
        return CARD_NONE;
}

fs::FS &SD_IF::fs() { return SD; }
