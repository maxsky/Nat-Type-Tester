//
// Created by Max Sky on 2021/9/16.
//

#include "client.h"

using namespace std;

char *client::randstr(unsigned length) {
    const char charset[] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    unsigned charlen = strlen(charset);

    char *str = new char[length];
    str[length] = '\0';

    random_device rand;
    default_random_engine random(rand());

    for (int i = 0; i < length; i++) {
        str[i] = charset[random() % charlen];
    }

    return str;
}

bool client::validateRequired(const char *host, const char *port) {
    if (host != nullptr && host[0] == '\0') {
        GtkWidget *dialog = gtk_message_dialog_new(nullptr,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_CLOSE,
                                                   "\nPlease select or input a STUN server.");

        GtkWindow *window = GTK_WINDOW(dialog);

        gtk_window_set_title(window, "Warning");
        gtk_window_set_position(window, GTK_WIN_POS_CENTER);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return false;
    }

    if (port != nullptr && port[0] == '\0') {
        GtkWidget *dialog = gtk_message_dialog_new(nullptr,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_CLOSE,
                                                   "\nPlease input port number, the default port is [3478].");

        GtkWindow *window = GTK_WINDOW(dialog);

        gtk_window_set_title(window, "Warning");
        gtk_window_set_position(window, GTK_WIN_POS_CENTER);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return false;
    }

    return true;
}

void client::buildRequest(StunMessage *msg, bool change_ip, bool change_port) {
    msg->msg_header.msg_type = STUN_BIND_REQUEST;
    msg->msg_header.id = randstr(12);
    msg->change_req.value = (change_ip ? STUN_FLAG_CHANGE_IP : 0) | (change_port ? STUN_FLAG_CHANGE_PORT : 0);
}

void client::request() {
    const gchar *host = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(component::server));
    const gchar *port = gtk_entry_get_text(GTK_ENTRY(component::port));

    if (!validateRequired(host, port)) {
        return;
    }

    socket::build(host, port);

    StunMessage msg;

    socket::send(msg);

    socket::close();
}
