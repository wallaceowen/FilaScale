#ifndef __adjuster_h
#define __adjuster_h

#include <stdint.h>

#include "display.h"
#include "button.h"
#include "label.h"
#include "rect.h"
#include "dialog.h"
#include "val_mapper.h"

#define ADJ_FONT 4
#define ADJ_RADIUS 10

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
    bool loop(void);
    void show(void);
    // bool check_touch(uint16_t x, uint16_t y, bool pressed);

private:

    Display &m_display;
    const Rect      rect;
    Button          less_button;
    Button          more_button;
    Label           current_l;
    const char     *m_title;
    const char     *m_prompt;
    const char     *varname;
    const ValMapper mapper;
    const uint16_t  inmin;
    const uint16_t  inmax;
};

#endif
