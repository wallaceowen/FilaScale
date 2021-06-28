
// #include <SPI.h>
#include <Arduino.h>

#include "display.h"

static TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// uint16_t cal_params[5] = { 469, 2616, 598, 1790, 7 };
// uint16_t cal_params[5] = { 414, 3054, 602, 2863, 7 };
// uint16_t cal_params[5] = { 473, 2953, 645, 2759, 7 };
uint16_t cal_params[5] = { 310, 3269, 417, 3233, 7 };

Display::Display(void) :
    m_touch_state(false),
    m_callback_count(0)
{
    memset(m_callbacks, 0, sizeof(m_callbacks));
    tft.init();
    tft.setRotation(ROTATION);
    tft.fillScreen(TFT_BLACK);
    tft.setTouch(cal_params);
    // tft.setTextPadding(5);
}

TFT_eSPI& Display::get_tft(void)
{
    return tft;
}

void set_calibration(uint16_t params[5])
{
    for (unsigned i = 0; i < 5; ++i)
        cal_params[i] = params[i];
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

void Display::invoke_callbacks(uint16_t x, uint16_t y, bool pressed)
{
    static unsigned iteration = 0;

    if (m_callback_count > iteration)
    {
        CallbackData &cd = m_callbacks[iteration];
        cd.cb(this, cd.user, x, y, pressed);
    }
    else
        iteration = 0;
}

void Display::calibrate(void)
{
    uint16_t touch_parameters[5];

    tft.fillRect(0, 0, tft.width(), tft.height(), TFT_BLACK);

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
        if (!m_touch_state)
        {
            m_touch_state=true;
            invoke_callbacks(x, y, m_touch_state);
        }
    }
    else
    {
        if (m_touch_state)
        {
            tft.last_touched(x, y);
            m_touch_state = false;
            invoke_callbacks(x, y, m_touch_state);
        }
    }
}

void Display::loop(void)
{
    // Serial.println("Display::loop()");
    check_touch();
    // calibrate();
}
