// view.h

#ifndef __view_h
#define __view_h

#include "display.h"

class View
{
public:
    View(Display&);

    // Only called if this is the active view
    virtual void loop(void);

    // Show the view
    virtual void show(void);

    // Update the view, whatever that means for the view
    virtual bool update(void);

    virtual void touch_callback(uint16_t x, uint16_t y, bool pressed) = 0;

protected:

    Display &m_display;
};

#endif
