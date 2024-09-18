//
// Created by Max Sky on 2021/12/16.
//

#include <gtk/gtk.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include "socket.h"

GSocket *gsocket;

/**
 * @param host
 * @param port
 * @return int
 */
int socket::build(const char *host, const char *port) {
    int res;

    pthread_t recv_thread;
    pthread_attr_t thread_attr;

    /* set status of thread */
    res = pthread_attr_init(&thread_attr);

    if (res != 0) {
        perror("Setting detached attribute failed");
        exit(EXIT_FAILURE);
    }

    GError *error;

    gsocket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &error);

    if (gsocket == nullptr) {
        g_error("%s", error->message);
    }

    // g_socket_set_blocking(gsocket, FALSE);

    g_socket_set_option(gsocket, IPPROTO_TCP, TCP_NODELAY, TRUE, &error);

    host = inet_ntoa(*(struct in_addr *) gethostbyname(host)->h_addr_list[0]); // convert to IP

    uint16_t iport = g_ascii_strtoull(port, nullptr, 10);

    printf("STUN Server: %s:%d\n", host, iport);

    /* set the stats of thread:means do not wait for the return value of subthread */
    res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (res != 0) {
        perror("Setting detached attribute failed");
        exit(EXIT_FAILURE);
    }

    /* Create a thread to process the reception function. */
    res = pthread_create(&recv_thread, &thread_attr, &socket::receive, nullptr);

    if (res != 0) {
        perror("Thread create error");
        exit(EXIT_FAILURE);
    }

    /* callback the attribute */
    (void) pthread_attr_destroy(&thread_attr);

    return 0;
}

/**
 * @param msg
 */
void socket::send(StunMessage msg) {
    GError *error;

    gchar *buffer = encodeMessage(msg);

    gssize len = g_socket_send(gsocket, buffer, sizeof(buffer), nullptr, &error);

    if (len < 0) {
        g_error("%s", error->message);
    }
}

/**
 * @param arg
 * @return
 */
void *socket::receive(void *arg) {
    gchar *buffer = nullptr;

    while (strlen(buffer) <= 0) {
        /* 阻塞直到收到客户端发的消息 */
        g_socket_receive(gsocket, buffer, STUN_MAX_MESSAGE_SIZE, nullptr, nullptr);

        printf("%s", buffer);
    }
}

void socket::close() {
    GError *error;

    g_socket_close(gsocket, &error);
}
