//
// Created by Max Sky on 2021/9/15.
//

#include "component.h"

GtkWidget *component::server = nullptr;
GtkWidget *component::port = nullptr;
GtkWidget *component::nat_type = nullptr;
GtkWidget *component::addr_local = nullptr;
GtkWidget *component::addr_public = nullptr;

void component::setWindow(GtkWidget *wWindow) {
    GtkWindow *window = GTK_WINDOW(wWindow);

    gtk_window_set_title(window, "Nat Type Tester");
    gtk_window_set_resizable(window, false);
    // gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DROPDOWN_MENU);
    gtk_window_unmaximize(window);
    gtk_window_set_position(window, GTK_WIN_POS_CENTER);
    gtk_window_activate_default(window);
    gtk_window_activate_focus(window);

    gtk_container_set_border_width(GTK_CONTAINER(wWindow), 10);
}

GtkWidget *component::getWindow() {
    return gtk_window_new(GTK_WINDOW_TOPLEVEL);
}

void component::setLabel(GtkWidget *pBox) {
    auto
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

    gtk_box_pack_start(box, lbl_server, true, true, 0);
    gtk_box_pack_start(box, lbl_nat_type, true, true, 0);
    gtk_box_pack_start(box, lbl_addr_local, true, true, 0);
    gtk_box_pack_start(box, lbl_addr_public, true, true, 0);

    gtk_box_pack_start(GTK_BOX(pBox), vbox, true, true, 10);
}

void component::setEditBox(GtkWidget *pBox) {
    auto
            *edit_server = gtk_combo_box_text_new_with_entry(),
            *edit_nat_type = gtk_entry_new(),
            *edit_addr_local = gtk_entry_new(),
            *edit_addr_public = gtk_entry_new();

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "global.stun.twilio.com");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.3cx.com");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.cloudflare.com");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.ekiga.net");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.l.google.com");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.prod.fastly.net");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.voipbuster.com");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.voipstunt.com");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(edit_server), "stun.xten.com");


    g_signal_connect(edit_server, "changed", G_CALLBACK(on_server_changed), NULL);

    gtk_widget_set_name(edit_server, "edit_server");
    gtk_widget_set_name(edit_nat_type, "edit_nat_type");
    gtk_widget_set_name(edit_addr_local, "edit_addr_local");
    gtk_widget_set_name(edit_addr_public, "edit_addr_public");

    gtk_widget_set_sensitive(edit_nat_type, false); // disable component
    gtk_widget_set_sensitive(edit_addr_local, false); // disable component
    gtk_widget_set_sensitive(edit_addr_public, false); // disable component

    auto *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    auto *box = GTK_BOX(vbox);

    gtk_box_pack_start(box, edit_server, true, true, 10);
    gtk_box_pack_start(box, edit_nat_type, true, true, 10);
    gtk_box_pack_start(box, edit_addr_local, true, true, 10);
    gtk_box_pack_start(box, edit_addr_public, true, true, 10);

    server = edit_server;
    nat_type = edit_nat_type;
    addr_local = edit_addr_local;
    addr_public = edit_addr_public;

    gtk_box_pack_start(GTK_BOX(pBox), vbox, true, true, 10);
}

void component::setPort(GtkWidget *pBox) {
    GtkWidget *editPort = gtk_entry_new();

    gtk_widget_set_name(editPort, "edit_port");
    gtk_entry_set_text(GTK_ENTRY(editPort), "3478");
    gtk_entry_set_input_purpose(GTK_ENTRY(editPort), GTK_INPUT_PURPOSE_NUMBER);
    auto *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkBox *box = GTK_BOX(vbox);

    gtk_box_pack_start(box, editPort, false, false, 10);

    port = editPort;

    gtk_box_pack_start(GTK_BOX(pBox), vbox, true, true, 10);
}

void component::setButton(GtkWidget *pBox, GCallback func) {
    auto *button = gtk_button_new_with_label("Test");
    gtk_widget_set_valign(button, GTK_ALIGN_END);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);

    g_signal_connect(button, "clicked", func, NULL);

    gtk_box_pack_end(GTK_BOX(pBox), button, true, true, 10);
}

void component::on_server_changed(GtkComboBox *widget) {
    const gchar *selected_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));

    const gchar *google = "stun.l.google.com";

    if (g_strcmp0(selected_text, google) == 0) {
        gtk_entry_set_text(GTK_ENTRY(port), "19302");
    } else {
        gtk_entry_set_text(GTK_ENTRY(port), "3478");
    }

    g_free((gchar *) selected_text);
}
