// view.cpp

#include "view.h"

View::View(Display &d, ViewChangeCallback change_cb, void *ud) :
    m_display(d),
    m_change_cb(change_cb),
    m_user_data(ud)
{
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

