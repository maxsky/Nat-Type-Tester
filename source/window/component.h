//
// Created by Max Sky on 2021/9/15.
//

#include <gtk/gtk.h>

class component {

public:
    static GtkWidget *server;
    static GtkWidget *port;
    static GtkWidget *nat_type;
    static GtkWidget *addr_local;
    static GtkWidget *addr_public;

    static GtkWidget *getWindow();

    static void setWindow(GtkWidget *wWindow);

    static void setLabel(GtkWidget *pBox);

    static void setEditBox(GtkWidget *pBox);

    static void setPort(GtkWidget *pBox);

    static void setButton(GtkWidget *pBox, GCallback func);

private:
    static void on_server_changed(GtkComboBox *widget);
};
