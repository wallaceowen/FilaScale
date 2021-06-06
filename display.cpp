
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#include "display.h"

static TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

Display::Display(void)
    : m_callback_count(0)
{
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
}

DisplayCallback::DisplayCallback()
{
}

bool Display::add_callback(DisplayCallback *cb)
{
    if (m_callback_count < MAX_CALLBACKS)
    {
        m_callbacks[m_callback_count] = cb;
        ++m_callback_count;
        return true;
    }
    return false;
}

void Display::loop()
{
    static int iteration = 1;

    if (m_callback_count >= iteration)
    {
        m_callbacks[iteration]->operator()(this);
    }
}

