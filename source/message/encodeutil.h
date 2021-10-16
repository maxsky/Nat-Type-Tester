//
// Created by Max Sky on 2021/9/16.
//

#ifndef NATTYPETESTER_ENCODEUTIL_H
#define NATTYPETESTER_ENCODEUTIL_H

#endif //NATTYPETESTER_ENCODEUTIL_H

#ifdef __WIN32

#include <winsock2.h>

#else

#include <netinet/in.h>

#endif

#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "msgstruct.h"

char *encode(char *buf, const char *data, unsigned int length);

char *encode16(char *buf, uint16_t data);

char *encode32(char *buf, uint32_t data);

char *encodeAttrAddress(char *ptr, uint16_t type, const StunAttrAddress &attr);

char *encodeAttrChangeRequest(char *ptr, const StunAttrChangeRequest &attr);

char *encodeAttrString(char *ptr, uint16_t type, const StunAttrString &attr);

char *encodeAttrError(char *ptr, const StunAttrError &attr);

char *encodeAttrUnknown(char *ptr, const StunAttrUnknown &atr);

char *encodeXorOnly(char *ptr);
