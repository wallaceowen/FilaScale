
// #include <SPI.h>
#include <Arduino.h>

#include "display.h"

TFT_eSPI display_tft = TFT_eSPI();       // Invoke custom library

uint16_t cal_params[5] = { 310, 3269, 417, 3233, 7 };

Display::Display(void) :
    m_touch_state(false),
    m_callback_count(0)
{
    memset(m_callbacks, 0, sizeof(m_callbacks));
    display_tft.init();
    display_tft.setRotation(ROTATION);
    display_tft.fillScreen(TFT_BLACK);
    display_tft.setTouch(cal_params);
    display_tft.setTextPadding(5);
}

// TFT_eSPI& Display::get_tft(void)
// {
    // return display_tft;
// }

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

#ifdef BOZO
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
#else
bool Display::invoke_callbacks(uint16_t x, uint16_t y, bool pressed)
{
    // if (m_callback_count > iteration)
    for (uint16_t iteration = 0; iteration < m_callback_count; ++iteration)
    {
        CallbackData &cd = m_callbacks[iteration];
        bool result = cd.cb(this, cd.user, x, y, pressed);
        if (result)
            return true;
    }
    return false;
}
#endif

void Display::calibrate(void)
{
    uint16_t touch_parameters[5];

    display_tft.fillRect(0, 0, display_tft.width(), display_tft.height(), TFT_BLACK);

    display_tft.calibrateTouch(touch_parameters, TFT_YELLOW, TFT_MAROON, 20);

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
    auto result = display_tft.getTouch(&x, &y);
    if (result)
    {
        if (!m_touch_state)
        {
            m_touch_state=true;
            return invoke_callbacks(x, y, m_touch_state);
        }
    }
    else
    {
        if (m_touch_state)
        {
            display_tft.last_touched(x, y);
            m_touch_state = false;
            return invoke_callbacks(x, y, m_touch_state);
        }
    }
    return false;
}

void Display::loop(void)
{
    check_touch();
}
