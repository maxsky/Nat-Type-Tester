//
// Created by Max Sky on 2021/9/16.
//

#ifndef __WIN32

#include <netdb.h>
#include <arpa/inet.h>

#endif

#include <gtk/gtk.h>
#include "socket.h"

class client {

public:
    static bool validateRequired(const char *host, const char *port);

    static void request();
};
