// state_view.h

#ifndef __state_view_h
#define __state_view_h

#include "display.h"
#include "button.h"
#include "scale.h"
// #include "view.h"
#include "bme280_if.h"

#define NUM_BUTTONS 3

// class StateView: public View
class StateView
{
public:
    StateView(Display&);
    void loop();
    void render();
    bool update_state(Scale &scale, BME280_IF &bme);
private:
    void draw_state();
    static void touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y);
    void touch_callback(Display *d, uint16_t x, uint16_t y);

    Display &m_display;
    Button *buttons[NUM_BUTTONS];
    float m_temp;
    float m_humid;
    float m_weight;
    float m_full_weight;
};

#endif
