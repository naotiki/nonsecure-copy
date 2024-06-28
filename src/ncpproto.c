//
// Created by root on 6/28/24.
//

#include "ncpproto.h"


void send_header(int fd, struct ncp_header header) {
    write(fd, &(header.from_identifier_size), sizeof(size_t));
    write(fd, header.from_identifier, header.from_identifier_size);
    write(fd, &(header.filename_size), sizeof(size_t));
    write(fd, header.filename, header.filename_size);
    write(fd, &(header.content_size), sizeof(unsigned long long));
}

#include <stdio.h>

void recv_header(int fd, struct ncp_header *header) {
    read(fd, &(header->from_identifier_size), sizeof(size_t));
    printf("%zu\n", header->from_identifier_size);
    char *f= malloc(header->from_identifier_size * sizeof(char ));
    read(fd, f, header->from_identifier_size);
    printf("%s\n", f);
    free(f);
    read(fd, &(header->filename_size), sizeof(size_t));
    printf("%zu\n", header->filename_size);
    read(fd, header->filename, header->filename_size);
    read(fd, &(header->content_size), sizeof(unsigned long long));
    printf("%llu\n", header->content_size);
}

struct ncp_header create_header(char *from_id, char *filename, unsigned long long int content_size) {
    struct ncp_header result = {
            strlen(from_id),
            from_id,
            strlen(filename),
            filename,
            content_size
    };
    return result;
}


void free_header(struct ncp_header header) {
    free(header.from_identifier);
    free(header.filename);
}
