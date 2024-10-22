//
// Created by Max Sky on 2021/9/16.
//

#include "client.h"

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

void client::request() {
    const gchar *host = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(component::server));
    const gchar *port = gtk_entry_get_text(GTK_ENTRY(component::port));

    if (!validateRequired(host, port)) {
        return;
    }

    socket::send(host, port);
    socket::receive();
}
