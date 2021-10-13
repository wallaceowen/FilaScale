/* protocol.cpp */

#include <Arduino.h>
#include <string.h>

#include "octo_protocol.h"
#include "crc16.h"


#define SIZEOF_SYNC 1
#define SIZEOF_MSG_TYPE 1
#define SIZEOF_MSG_LENGTH 2
#define SIZEOF_HEADER (SIZEOF_SYNC+SIZEOF_MSG_TYPE+SIZEOF_MSG_LENGTH)
#define SIZEOF_CRC 2

typedef int8_t type_type;
typedef uint16_t length_type;

void show_crc()
{
    char fake_msg[] = {'U', 0, 0, 0 };
    uint16_t crc = 0;
    calcCRCArray16((uint8_t*)fake_msg, sizeof(fake_msg), &crc);
    Serial.print("CRC of fake msg: ");
    Serial.println(crc, HEX);
}

OctoProtocol::OctoProtocol() :
    m_handler(0),
    m_user_data(0)
{
}

void dump_bytes(const char *title, const unsigned char *bytes, int len)
{
    char dbg[12];
    Serial.print(title);
    Serial.print(": ");
    for (int i = 0; i < len; ++i)
    {
        sprintf(dbg, "%2x", bytes[i]);
        Serial.print(dbg);
        if (i)
            Serial.print(" ");
    }
    Serial.println("");
}

// Handler is invoked with messages as they arrive.
void OctoProtocol::set_handler(MessageHandlerFunc func, void *user_data)
{
    m_handler = func;
    m_user_data = user_data;
}

OctoProtocol::OP_Error OctoProtocol::send_msg(MsgType _type, uint16_t length, const char *body)
{
    uint16_t xcrc = 0;
    if (Serial.write(SYNC_BYTE) != SIZEOF_SYNC)
        return OP_NoConnection;
    else
    {
        uint8_t sync_byte = SYNC_BYTE;
        calcCRCArray16(&sync_byte, SIZEOF_SYNC, &xcrc);
    }

    if (Serial.write(static_cast<type_type>(_type)) != SIZEOF_MSG_TYPE)
        return OP_NoConnection;
    else
        calcCRCArray16(reinterpret_cast<uint8_t*>(&_type), SIZEOF_MSG_TYPE, &xcrc);

    if (Serial.write(reinterpret_cast<uint8_t*>(&length), SIZEOF_MSG_LENGTH) != SIZEOF_MSG_LENGTH)
        return OP_NoConnection;
    else
        calcCRCArray16(reinterpret_cast<uint8_t*>(&length), SIZEOF_MSG_LENGTH, &xcrc);

    if (Serial.write(body, length) != length)
        return OP_NoConnection;
    else
        calcCRCArray16(reinterpret_cast<const uint8_t*>(body), length, &xcrc);

    if (Serial.write(reinterpret_cast<uint8_t*>(&xcrc), SIZEOF_CRC) != SIZEOF_CRC)
        return OP_NoConnection;

    return OP_NoError;
}

int OctoProtocol::recv_bytes(unsigned char *buffer, uint16_t qty) const
{
    size_t rcount = Serial.readBytes(buffer, qty);
    return rcount;
}

// Attempt to read an entire message 
OctoProtocol::OP_Error OctoProtocol::recv_msg(MsgType &_type, uint16_t &length, char *msg) const
{
    // Read bytes until we find the sync byte or we time out
    unsigned char *bptr = reinterpret_cast<unsigned char *>(msg);
    while (1)
    {
        int ch = Serial.read();
        if (ch == -1)
            return OP_NoData;

        if (ch == SYNC_BYTE)
            break;
    }

    *bptr++=SYNC_BYTE;

    if (!Serial.readBytes(bptr, SIZEOF_MSG_TYPE))
        return OP_NoData;
    // Save the type
    // char t = *bptr++;
    // _type = static_cast<MsgType>(t);
    _type = static_cast<MsgType>(*bptr++);

    if (_type >= NUM_VALID_MESSAGES)
        return OP_Bad_MsgType;

    // Get the length or time out
    // if (!Serial.readBytes(body+SIZEOF_MSG_TYPE, SIZEOF_MSG_LENGTH))
        // return OP_NoData;
    if (!Serial.readBytes(bptr, SIZEOF_MSG_LENGTH))
        return OP_NoData;

    length = *reinterpret_cast<uint16_t*>(bptr);
    bptr += SIZEOF_MSG_LENGTH;
    if (length > MAX_BODY_SIZE)
        return OP_BadSize;

    if (length)
    {
        // Try to read the body
        if (!Serial.readBytes(bptr, length))
            return OP_NoData;
        bptr += length;
    }

    // Read the transmitted crc
    uint16_t rcrc;
    // if (!Serial.readBytes(reinterpret_cast<char*>(&rcrc), SIZEOF_CRC))
    if (!Serial.readBytes(bptr, SIZEOF_CRC))
        return OP_NoData;
    rcrc = *reinterpret_cast<uint16_t*>(bptr);
    bptr += SIZEOF_CRC;

    // dump_bytes("received msg", buffer, bptr-buffer);

    // Compute the CRC of the bytes we received in the message
    uint16_t msize = SIZEOF_SYNC+SIZEOF_MSG_TYPE+SIZEOF_MSG_LENGTH+length;
    uint16_t ccrc = 0;
    calcCRCArray16(reinterpret_cast<unsigned char *>(msg), msize, &ccrc);

    if (ccrc != rcrc)
        return OP_Bad_CRC;

    return OP_NoError;
}


static char message[OctoProtocol::MAX_RX_MSG_SIZE];

void OctoProtocol::loop()
{
    // If there's enough bytes waiting for a minimum message check it out and
    // try to read it
    if (Serial.available() >= SIZEOF_HEADER+SIZEOF_CRC)
    {
        MsgType _type;
        uint16_t length;
        // OP_Error err = recv_msg(_type, length, body);
        OP_Error err = recv_msg(_type, length, message);
        if (err == OP_NoError)
            // show_crc();
            if (m_handler)
                // m_handler(_type, length, body, m_user_data);
                m_handler(_type, length, message+HEADER_SIZE, m_user_data);
    }
}
