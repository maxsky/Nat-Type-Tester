//
// Created by Max Sky on 2021/12/16.
//

#ifndef NATTYPETESTER_SOCKET_H
#define NATTYPETESTER_SOCKET_H

#include "../message/stunmsg.h"

class socket {

public:
    static int build(const char *serv_ip, const char *port);

    static void send(StunMessage msg);

    static void *receive(void *arg);

    static void close();
};

#endif //NATTYPETESTER_SOCKET_H
