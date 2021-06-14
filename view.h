// view.h

#ifndef __view_h
#define __view_h

#include "display.h"

typedef void (*ViewChangeCallback)(const char *view_name, void *user);

class View
{
public:
    View(Display&, ViewChangeCallback, void*);
    // virtual ~View(void) = 0;

    // Only called if this is the active view
    virtual void loop(void) = 0;

    // Show the view
    virtual void show(void) = 0;

    // Update the view, whatever that means for the view
    virtual bool update(void) = 0;

    // Handle the user touching your view
    virtual void touch_callback(uint16_t x, uint16_t y, bool pressed) = 0;

protected:

    Display &m_display;
    ViewChangeCallback m_change_cb;
    void *m_user_data;
};

#endif
