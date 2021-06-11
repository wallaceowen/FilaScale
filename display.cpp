
// #include <SPI.h>
#include <Arduino.h>

#include "display.h"

static TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// uint16_t cal_params[5] = { 469, 2616, 598, 1790, 7 };
uint16_t cal_params[5] = { 414, 3054, 602, 2863, 7 };

Display::Display(void)
    : m_callback_count(0)
{
    memset(m_callbacks, 0, sizeof(m_callbacks));
    tft.init();
    tft.setRotation(ROTATION);
    tft.fillScreen(TFT_BLACK);
    tft.setTouch(cal_params);
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

void Display::calibrate(void)
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

void Display::check_touch(void)
{
    uint16_t x, y;
    auto result = tft.getTouch(&x, &y);
    if (result)
    {
        invoke_callbacks(x, y);
    }
}

void Display::loop(void)
{
    // Serial.println("Display::loop()");
    check_touch();
    // calibrate();
}
