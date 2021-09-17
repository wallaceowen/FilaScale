/* protocol.cpp */

#include <Arduino.h>

#include "octo_protocol.h"
#include "crc16.h"


#define SIZEOF_SYNC 1
#define SIZEOF_MSG_TYPE 1
#define SIZEOF_MSG_LENGTH 2
#define SIZEOF_HEADER (SIZEOF_SYNC+SIZEOF_MSG_TYPE+SIZEOF_MSG_LENGTH)
#define SIZEOF_CRC 2

// OctoProtocol::OctoProtocol(MessageHandlerFunc func, void *user_data) :
    // m_handler(func),
    // m_user_data(user_data)
// {
    // Serial.begin(BaudRate);
    // Serial.setTimeout(500);
// }

OctoProtocol::OctoProtocol() :
    m_handler(0),
    m_user_data(0)
{
    Serial.begin(BaudRate);
    Serial.setTimeout(500);
}

void OctoProtocol::set_handler(MessageHandlerFunc func, void *user_data)
{
    m_handler = func;
    m_user_data = user_data;
}

int OctoProtocol::recv_bytes(char *buffer, uint16_t qty) const
{
    size_t rcount = Serial.readBytes(buffer, qty);
    return rcount;
}

uint16_t compute_msg_crc(char *body, uint16_t len)
{
}

// Attempt to read an entire message 
OctoProtocol::OP_Error OctoProtocol::recv_msg(MsgType &_type, uint16_t &length, char *body) const
{
    // Read bytes until we find the sync byte or we time out
    if (!Serial.find(SYNC_BYTE))
        return OP_NoData;

    // Read the message type or time out
    if (!recv_bytes(body, SIZEOF_MSG_TYPE))
        return OP_NoData;

    // Save the type
    _type = static_cast<MsgType>(body[0]);

    if (_type >= NUM_VALID_MESSAGES)
        return OP_Bad_MsgType;

    // Get the length or time out
    if (!recv_bytes(body+SIZEOF_MSG_TYPE, SIZEOF_MSG_LENGTH))
        return OP_NoData;

    length = *(reinterpret_cast<uint16_t*>(body+SIZEOF_MSG_TYPE));
    if (length > MAX_RX_SIZE)
        return OP_BadSize;

    if (length)
    {
        // Try to read the body
            if (!recv_bytes(body+SIZEOF_MSG_TYPE+SIZEOF_MSG_LENGTH, length))
                return OP_NoData;
        }

        // Read in the transmitted crc
        uint16_t rcrc;
        if (!recv_bytes(reinterpret_cast<char*>(&rcrc), SIZEOF_CRC))
        return OP_NoData;

    // Compute the CRC of the bytes we received in the message
    uint16_t ccrc = compute_msg_crc(body, SIZEOF_MSG_TYPE+SIZEOF_MSG_LENGTH+length);

    if (ccrc != rcrc)
        return OP_Bad_CRC;

    return OP_NoError;
}

OctoProtocol::OP_Error OctoProtocol::send_msg(MsgType _type, uint16_t length, const char *body)
{
    uint16_t xcrc = 0;
    if (Serial.write(SYNC_BYTE) != SIZEOF_SYNC)
        return OP_NoConnection;
    // else
        // calcCRC16(static_cast<unsigned char>(SYNC_BYTE), &xcrc);

    if (Serial.write(static_cast<int8_t>(_type)) != SIZEOF_MSG_TYPE)
        return OP_NoConnection;
    else
        calcCRC16(_type, &xcrc);

    if (Serial.write(reinterpret_cast<uint8_t*>(&length), SIZEOF_MSG_LENGTH) != SIZEOF_MSG_LENGTH)
        return OP_NoConnection;
    else
        calcCRCArray16(reinterpret_cast<uint8_t*>(&length), 2, &xcrc);

    if (Serial.write(body, length) != length)
        return OP_NoConnection;
    else
        calcCRCArray16(reinterpret_cast<uint8_t*>(&body), length, &xcrc);

    if (Serial.write(reinterpret_cast<uint8_t*>(&xcrc), SIZEOF_CRC) != SIZEOF_CRC)
        return OP_NoConnection;
}

// void OctoProtocol::handle_message(MsgType _type, uint16_t length, char *buffer)
// {
    // if (m_handler)
        // m_handler(_type, length, buffer, m_user_data);
// }

static char rx_buffer[OctoProtocol::MAX_RX_SIZE];

void OctoProtocol::loop()
{
    // If there's enough bytes waiting for a minimum message check it out and
    // try to read it
    if (Serial.available() > SIZEOF_HEADER+SIZEOF_CRC)
    {
        MsgType _type;
        uint16_t length;
        OP_Error err = recv_msg(_type, length, rx_buffer);
        if (err == OP_NoError)
            // handle_message(_type, length, rx_buffer);
            if (m_handler)
                m_handler(_type, length, rx_buffer, m_user_data);
    }
}
