
#ifndef __control_h
#define __control_h

#include <stdint.h>

#include "bme280_if.h"
#include "display.h"
#include "protocol.h"
#include "scale.h"
#include "state_view.h"

class Control
{
public:
    enum Mode { M_Show, M_Update, M_Error };
    Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol);
    void loop();
private:
    void show_errors();

    static void disp_callback_func(Display *d, void *user);
    void disp_callback(Display *d);
#ifdef BOZO
    static void touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y, bool pressed);
    void touch_callback(Display *d, uint16_t x, uint16_t y, bool touched);
#endif
    void button_callback(const char *label, bool pressed);

    Mode m_mode;
    Scale &m_scale;
    BME280_IF &m_bme280;
    Display &m_display;
    StateView *m_state_view;
    View *m_view;
    Protocol &m_protocol;
};

#endif
