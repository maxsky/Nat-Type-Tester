#include <gtk/gtk.h>
#include "source/layout.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    layout::init();

    gtk_main();

    return 0;
}



