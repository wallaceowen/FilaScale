
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#include "display.h"

static TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

Display::Display(void)
    : m_callback_count(0)
{
  memset(m_callbacks, 0, sizeof(m_callbacks));
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
}

bool Display::add_callback(const CallbackData &cd)
{
    if (m_callback_count < MAX_CALLBACKS)
    {
        m_callbacks[m_callback_count] = cd;
        ++m_callback_count;
        return true;
    }
    return false;
}

void Display::loop()
{
    static unsigned iteration = 0;

    if (m_callback_count > iteration)
    {
        CallbackData &cd = m_callbacks[iteration];
        cd.cb(this, cd.user);
    }
    else
        iteration = 0;
}

