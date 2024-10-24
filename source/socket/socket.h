//
// Created by Max Sky on 2021/12/16.
//

#include "../constants/msgstruct.h"

class socket {

public:
    static int send(gchar *host, const gchar *port);

    static int receive();

private:
    static char *generate_transaction_id(size_t length);

    static char *serialize_stun_header(StunMessageHeader *header);

    static int build(gchar *host, const gchar *port, StunMessageHeader *header);

    static void parse(const gchar *response, gssize size);

    static void close();
};
