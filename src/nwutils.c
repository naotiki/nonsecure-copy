//
// Created by root on 6/28/24.
//

#include "nwutils.h"

void set_host_port(char *hostname, char *port, struct sockaddr_in *addr) {
    struct hostent *server_ent;
    server_ent = gethostbyname(hostname);
    memcpy((char *) &addr->sin_addr, server_ent->h_addr, server_ent->h_length);

    addr->sin_port = htons(atoi(port));
}
