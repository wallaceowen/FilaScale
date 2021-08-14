
#ifndef __display_h
#define __display_h

#include <stdint.h>
#include <TFT_eSPI.h> // Hardware-specific library

class Display;

#define ROTATION 3
// #define ROTATION 1

#define MAX_CALLBACKS 10

// #define SCREEN_BG TFT_BROWN

typedef bool (*TouchCallbackFunc)(
        Display *d,
        void *user,
        uint16_t x,
        uint16_t y,
        bool pressed);

struct CallbackData
{
    TouchCallbackFunc cb;
    void *user;
};


extern TFT_eSPI display_tft;

class Display
{
public:
    Display(void);
    void loop(void);
    bool add_callback(const CallbackData&);
    TFT_eSPI& get_tft(void) { return display_tft; }
    void calibrate(void);
    void set_calibration(uint16_t params[5]);

private:
    bool invoke_callbacks(uint16_t x, uint16_t y, bool pressed);
    bool check_touch(void);
    unsigned num_cb(void) const { return m_callback_count; }

    bool         m_touch_state;
    CallbackData m_callbacks[MAX_CALLBACKS];
    unsigned     m_callback_count;
};

#endif
