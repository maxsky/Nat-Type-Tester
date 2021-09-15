//
// Created by Max Sky on 9/16/2021.
//

#include <gtk/gtk.h>
#include "layout.h"
#include "component.h"

bool validateRequired(const char *host, const char *port) {
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

void test() {
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

    auto iport = g_ascii_strtoull(port, nullptr, 10);;

    auto *address = g_inet_address_new_from_string("216.93.246.18");
    auto *socket_address = g_inet_socket_address_new(address, iport);

    gssize len = g_socket_send_to(socket, socket_address, "0", 1, nullptr, &error);

    printf("%l", len);

    if (error != nullptr) {
        g_error("%s", error->message);
    }

    gchar data;

    g_socket_receive(socket, &data, 32, nullptr, &error);

    printf(reinterpret_cast<const char *>(data));

    g_socket_close(socket, &error);
}

void layout::init() {
    GtkWidget *window = component::getWindow();
    component::setWindow(window);

    GtkWidget
            *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0),
            *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    component::setLabel(hbox);
    component::setEditBox(hbox);
    component::setPort(hbox);
    component::setButton(vbox, test);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
