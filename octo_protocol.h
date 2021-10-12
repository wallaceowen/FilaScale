/* protocol.cpp */

#ifndef __octo_protocol_h
#define __octo_protocol_h

#include <stdint.h>

typedef void (*MessageHandlerFunc)(uint8_t, uint16_t, char*, void *);

class OctoProtocol
{
public:
    enum OP_Error {
        OP_NoError,
        OP_NoConnection,
        OP_NoData,
        OP_ShortMsg,
        OP_BadSize,
        OP_RetriesExhausted,
        OP_Bad_CRC,
        OP_Bad_MsgType
    };
    enum Constants { BaudRate=115200, HEADER_SIZE=4, MAX_BODY_SIZE=160, SYNC_BYTE=0x55 };

    // enum RX_State { RS_Sync, RS_Type, RS_Addr, RS_Body, RS_CRC, RS_Ready };

    // These must track the message types declared in the FilaMon plugin for octoprint
    enum MsgType { MT_STATUS, MT_CONFIG, MT_START, MT_STOP, MT_THRESHOLD, NUM_VALID_MESSAGES };


    OctoProtocol();
    void set_handler(MessageHandlerFunc func, void *user_data);

    OP_Error recv_msg(MsgType &_type, uint16_t &length, char *body) const;
    OP_Error send_msg(MsgType _type, uint16_t length, const char *body);

    void loop();

private:
    int recv_bytes(unsigned char *buffer, uint16_t qty) const;

    MessageHandlerFunc m_handler;
    void *m_user_data;
};

#endif
