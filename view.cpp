// view.cpp

#include "view.h"

View::View(Display &d, FilaConfig *fc, ViewChangeCallback change_cb, void *ud) :
    m_display(d),
    m_fc(fc),
    m_change_cb(change_cb),
    m_change_data(ud)
{
}

