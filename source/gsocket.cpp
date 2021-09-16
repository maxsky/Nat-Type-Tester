//
// Created by Max Sky on 2021/9/16.
//

#include "gsocket.h"

char *gsocket::randstr(const int len) {
    srandom(time(nullptr));

    char *str;
    int i;

    for (i = 0; i < len; ++i) {
        switch ((random() % 3)) {
            case 1:
                str[i] = 'A' + random() % 26;
                break;
            case 2:
                str[i] = 'a' + random() % 26;
                break;
            default:
                str[i] = '0' + (random() % 10);
                break;
        }
    }

    return str;
}

void gsocket::buildRequest(StunMessage *msg, bool change_ip, bool change_port) {
    msg->has_changed_req = true;
    msg->msg_header.msg_type = STUN_BIND_REQUEST;
    msg->msg_header.id = randstr(12);
    msg->change_req.value = (change_ip ? STUN_FLAG_CHANGE_IP : 0) | (change_port ? STUN_FLAG_CHANGE_PORT : 0);
}

bool gsocket::validateRequired(const char *host, const char *port) {
    if (host != nullptr && host[0] == '\0') {
        GtkWidget *dialog = gtk_message_dialog_new(nullptr,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_CLOSE,
                                                   "\nPlease select or input a STUN server.");

        gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return false;
    }

    if (port != nullptr && port[0] == '\0') {
        GtkWidget *dialog = gtk_message_dialog_new(nullptr,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_CLOSE,
                                                   "\nPlease input port number, the default port is '3478'.");

        gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return false;
    }

    return true;
}

void gsocket::receiveData() {
    const gchar *host = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(component::server));
    const gchar *port = gtk_entry_get_text(GTK_ENTRY(component::port));

    if (!validateRequired(host, port)) {
        return;
    }

    GError *error;

    auto *socket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &error);

    if (socket == nullptr) {
        g_error("%s", error->message);
    }

    g_socket_set_blocking(socket, FALSE);

    host = inet_ntoa(*(struct in_addr *) gethostbyname(host)->h_addr_list[0]); // convert to IP

    uint iport = g_ascii_strtoull(port, nullptr, 10);

    printf("STUN Server: %s:%d\n", host, iport);

    auto *socket_address = g_inet_socket_address_new(g_inet_address_new_from_string(host), iport);

    StunMessage req;
    memset(&req, 0, sizeof(StunMessage));

    buildRequest(&req, false, false);

    printf("%d\n", req.msg_header.msg_type);

    char buffer[STUN_MAX_MESSAGE_SIZE];

    unsigned int bufferLen = encodeMessage(req, buffer);

    gssize len = g_socket_send_to(socket, socket_address, buffer, bufferLen, nullptr, &error);

    if (len == -1) {
        g_error("%s", error->message);
    }

    auto *localhost = g_inet_socket_address_new(g_inet_address_new_from_string("0.0.0.0"), 54321);

    if (!g_socket_bind(socket, localhost, true, &error)) {
        g_error("%s", error->message);
    }

    char receive[STUN_MAX_MESSAGE_SIZE];

    g_socket_receive(socket, receive, STUN_MAX_MESSAGE_SIZE, nullptr, &error);

    g_socket_close(socket, &error);
}
