//
// Created by Max Sky on 2021/12/16.
//

#include "socket.h"
#include <arpa/inet.h>
#include <thread>

GSocket *gsocket;
GSocketAddress *server_addr;

using namespace std;

void socket::generate_transaction_id(char transaction_id[STUN_HEADER_TRANSACTION_ID_LENGTH]) {
    const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    // 创建随机数生成器
    random_device rd;  // 使用随机设备生成种子
    mt19937 gen(rd()); // Mersenne Twister 随机数引擎
    uniform_int_distribution<> dist(0, (int) charset.size() - 1);

    for (size_t i = 0; i < STUN_HEADER_TRANSACTION_ID_LENGTH; i++) {
        transaction_id[i] = charset[dist(gen)];
    }
}

gboolean socket::resolve_hostname_to_ip(gchar **host) {
    GError *error = nullptr;

    GResolver *resolver = g_resolver_get_default();
    GList *addresses = g_resolver_lookup_by_name(resolver, *host, nullptr, &error);

    if (!addresses) {
        g_print("Failed to resolve domain: %s. Error: %s.\n", *host, error ? error->message : "Unknown error");

        return false;
    }

    for (GList *l = addresses; l != nullptr; l = l->next) {
        GInetAddress *addr = G_INET_ADDRESS(l->data);

        if (addr) {
            *host = g_inet_address_to_string(addr);

            if (*host) {
                g_resolver_free_addresses(addresses);

                return true;
            }
        }
    }

    g_resolver_free_addresses(addresses);

    return false;
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

//    // 绑定本机地址
//    GSocketAddress *local_addr = g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 0);
//
//    gboolean bind_result = g_socket_bind(gsocket, local_addr, false, &error);
//
//    g_object_unref(local_addr);
//
//    if (!bind_result || error) {
//        g_print("Failed to bind socket: %s.\n", error ? error->message : "Unknown error");
//
//        return -1002;
//    }

    if (!resolve_hostname_to_ip(&host)) {
        return -1003;
    }

    server_addr = g_inet_socket_address_new_from_string(host, strtol(port, nullptr, 10)); // 使用 strtol 转换数据类型

    if (!server_addr) {
        g_print("Failed to create server address.\n");

        return -1004;
    }

    header->msg_type = g_htons(STUN_BIND_REQUEST); // 转换为网络字节序
    header->msg_length = g_htons(0);
    generate_transaction_id(header->transaction_id);

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
    StunMessageHeader header;

    int status = build(host, port, &header);

    if (status != 0) {
        return status;
    }

    g_socket_set_blocking(gsocket, false); // 非阻塞模式
    g_socket_set_timeout(gsocket, 10); // timeout 10 sec

    GError *error = nullptr;

    gchar buffer[sizeof(StunMessageHeader)];
    memcpy(buffer, &header, sizeof(StunMessageHeader));

    gssize sent = g_socket_send_to(gsocket, server_addr, buffer, STUN_HEADER_SIZE, nullptr, &error);

    if (sent == -1 || error) {
        if (error && error->code == G_IO_ERROR_TIMED_OUT) {
            g_print("Socket send timeout after 10 seconds.\n");
        } else {
            g_print("STUN request error code: %d\n", error->code);

            g_print("Failed to send STUN request: %s\n", error->message);
        }

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

    auto *response = new gchar[4096];
    gssize received = 0;

    int count = 0;

    while (count <= 20) {
        count++;

        received = g_socket_receive_from(
                gsocket, &src_addr, response, 4096, nullptr, &error);

        if (received == -1 || error) {
            g_print("Error receiving data: %s\n", error->message);

            g_clear_error(&error);
        } else {
            g_print("Received STUN response: %ld bytes.\n", received);

            break;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    if (received == -1) {
        return -3001;
    }

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
