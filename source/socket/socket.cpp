//
// Created by Max Sky on 2021/12/16.
//

#include <gtk/gtk.h>
#include <arpa/inet.h>
#include "socket.h"

GSocket *gsocket;

using namespace std;

string socket::randstr(unsigned length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;  // 用于随机数种子
    std::mt19937 generator(rd());  // 使用 Mersenne Twister 随机数生成器
    std::uniform_int_distribution<> dist(0, charset.size() - 1);

    std::string random_string;
    random_string.reserve(length);  // 提前分配好长度

    for (size_t i = 0; i < length; i++) {
        random_string += charset[dist(generator)];
    }

    return random_string;
}

/**
 * @param host
 * @param port
 * @return int
 */
GSocketAddress *socket::build(const char *host, const char *port, StunMessageHeader *header) {
    GError * error = nullptr;

    gsocket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &error);

    if (error || !gsocket) {
        printf("Failed to create socket: %s\n", error ? error->message : "Unknown error");

        exit(EXIT_FAILURE);
    }

    header->msg_type = STUN_BIND_REQUEST; // g_htons(0x0001)  // STUN Binding Request
    header->msg_len = 0;  // 没有额外的数据
    header->id = randstr(12);

    GInetAddress *server_ip = g_inet_address_new_from_string(host);

    return g_inet_socket_address_new(server_ip, *port);
}

void socket::parse(const char *response, size_t len) {
    if (len < sizeof(StunMessageHeader)) {
        printf("Invalid STUN response\n");
        return;
    }

    // 跳过 STUN header，查找 XOR-MAPPED-ADDRESS 属性
    const auto *ptr = (const unsigned char *) (response + sizeof(StunMessageHeader));
    while (ptr < (const unsigned char *) (response + len)) {
        uint16_t attr_type = (ptr[0] << 8) | ptr[1];
        uint16_t attr_len = (ptr[2] << 8) | ptr[3];
        ptr += 4;

        if (attr_type == 0x0020 && attr_len >= 8) {  // XOR-MAPPED-ADDRESS
            uint8_t family = ptr[1];
            uint16_t port = (ptr[2] << 8) | ptr[3];
            port ^= 0x2112;

            if (family == 0x01) {  // IPv4
                uint32_t ip = (ptr[4] << 24) | (ptr[5] << 16) | (ptr[6] << 8) | ptr[7];
                ip ^= 0x2112A442;

                printf("External IP: %u.%u.%u.%u\n", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
                printf("External Port: %u\n", port);
            }
        }

        ptr += attr_len;  // 移动到下一个属性
    }
}

/**
 * @param msg
 */
int socket::send(const char *host, const char *port) {
    StunMessageHeader header;

    GSocketAddress *server_addr = build(host, port, &header);

    GError * error = nullptr;

    if (!gsocket) {
        printf("Failed to create socket\n");

        return -1;
    }

    gchar buffer[sizeof(StunMessageHeader)];
    memcpy(buffer, &header, sizeof(StunMessageHeader));

    gssize sent = g_socket_send_to(gsocket, server_addr, buffer, sizeof(buffer), nullptr, &error);

    if (error || sent < 0) {
        printf("Failed to send STUN request: %s\n", error ? error->message : "Unknown error");
        return -1;
    }

    return 0;
}

/**
 * @return
 */
int socket::receive() {
    GError * error = nullptr;

    GSocketAddress *src_addr = nullptr;

    char response[512];

    gssize received = g_socket_receive_from(gsocket, &src_addr, response, sizeof(response), nullptr, &error);

    if (error || received < 0) {
        printf("Failed to receive STUN response: %s\n", error ? error->message : "Unknown error");

        return -1;
    }

    socket::parse(response, received);

    g_object_unref(gsocket);

    return 0;
}

void socket::close() {
    GError * error;

    g_socket_close(gsocket, &error);
}
