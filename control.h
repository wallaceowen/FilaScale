
#ifndef __control_h
#define __control_h

#include <stdint.h>

#include "bme280_if.h"
#include "display.h"
#include "protocol.h"
#include "scale.h"
#include "state_view.h"
#include "calib_view.h"

class Control
{
public:
    enum Mode { M_Show, M_Update, M_Error };
    Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol);
    void loop();
private:

    static void touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y, bool pressed);
    void touch_callback(uint16_t x, uint16_t y, bool touched);

    static void change_view_func(const char *view_name, void *user);
    void change_view(const char *view_name);
// #ifdef BOZO
    static void calib_cb_func(const char *result, void *user);
    void calib_cb(const char *result);
// #endif

    Mode m_mode;
    Scale &m_scale;
    BME280_IF &m_bme280;
    Display &m_display;
    StateView *m_state_view;
    CalibView *m_calib_view;

    View *m_view;
    Protocol &m_protocol;
};

#endif
