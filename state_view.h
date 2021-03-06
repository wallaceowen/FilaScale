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
    // ctor
    StateView(Display &d, FilaConfig *fc, ViewChangeCallback ccb, void *change_user_data, Scale &s, BME280_IF &b);

    // call this often?
    void loop();

    // void render();
    void show();

    bool update();

    bool touch_callback(uint16_t x, uint16_t y, bool pressed);

    void set_tag(uint64_t &tag);

protected:

private:

    void state_menu_callback(const char *label, bool pressed);

    void draw_state(void);

    Display   &m_display;
    Scale     &m_scale;
    BME280_IF &m_bme;
    float      m_temp;
    float      m_humid;
    float      m_weight;
    float      m_full_weight;
    uint16_t   m_title_height;
    uint64_t   m_tag;
};

#endif
