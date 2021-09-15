//
// Created by Max Sky on 2021/9/15.
//

#include <gtk/gtk.h>
#include <sys/socket.h>
#include "type_test.h"

void test() {
    GError **error = nullptr;

    GSocket *socket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, error);

    printf("%d", g_error_get_type());

    g_socket_close(socket, error);
}