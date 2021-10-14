/* protocol.cpp */

#include <Arduino.h>
#include <string.h>

#include "tag_protocol.h"

#define INTER_MSG_DELAY 1000

// If there's nothing waiting, return 0
// if there's something to read
//    If there's no room in buffer, return -1
//    If there's room, add to buffer.
//       If the char added is a \n, reset offset and
//       return 1, else return 0 (we're still adding
//       bytes to the buffer)
TagProtocol::TagRxState TagProtocol::read_tag()
{
    static auto until = millis();
    // if (m_softSerial.available())
    {
        // int val = m_softSerial.read();

        switch(m_state)
        {
            case TS_Init:
            {
                memset(m_buffer, 0, sizeof(m_buffer));
                m_offset = 0u;
                m_state = TS_WaitSTX;
                // break;
            }
            case TS_WaitSTX:
            {
                if (m_softSerial.available())
                {
                    int val = m_softSerial.read();

                    if (val == STX)
                    {
                        m_state = TS_GotSTX;
                    }
                }

                break;
            }
            case TS_GotSTX:
            {
                if (m_softSerial.available())
                {
                    int val = m_softSerial.read();
                    if (val == ETX)
                    {
                        m_buffer[m_offset++] = '\0';
                        if (m_cb)
                            m_cb(m_buffer, m_user_data);
                        until = millis()+INTER_MSG_DELAY;
                        m_state = TS_GotETX;
                    }
                    else
                    {
                        m_buffer[m_offset++] = val;
                        if (m_offset >= TAG_MSGLEN)
                        {
                            m_state = TS_Init;
                        }
                    }
                }
                break;
            }
            case TS_GotETX:
            {
                if (until < millis())
                {
                    m_state = TS_Init;
                }
                else
                {
                    while (m_softSerial.available())
                        m_softSerial.read();
                }
                break;
            }
            default:
            {
                m_state = TS_Init;
                break;
            }
        }
    }

    return m_state;
}

void TagProtocol::loop(void)
{
    // TagProtocol::TagRxState state = read_tag();
    // Serial.println((int)state);
    read_tag();
}

