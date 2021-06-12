// view.cpp

#include "view.h"

View::View(Display &d) :
    m_display(d)
{
    CallbackData cd;
    cd.cb = this->touch_callback_func;
    cd.user = this;
    m_display.add_callback(cd);
}

void View::loop()
{
}

void View::show()
{
}

bool View::update()
{
}

void View::touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y, bool pressed)
{
    View *view = reinterpret_cast<View*>(user);
    view->touch_callback(d, x, y, pressed);
}

