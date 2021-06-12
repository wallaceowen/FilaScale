// state_view.h

#ifndef __state_view_h
#define __state_view_h

#include "display.h"
#include "button.h"
#include "scale.h"
#include "view.h"
#include "bme280_if.h"

#define NUM_BUTTONS 3

// class StateView: public View
class StateView: public View
{
public:
    StateView(Display&, Scale &s, BME280_IF &b);
    // loop only called when this view is active
    void loop();

    // void render();
    void show();

    bool update();

    // void touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y);
    void touch_callback(Display *d, uint16_t x, uint16_t y, bool pressed);

    void active(bool v) { m_active = v; }
    bool active(void) const { return m_active; }

protected:

private:

    void draw_state();

    Display &m_display;
    Scale &m_scale;
    BME280_IF &m_bme;
    Button *buttons[NUM_BUTTONS];
    bool m_active;
    float m_temp;
    float m_humid;
    float m_weight;
    float m_full_weight;
};

#endif
