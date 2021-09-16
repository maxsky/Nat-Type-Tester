//
// Created by Max Sky on 9/16/2021.
//

#include <gtk/gtk.h>

#include "layout.h"
#include "component.h"
#include "gsocket.h"

void handler() {
    gsocket::receiveData();
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

    component::setButton(vbox, handler);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
