
// #include <SPI.h>
#include <Arduino.h>

#include "display.h"

// #define DEBUG_TOUCH

TFT_eSPI display_tft = TFT_eSPI();       // Invoke custom library

// Rotation 1:
// uint16_t cal_params[5] = { 310, 3269, 417, 3233, 7 };
// Rotation 3:
// uint16_t cal_params[5] = { 537, 3307, 689, 2630, 1 };
ScreenData sdata = { { 519, 3333, 702, 2461, 1 }, ROTATION };

Display::Display(FilaConfig *fc) :
    m_touch_state(false),
    m_fc(fc),
    m_callback_count(0)
{
    memset(m_callbacks, 0, sizeof(m_callbacks));
    display_tft.init();
    display_tft.setRotation(sdata.rotation);
    display_tft.fillScreen(TFT_BLACK);
    if (m_fc->is_present(FilaConfig::PB_Screen))
        memcpy(&sdata, &m_fc->get_screen_data(), sizeof(ScreenData));
    else
        m_fc->set_screen_data(sdata);

    for (size_t i = 0; i < 5; ++i)
    {
        if (i)
            Serial.print(", ");
        Serial.print(sdata.cal_params[i]);
    }

    display_tft.setTouch(m_fc->get_screen_data().cal_params);
    display_tft.setTextPadding(5);
}

void Display::set_calibration(uint16_t params[5])
{
    for (unsigned i = 0; i < 5; ++i)
        sdata.cal_params[i] = params[i];
    m_fc->set_screen_data(sdata);
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

void Display::calibrate(void)
{
    ScreenData sdata;
    sdata.rotation = ROTATION;

    display_tft.fillRect(0, 0, display_tft.width(), display_tft.height(), TFT_BLACK);

    display_tft.calibrateTouch(sdata.cal_params, TFT_YELLOW, TFT_MAROON, 20);

    Serial.print("touch parameters: ");
    for (size_t i = 0; i < 5; ++i)
    {
        if (i)
            Serial.print(", ");
        Serial.print(sdata.cal_params[i]);
    }
    Serial.println("");
    m_fc->set_screen_data(sdata);
}

bool Display::check_touch(void)
{
    uint16_t x, y;
    auto result = display_tft.getTouch(&x, &y);
    if (result)
    {
#ifdef DEBUG_TOUCH
        Serial.print("touched at ");
        Serial.print(x);
        Serial.print(",");
        Serial.println(y);
#endif
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
