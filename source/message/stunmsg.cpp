//
// Created by Max Sky on 2021/9/16.
//

#include "stunmsg.h"
#include <cstdio>

unsigned int encodeMessage(const StunMessage &msg, char *buffer) {
    char *ptr = buffer;

    ptr = encode16(ptr, msg.msg_header.msg_type);

    char *lengthp = ptr;

    ptr = encode16(ptr, 0);
    ptr = encode(ptr, msg.msg_header.id, sizeof(msg.msg_header.id));

    if (msg.has_mapped_addr) {
        ptr = encodeAttrAddress(ptr, STUN_MSG_MAPPED_ADDR, msg.mapped_addr);
    }

    if (msg.has_resp_addr) {
        ptr = encodeAttrAddress(ptr, STUN_MSG_RESPONSE_ADDR, msg.resp_addr);
    }

    if (msg.has_changed_req) {
        ptr = encodeAttrChangeRequest(ptr, msg.change_req);
    }

    if (msg.has_src_addr) {
        ptr = encodeAttrAddress(ptr, STUN_MSG_SOURCE_ADDR, msg.src_addr);
    }

    if (msg.has_changed_addr) {
        ptr = encodeAttrAddress(ptr, STUN_MSG_CHANGED_ADDR, msg.changed_addr);
    }

    if (msg.has_username) {
        ptr = encodeAttrString(ptr, STUN_MSG_USERNAME, msg.username);
    }

    if (msg.has_password) {
        ptr = encodeAttrString(ptr, STUN_MSG_PASSWORD, msg.password);
    }

    if (msg.has_err) {
        ptr = encodeAttrError(ptr, msg.err);
    }

    if (msg.has_unknown_attrs) {
        ptr = encodeAttrUnknown(ptr, msg.unknown_attrs);
    }

    if (msg.has_reflected_from) {
        ptr = encodeAttrAddress(ptr, STUN_MSG_REFLECTED_FROM, msg.reflected_from);
    }

    if (msg.has_xor_mapped_addr) {
        ptr = encodeAttrAddress(ptr, STUN_MSG_XOR_MAPPED_ADDR, msg.xor_mapped_addr);
    }

    if (msg.xor_only) {
        ptr = encodeXorOnly(ptr);
    }

    if (msg.has_serv_name) {
        ptr = encodeAttrString(ptr, STUN_MSG_SERVER_NAME, msg.serv_name);
    }

    if (msg.has_secondary_addr) {
        ptr = encodeAttrAddress(ptr, STUN_MSG_SECONDARY_ADDR, msg.secondary_addr);
    }

    encode16(lengthp, uint16_t(ptr - buffer - sizeof(StunMessageHeader)));
    return int(ptr - buffer);
}
