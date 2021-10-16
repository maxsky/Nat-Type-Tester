//
// Created by Max Sky on 2021/9/16.
//

#include "encodeutil.h"

char *encode(char *buf, const char *data, unsigned int length) {
    memcpy(buf, data, length);
    return buf + length;
}

char *encode16(char *buf, uint16_t data) {
    data = htons(data);
    memcpy(buf, &data, sizeof(uint16_t));

    return buf + sizeof(uint16_t);
}

char *encode32(char *buf, uint32_t data) {
    data = htonl(data);
    memcpy(buf, &data, sizeof(uint32_t));

    return buf + sizeof(uint32_t);
}

char *encodeAttrAddress(char *ptr, uint16_t type, const StunAttrAddress &attr) {
    ptr = encode16(ptr, type);
    ptr = encode16(ptr, 8);
    *ptr++ = attr.pad;
    *ptr++ = 2;
    ptr = encode32(ptr, attr.ipv4.addr);
    ptr = encode16(ptr, attr.ipv4.port);

    return ptr;
}

char *encodeAttrChangeRequest(char *ptr, const StunAttrChangeRequest &attr) {
    ptr = encode16(ptr, STUN_MSG_CHANGE_REQUEST);
    ptr = encode16(ptr, 4);
    ptr = encode32(ptr, attr.value);

    return ptr;
}

char *encodeAttrString(char *ptr, uint16_t type, const StunAttrString &attr) {
    ptr = encode16(ptr, type);
    ptr = encode16(ptr, attr.size);

    return encode(ptr, attr.value, attr.size);
}

char *encodeAttrError(char *ptr, const StunAttrError &attr) {
    ptr = encode16(ptr, STUN_MSG_ERROR_CODE);
    ptr = encode16(ptr, 6 + attr.reason_size);
    ptr = encode16(ptr, attr.pad);
    *ptr++ = attr.err_class;
    *ptr++ = attr.code;

    return encode(ptr, attr.reason, attr.reason_size);
}

char *encodeAttrUnknown(char *ptr, const StunAttrUnknown &attr) {
    ptr = encode16(ptr, STUN_MSG_UNKNOWN_ATTRS);
    ptr = encode16(ptr, 2 + 2 * attr.attr_num);

    for (int i = 0; i < attr.attr_num; i++) {
        ptr = encode16(ptr, attr.attr_type[i]);
    }

    return ptr;
}

char *encodeXorOnly(char *ptr) {
    return encode16(ptr, STUN_MSG_XOR_ONLY);
}
