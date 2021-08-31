// view.h

#ifndef __view_h
#define __view_h

#include "display.h"

// This callback lives in Control class, lets control know when the view
// has changed, so it can ensure the new view renders itself on the display
// once.
typedef void (*ViewChangeCallback)(const char *view_name, void *userdata);

class View
{
public:
    View(Display&, FilaConfig*, ViewChangeCallback, void*);

    // Only called if this is the active view
    virtual void loop(void) = 0;

    // Show the view: render the static part of the view
    // (or call after dirtying the view with a pop-up)
    virtual void show(void) = 0;

    // Handle the user touching your view
    virtual bool touch_callback(uint16_t x, uint16_t y, bool pressed) = 0;

protected:

    // Update the view, whatever that means for the view.  Only called by loop().
    virtual bool update(void) = 0;

    Display           &m_display;
    FilaConfig        *m_fc;
    ViewChangeCallback m_change_cb;
    void              *m_change_data;
};

#endif
