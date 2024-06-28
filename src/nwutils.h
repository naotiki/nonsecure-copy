//
// Created by root on 6/28/24.
//

#ifndef NONSECURE_COPY_NWUTILS_H
#define NONSECURE_COPY_NWUTILS_H
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

void set_host_port(char *hostname, char *port, struct sockaddr_in *addr);

#endif //NONSECURE_COPY_NWUTILS_H
