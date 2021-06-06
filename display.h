
#ifndef __display_h
#define __display_h

class Display;

#define MAX_CALLBACKS 10

typedef void (*DisplayCallbackFunc)(Display *d, void *user);

struct CallbackData
{
    // CallbackData(DisplayCallbackFunc c, void *u) : cb(c), user(u) { }

    DisplayCallbackFunc cb;
    void *user;
};


class Display
{
public:
    Display();
    void loop();
    bool add_callback(const CallbackData&);
    unsigned num_cb(void) { return m_callback_count; }

private:
    CallbackData m_callbacks[MAX_CALLBACKS];
    unsigned m_callback_count;
};

#endif
