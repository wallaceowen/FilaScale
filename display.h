
#ifndef __display_h
#define __display_h

class Display;

#define MAX_CALLBACKS 10

#ifdef USE_VFUNCS

class DisplayCallback
{
public:
    DisplayCallback();
    virtual ~DisplayCallback() = 0;
    virtual int operator()(Display*) = 0;
protected:
};

class Display
{
public:
    Display();
    void loop();
    bool add_callback(DisplayCallback*);
    unsigned num_cb(void) { return m_callback_count; }

private:
    DisplayCallback *m_callbacks[MAX_CALLBACKS];
    unsigned m_callback_count;
};

#else

typedef void (*DisplayCallback)(Display *d, void *user);

class Display
{
public:
    Display();
    void loop();
    bool add_callback(DisplayCallback);
    unsigned num_cb(void) { return m_callback_count; }

private:
    DisplayCallback m_callbacks[MAX_CALLBACKS];
    unsigned m_callback_count;
};

#endif

#endif
