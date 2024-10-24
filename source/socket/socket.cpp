//
// Created by Max Sky on 2021/12/16.
//

#include <arpa/inet.h>
#include <gtk/gtk.h>
#include "socket.h"

GSocket *gsocket;
GSocketAddress *server_addr;

using namespace std;

char *socket::generate_transaction_id(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    char *transaction_id = new char[length];

    for (size_t i = 0; i < length; i++) {
        int key = rand() % (int) (sizeof(charset) - 1);

        transaction_id[i] = charset[key];
    }

    return transaction_id;
}

// 序列化 STUN 请求为字节数组
char *socket::serialize_stun_header(StunMessageHeader *header) {
    char *buffer = static_cast<char *>(malloc(STUN_HEADER_SIZE));

    // 序列化消息类型和消息长度，转换为网络字节序
    guint16 msg_type_net = htons(header->msg_type);
    guint16 msg_length_net = htons(header->msg_length);

    // 将数据复制到缓冲区
    memcpy(buffer, &msg_type_net, sizeof(msg_type_net));      // 复制消息类型
    memcpy(buffer + 2, &msg_length_net, sizeof(msg_length_net));  // 复制消息长度
    memcpy(buffer + 4, header->transaction_id, STUN_HEADER_TRANSACTION_ID_LENGTH);  // 复制事务 ID

    return buffer;
}

/**
 * @param host
 * @param port
 * @param header
 *
 * @return
 */
int socket::build(gchar *host, const gchar *port, StunMessageHeader *header) {
    GError *error = nullptr;

    gsocket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &error);

    if (!gsocket || error) {
        g_print("Failed to create socket: %s.\n", error ? error->message : "Unknown error");

        return -1001;
    }

    GResolver *resolver = g_resolver_get_default();
    GList *addresses = g_resolver_lookup_by_name(resolver, host, nullptr, &error);

    if (!addresses) {
        g_print("Failed to resolve domain: %s. Error: %s.\n", host, error ? error->message : "Unknown error");

        return -1002;
    }

    for (GList *l = addresses; l != nullptr; l = l->next) {
        GInetAddress *addr = G_INET_ADDRESS(l->data);

        if (addr) {
            host = g_inet_address_to_string(addr);

            if (host) {
                break;
            }
        }
    }

    g_resolver_free_addresses(addresses);

    server_addr = g_inet_socket_address_new_from_string(host, *(guint16 *) port);

    if (!server_addr) {
        g_print("Failed to create server address.\n");

        return -1003;
    }

    header->msg_type = STUN_BIND_REQUEST; // STUN Binding Request
    header->msg_length = 0;
    header->transaction_id = generate_transaction_id(12);

    return 0;
}

void socket::parse(const gchar *response, gssize size) {
    if (size < sizeof(StunMessageHeader)) {
        g_print("Invalid STUN response\n");

        return;
    }

    guint16 response_code = (response[0] << 8) | response[1];  // 将前两个字节组合为 16 位整数

    if (response_code != STUN_BIND_RESPONSE) {
        g_print("Received non-success STUN response: 0x%04x\n", response_code);

        return;
    }

    int pos = STUN_HEADER_SIZE;

    while (pos < size) {
        guint16 attr_type = (response[pos] << 8) | response[pos + 1];
        guint16 attr_length = (response[pos + 2] << 8) | response[pos + 3];

        if (attr_type == 0x0001) {  // MAPPED-ADDRESS
            guint8 family = response[pos + 5];  // 地址家族 (0x01 表示 IPv4)
            if (family == 0x01) {
                guint16 port = (response[pos + 6] << 8) | response[pos + 7];
                guint32 ip = (response[pos + 8] << 24) |
                             (response[pos + 9] << 16) |
                             (response[pos + 10] << 8) |
                             response[pos + 11];
                g_print("External IP: %d.%d.%d.%d, Port: %d\n",
                        (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF, port);
            }
        }

        pos += 4 + attr_length;  // 下一个属性的位置
    }
}

/**
 * @param msg
 */
int socket::send(char *host, const char *port) {
    GError *error = nullptr;

    StunMessageHeader header;

    int status = build(host, port, &header);

    if (status != 0) {
        return status;
    }

    char *buffer = serialize_stun_header(&header);

    gssize sent = g_socket_send_to(gsocket, server_addr, buffer, STUN_HEADER_SIZE, nullptr, &error);

    if (error || sent < 0) {
        g_print("Failed to send STUN request: %s\n", error ? error->message : "Unknown error");

        return -2001;
    }

    g_print("Sent STUN request: %ld bytes.\n", sent);

    receive();

    return 0;
}

/**
 * @return
 */
int socket::receive() {
    GError *error = nullptr;

    GSocketAddress *src_addr = nullptr;

    auto *response = new gchar[1024];

    // g_socket_set_blocking(gsocket, false);
    g_socket_set_timeout(gsocket, 10); // sec

    gssize received = g_socket_receive_from(
            gsocket, &src_addr, response, sizeof(response), nullptr, &error);

    if (received == -1) {
        g_print("STUN response error code: %d\n", error->code);

        if (error->code == G_IO_ERROR_TIMED_OUT) {
            g_print("Socket receive timeout after 10 seconds.\n");
        } else {
            g_print("Error receiving data: %s\n", error ? error->message : "Unknown error");
        }

        return -2002;
    }

    g_print("Received STUN response: %ld bytes.\n", received);

    parse(response, received);

    close();

    return 0;
}

void socket::close() {
    GError *error;

    g_socket_close(gsocket, &error);

    g_object_unref(gsocket);
    g_object_unref(server_addr);
}
