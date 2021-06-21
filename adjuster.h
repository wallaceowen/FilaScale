#ifndef __adjuster_h
#define __adjuster_h

#include <stdint.h>

#include "display.h"
#include "button.h"
#include "label.h"
#include "rect.h"
#include "val_mapper.h"
#include "slider.h"

#define ADJ_FONT 4
#define ADJ_RADIUS 10

/*
 * A very simple class for a bar graph that maps it's display width to the input
 * range, and allows the user to move their finger over the bar, assigning new
 * values of the pixel pressed position on the bar to member var position.
 * Calls to get the value use a reverse map to convert the screen position touched
 * into a proportional value in the output range.
 */
class Adjuster
{
public:

    Adjuster(
            Display &d,
            const Rect &rect,
            const char *t,
            const char *p,
            const char *v,
            uint16_t min,
            uint16_t max);

    void loop(void);
    void show(void);
    bool check_touch(uint16_t x, uint16_t y, bool pressed);
    // uint16_t get_value(void) { return value; }
    uint16_t get_value(void) const;

private:

    void update_bar(uint16_t);

    Display        &m_display;
    TFT_eSPI       &m_tft;
    const Rect      m_rect;
    Label           m_current_l;
    Button          m_less_button;
    Slider          m_slider;
    Button          m_more_button;
    const char     *m_title;
    const char     *m_prompt;
    const char     *m_varname;
    const ValMapper m_inmap;
    const ValMapper m_outmap;
    const uint16_t  m_inmin;
    const uint16_t  m_inmax;

    uint16_t        value;
};

#endif
