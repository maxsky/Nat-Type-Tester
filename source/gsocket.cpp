//
// Created by Max Sky on 2021/9/16.
//

#include "gsocket.h"

using namespace std;

char *gsocket::randstr(unsigned length) {
    const char charset[] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    unsigned charlen = strlen(charset);

    char *str = new char[length];
    str[length] = '\0';

    random_device rd;
    default_random_engine random(rd());

    for (int i = 0; i < length; i++) {
        str[i] = charset[random() % charlen];
    }

    return str;
}

void gsocket::buildRequest(StunMessage *msg, bool change_ip, bool change_port) {
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

    //printf("%s", randstr(12));

    auto *socket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &error);

    if (socket == nullptr) {
        g_error("%s", error->message);
    }

    g_socket_set_blocking(socket, FALSE);

    host = inet_ntoa(*(struct in_addr *) gethostbyname(host)->h_addr_list[0]); // convert to IP

    uint16_t iport = g_ascii_strtoull(port, nullptr, 10);

    printf("STUN Server: %s:%d\n", host, iport);

    GSocketAddress *socket_address = g_inet_socket_address_new(g_inet_address_new_from_string(host), iport);

    StunMessage req;
    memset(&req, 0, sizeof(StunMessage));

    buildRequest(&req, false, false);

    char *buffer = encodeMessage(req);


    gssize len = g_socket_send_to(socket, socket_address, buffer, sizeof(buffer), nullptr, &error);

    if (len == -1) {
        g_error("%s", error->message);
    }

//    auto *localhost = g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 54321);
//
//    if (!g_socket_bind(socket, localhost, true, &error)) {
//        g_error("%s", error->message);
//    }

    char receive[STUN_MAX_MESSAGE_SIZE];

    GInputVector *input = nullptr;

    GSocketControlMessage msg;

    g_socket_receive_message(socket, nullptr, input, 1, nullptr, nullptr, nullptr, nullptr, &error);

    printf("%s", receive);

    g_socket_close(socket, &error);
}
