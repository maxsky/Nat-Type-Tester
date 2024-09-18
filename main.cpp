#include <gtk/gtk.h>
#include <stdio.h>
#include <malloc/malloc.h>
#include "source/window/layout.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    layout::init();

    gtk_main();

    return 0;
}
