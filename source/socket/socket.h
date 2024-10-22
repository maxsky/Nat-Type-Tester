//
// Created by Max Sky on 2021/12/16.
//

#ifndef NATTYPETESTER_SOCKET_H
#define NATTYPETESTER_SOCKET_H


#include <random>
#include "../message/msgstruct.h"

class socket {

public:
    static int send(const char *host, const char *port);

    static int receive();

private:
    static std::string randstr(unsigned len);

    static GSocketAddress *build(const char *host, const char *port, StunMessageHeader *header);

    static void parse(const char *response, size_t len);

    static void close();
};

#endif //NATTYPETESTER_SOCKET_H
