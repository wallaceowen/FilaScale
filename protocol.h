/* protocol.h */

#ifndef __protocol_h
#define __protocol_h

#include <Arduino.h>
#include <string.h>

#include "scale.h"

#define MSGLEN 16

class Protocol
{
public:
    Protocol(Scale &scale);
    void loop();
    Scale &scale() { return m_scale; }

    typedef void (*CommandFunc)(Protocol *p, char *, size_t &);
    struct Command
    {
        const char *cmd;
        CommandFunc func;
        const char *desc;
    };

private:
    Scale &m_scale;
};

#endif
