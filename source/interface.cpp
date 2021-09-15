//
// Created by Max Sky on 2021/9/15.
//

#include <gtk/gtk.h>
#include "interface.h"
#include "type_test.h"

void setWindow(GtkWidget *wWindow) {
    GtkWindow *window = GTK_WINDOW(wWindow);

    gtk_window_set_title(window, "Net Type Tester");
    gtk_window_set_resizable(window, FALSE);
    //gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DROPDOWN_MENU);
    gtk_window_unmaximize(window);
    gtk_window_set_position(window, GTK_WIN_POS_NONE);
    gtk_window_activate_default(window);
    gtk_window_activate_focus(window);

    gtk_container_set_border_width(GTK_CONTAINER(wWindow), 10);
}

void setLabel(GtkWidget *pBox) {
    GtkWidget
            *lbl_server = gtk_label_new("STUN Server:"),
            *lbl_nat_type = gtk_label_new("Nat Type:"),
            *lbl_addr_local = gtk_label_new("Local Addr:"),
            *lbl_addr_public = gtk_label_new("Public Addr:");

    gtk_widget_set_halign(lbl_server, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_nat_type, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_addr_local, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_addr_public, GTK_ALIGN_START);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkBox *box = GTK_BOX(vbox);

    gtk_box_pack_start(box, lbl_server, TRUE, TRUE, 0);
    gtk_box_pack_start(box, lbl_nat_type, TRUE, TRUE, 0);
    gtk_box_pack_start(box, lbl_addr_local, TRUE, TRUE, 0);
    gtk_box_pack_start(box, lbl_addr_public, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pBox), vbox, TRUE, TRUE, 10);
}

void setEditBox(GtkWidget *pBox) {
    GtkWidget
            *edit_server = gtk_combo_box_text_new_with_entry(),
            *edit_nat_type = gtk_entry_new(),
            *edit_addr_local = gtk_entry_new(),
            *edit_addr_public = gtk_entry_new();

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.xten.com");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.ekiga.net");

    gtk_widget_set_sensitive(edit_nat_type, FALSE); // disable widget
    gtk_widget_set_sensitive(edit_addr_local, FALSE); // disable widget
    gtk_widget_set_sensitive(edit_addr_public, FALSE); // disable widget

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkBox *box = GTK_BOX(vbox);

    gtk_box_pack_start(box, edit_server, TRUE, TRUE, 10);
    gtk_box_pack_start(box, edit_nat_type, TRUE, TRUE, 10);
    gtk_box_pack_start(box, edit_addr_local, TRUE, TRUE, 10);
    gtk_box_pack_start(box, edit_addr_public, TRUE, TRUE, 10);

    gtk_box_pack_start(GTK_BOX(pBox), vbox, TRUE, TRUE, 10);
}

void setPort(GtkWidget *pBox) {
    GtkWidget *port = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(port), "3478");

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkBox *box = GTK_BOX(vbox);

    gtk_box_pack_start(box, port, FALSE, FALSE, 10);

    gtk_box_pack_start(GTK_BOX(pBox), vbox, TRUE, TRUE, 10);
}

void setButton(GtkWidget *pBox) {
    GtkWidget *button = gtk_button_new_with_label("Test");
    gtk_widget_set_valign(button, GTK_ALIGN_END);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);

    g_signal_connect(button, "clicked",
                     G_CALLBACK(test), NULL);

    gtk_box_pack_end(GTK_BOX(pBox), button, TRUE, TRUE, 10);
}

GtkWidget *getWindow() {
    return gtk_window_new(GTK_WINDOW_TOPLEVEL);
}

void initLayout(GtkWidget *wWindow) {
    setWindow(wWindow);

    GtkWidget
            *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0),
            *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    setLabel(hbox);
    setEditBox(hbox);
    setPort(hbox);
    setButton(vbox);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);

    gtk_container_add(GTK_CONTAINER(wWindow), vbox);
}
