//
// Created by Max Sky on 2021/12/16.
//

#include "../constants/msgstruct.h"
#include <gtk/gtk.h>
#include <random>

class socket {

public:
    static int send(gchar *host, const gchar *port);

    static int receive();

private:
    static void generate_transaction_id(char transaction_id[STUN_HEADER_TRANSACTION_ID_LENGTH]);

    static gboolean resolve_hostname_to_ip(gchar **host);

    static int build(gchar *host, const gchar *port, StunMessageHeader *header);

    static void parse(const gchar *response, gssize size);

    static void close();
};
