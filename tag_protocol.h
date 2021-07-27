/* tag_protocol.h */

#ifndef __tag_protocol_h
#define __tag_protocol_h

#include <Arduino.h>
#include <string.h>

#include "scale.h"

// First gen boards have tag tied to serial rx; rev 2 uses serial2 rx
#define TAG_PORT_IS_SERIAL
// #define TAG_PORT_IS_SERIAL2

#define MSGLEN 16

#define STX 2
#define ETX 3

typedef void (*TagReceivedCB)(char tag[MSGLEN], void *user_data);

class TagProtocol
{
public:
    enum TagRxState { TS_Init, TS_WaitSTX, TS_GotSTX, TS_GotETX, TS_NumStates };


    TagProtocol(TagReceivedCB cb, void *user_data) :
        m_cb(cb),
        m_user_data(user_data),
        m_state(TS_Init),
        m_offset(0u)
    {
    }

    void loop();

private:
    TagRxState read_tag(void);

    TagReceivedCB m_cb;
    void         *m_user_data;
    TagRxState    m_state;
    uint16_t      m_offset;
    char          m_buffer[MSGLEN];
};

#endif
