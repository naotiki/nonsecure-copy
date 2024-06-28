//
// Created by root on 6/28/24.
//

#ifndef NONSECURE_COPY_ARGPARSER_H
#define NONSECURE_COPY_ARGPARSER_H
#include "nwutils.h"

#include <stdlib.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>

#define NCP_MODE_NONE 0
#define NCP_MODE_SEND 1
#define NCP_MODE_RECV 2


struct ncp_info {
    int mode;
    struct sockaddr_in addr;

};

void parse_args(int argc, char *argv[], struct ncp_info *info);

#endif //NONSECURE_COPY_ARGPARSER_H
