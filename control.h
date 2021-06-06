
#ifndef __control_h
#define __control_h

#include "bme280_if.h"
#include "display.h"
#include "protocol.h"
#include "scale.h"

class Control
{
public:
    enum Mode { M_Show_State, M_Update_State, M_Show_Menu, M_Update_Menu, M_Adjust_BME, M_CalibrateScale, M_Error_State };
    Control(Scale &scale, Display &display, BME280_IF &bme280, Protocol &protocol);
    void loop();
private:
    void show_state();
    void update_state();
    void show_menu();
    void update_menu();
    void adjust_bme();
    void calibrate_scale();
    void show_errors();

    static void disp_callback_func(Display *d, void *user);
    void disp_callback(Display *d);

    Mode m_mode;
    Scale &m_scale;
    Display &m_display;
    BME280_IF &m_bme280;
    Protocol &m_protocol;
};

#endif
