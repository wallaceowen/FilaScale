
#ifndef __display_h
#define __display_h

#include <stdint.h>
#include <TFT_eSPI.h> // Hardware-specific library

#define LANDSCAPE_ORIENTATION

#ifdef LANDSCAPE_ORIENTATION
#define ROTATION 3
#define WIDTH 320
#define HEIGHT 240
#define VALUES_X 160
#define STATE_FONT 4
#else
#define ROTATION 2
#define WIDTH 240
#define HEIGHT 320
#define VALUES_X 100
#define STATE_FONT 4
#endif


#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2


class Display;

#define MAX_CALLBACKS 10

typedef void (*DisplayCallbackFunc)(Display *d, void *user);
typedef void (*TouchCallbackFunc)(Display *d, void *user, uint16_t x, uint16_t y);

struct CallbackData
{
    // CallbackData(DisplayCallbackFunc c, void *u) : cb(c), user(u) { }

    TouchCallbackFunc cb;
    void *user;
};


class Display
{
public:
    Display(void);
    void loop(void);
    bool add_callback(const CallbackData&);
    unsigned num_cb(void) { return m_callback_count; }
    TFT_eSPI& get_tft(void);
    void calibrate(void);


private:
    void invoke_callbacks(uint16_t x, uint16_t y);
    void check_touch(void);
    CallbackData m_callbacks[MAX_CALLBACKS];
    unsigned m_callback_count;
};

#endif
