//
// Created by Max Sky on 2021/9/16.
//

#include "stunmsg.h"

char *encodeMessage(const StunMessage &msg) {
    char *buffer = new char[STUN_MAX_MESSAGE_SIZE];

    buffer = encode16(buffer, msg.msg_header.msg_type);
    buffer = encode(buffer, msg.msg_header.id, sizeof(msg.msg_header.id));

    return buffer;

//    ptr = encode16(ptr, msg.msg_header.msg_type);
//
//    char *lengthp = ptr;
//
//    ptr = encode16(ptr, 0);
//    ptr = encodeAttrAddress(ptr, STUN_MSG_MAPPED_ADDR, msg.mapped_addr);
//    ptr = encodeAttrAddress(ptr, STUN_MSG_RESPONSE_ADDR, msg.resp_addr);
//    ptr = encodeAttrChangeRequest(ptr, msg.change_req);
//    ptr = encodeAttrAddress(ptr, STUN_MSG_SOURCE_ADDR, msg.src_addr);
//    ptr = encodeAttrAddress(ptr, STUN_MSG_CHANGED_ADDR, msg.changed_addr);
//    ptr = encodeAttrString(ptr, STUN_MSG_USERNAME, msg.username);
//    ptr = encodeAttrString(ptr, STUN_MSG_PASSWORD, msg.password);
//    ptr = encodeAttrError(ptr, msg.err);
//    ptr = encodeAttrUnknown(ptr, msg.unknown_attrs);
//    ptr = encodeAttrAddress(ptr, STUN_MSG_REFLECTED_FROM, msg.reflected_from);
//    ptr = encodeAttrAddress(ptr, STUN_MSG_XOR_MAPPED_ADDR, msg.xor_mapped_addr);
//    ptr = encodeXorOnly(ptr);
//    ptr = encodeAttrString(ptr, STUN_MSG_SERVER_NAME, msg.serv_name);
//    ptr = encodeAttrAddress(ptr, STUN_MSG_SECONDARY_ADDR, msg.secondary_addr);
//
//    encode16(lengthp, uint16_t(ptr - buffer - sizeof(StunMessageHeader)));
//    return int(ptr - buffer);
}
