
#ifndef __control_h
#define __control_h

#include <stdint.h>

#include "bme280_if.h"
#include "display.h"
#include "tag_protocol.h"
#include "scale.h"
#include "state_view.h"
#include "calib_view.h"
#include "config_view.h"
#include "filament_view.h"
#include "network_view.h"

class Control
{
public:

    enum Mode { M_Show, M_Update, M_Error };
    Control(Scale &scale, Display &display, BME280_IF &bme280, TagProtocol &tag_protocol);
    void loop();

private:

    static bool touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y, bool pressed);
    bool touch_callback(uint16_t x, uint16_t y, bool touched);

    static void change_view_func(const char *view_name, void *user);
    void change_view(const char *view_name);

    static void tag_handler_func(char tag[TAG_MSGLEN], void *user);
    void tag_handler(char tag[TAG_MSGLEN]);

    Mode m_mode;
    Scale &m_scale;
    BME280_IF &m_bme280;
    Display &m_display;

    StateView m_state_view;
    CalibView m_scale_calib_view;
    ConfigView m_config_view;
    FilamentView m_filament_view;
    NetworkView m_network_view;

    View *m_view;
    TagProtocol &m_tag_protocol;
};

#endif

