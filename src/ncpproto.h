//
// Created by root on 6/28/24.
//

#ifndef NONSECURE_COPY_NCPPROTO_H
#define NONSECURE_COPY_NCPPROTO_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <pwd.h>

enum NCPProtocol{
    ACC,
    BYE
};

struct ncp_header {
    size_t from_identifier_size;
    char *from_identifier;
    size_t filename_size;
    char *filename;
    off_t content_size;
};

struct ncp_header create_header(char *filename, off_t content_size);

void send_header(int fd, struct ncp_header header);

void recv_header(int fd, struct ncp_header *header);

void free_header(struct ncp_header header);

#endif //NONSECURE_COPY_NCPPROTO_H
