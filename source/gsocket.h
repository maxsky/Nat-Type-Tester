//
// Created by Max Sky on 2021/9/16.
//

#ifndef NATTYPETESTER_GSOCKET_H
#define NATTYPETESTER_GSOCKET_H

#ifndef __WIN32

#include <netdb.h>
#include <arpa/inet.h>

#endif

#include <gtk/gtk.h>
#include <random>
#include "component.h"
#include "message/stunmsg.h"

class gsocket {

public:
    static void receiveData();

    static char *randstr(int len);

    static void buildRequest(StunMessage *msg, bool change_ip, bool change_port);

    static bool validateRequired(const char *host, const char *port);
};

#endif //NATTYPETESTER_GSOCKET_H
