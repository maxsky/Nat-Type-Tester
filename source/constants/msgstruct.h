//
// Created by Max Sky on 2021/9/16.
//

#include <cstdint>

#define STUN_MAX_STRING 256
#define STUN_MAX_UNKNOWN_ATTRIBUTES 8
#define STUN_MAX_MESSAGE_SIZE 1024

/** STUN Method */
#define STUN_METHOD_BIND       0x001
#define STUN_METHOD_SHARED_SEC 0x002 // reserved

/** Message Headers  */
#define STUN_HEADER_SIZE                     20
#define STUN_HEADER_TRANSACTION_ID_LENGTH    12

/** Message Types */
#define STUN_BIND_REQUEST          0x0001
#define STUN_BIND_RESPONSE         0x0101
#define STUN_BIND_ERROR_RESP       0x0111
#define STUN_SHARED_SEC_REQUEST    0x0002
#define STUN_SHARED_SEC_RESPONSE   0x0102
#define STUN_SHARED_SEC_ERROR_RESP 0x0112

/** Message Attributes */
#define STUN_MSG_MAPPED_ADDR     0x0001
#define STUN_MSG_RESPONSE_ADDR   0x0002 // reserved
#define STUN_MSG_CHANGE_REQUEST  0x0003 // reserved
#define STUN_MSG_SOURCE_ADDR     0x0004 // reserved
#define STUN_MSG_CHANGED_ADDR    0x0005 // reserved
#define STUN_MSG_USERNAME        0x0006
#define STUN_MSG_PASSWORD        0x0007 // reserved
#define STUN_MSG_INTEGRITY       0x0008
#define STUN_MSG_ERROR_CODE      0x0009
#define STUN_MSG_UNKNOWN_ATTRS   0x000A
#define STUN_MSG_REFLECTED_FROM  0x000B // reserved
#define STUN_MSG_REALM           0x0014
#define STUN_MSG_NONCE           0x0015
#define STUN_MSG_XOR_MAPPED_ADDR 0x0020
#define STUN_MSG_XOR_ONLY        0x0021
#define STUN_MSG_SERVER_NAME     0x8022
#define STUN_MSG_SECONDARY_ADDR  0x8050 // Non standard extension

#define STUN_FLAG_CHANGE_IP      0x04
#define STUN_FLAG_CHANGE_PORT    0x02

typedef struct {
    uint16_t msg_type;
    uint16_t msg_length;
    char transaction_id[STUN_HEADER_TRANSACTION_ID_LENGTH]; // 12 bytes
} StunMessageHeader;
