/* protocol.cpp */

#include <Arduino.h>
#include <string.h>

#include "tag_protocol.h"

// If there's nothing waiting, return 0
// if there's something to read
//    If there's no room in buffer, return -1
//    If there's room, add to buffer.
//       If the char added is a \n, reset offset and
//       return 1, else return 0 (we're still adding
//       bytes to the buffer)
bool TagProtocol::read_tag()
{
    int tag_received = false;

    if (Serial.available())
    {
        int val = Serial.read();

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
                if (val == STX)
                {
                    m_state = TS_GotSTX;
                }

                break;
            }
            case TS_GotSTX:
            {
                if (val == ETX)
                {
                    m_buffer[m_offset++] = '\0';
                    m_cb(m_buffer, m_user_data);
                    tag_received = true;
                    m_state = TS_GotETX;
                }
                else
                {
                    m_buffer[m_offset++] = val;
                    if (m_offset >= MSGLEN)
                    {
                        m_state = TS_Init;
                    }
                }
                break;
            }
            case TS_GotETX:
            {
                m_state = TS_Init;
                break;
            }
            default:
            {
                m_state = TS_Init;
                break;
            }
        }
    }

    return tag_received;
}

void TagProtocol::loop(void)
{
    read_tag();
}

