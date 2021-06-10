
// #include <SPI.h>
#include <Arduino.h>

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

TFT_eSPI& Display::get_tft(void)
{
    return tft;
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

void Display::invoke_callbacks(uint16_t x, uint16_t y)
{
    static unsigned iteration = 0;

    if (m_callback_count > iteration)
    {
        CallbackData &cd = m_callbacks[iteration];
        cd.cb(this, cd.user, x, y);
    }
    else
        iteration = 0;
}

void calibrate()
{
    uint16_t touch_parameters[5];
    tft.calibrateTouch(touch_parameters, TFT_YELLOW, TFT_MAROON, 20);
    Serial.print("touch parameters: ");
    for (size_t i = 0; i < 5; ++i)
    {
        if (i)
            Serial.print(", ");
        Serial.print(touch_parameters[i]);
    }
    Serial.println("");
}

bool Display::check_touch(void)
{
    uint16_t x, y;
    auto result = tft.getTouch(&x, &y);
    if (result)
    {
        invoke_callbacks(x, y);
    }
}

void Display::loop()
{
    Serial.println("Display::loop()");
    // invoke_callbacks();
    // calibrate();
}
