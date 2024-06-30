//
// Created by root on 6/28/24.
//

#include "ncpproto.h"


void send_header(int fd, struct ncp_header header) {
    write(fd, &(header.from_identifier_size), sizeof(size_t));
    write(fd, header.from_identifier, header.from_identifier_size);
    write(fd, &(header.filename_size), sizeof(size_t));
    write(fd, header.filename, header.filename_size);
    write(fd, &(header.content_size), sizeof(off_t));
}

void recv_header(int fd, struct ncp_header *header) {
    read(fd, &(header->from_identifier_size), sizeof(size_t));
    header->from_identifier = malloc(header->from_identifier_size * sizeof(char));
    read(fd, header->from_identifier, header->from_identifier_size);
    read(fd, &(header->filename_size), sizeof(size_t));
    header->filename = malloc(header->filename_size * sizeof(char));
    read(fd, header->filename, header->filename_size);
    read(fd, &(header->content_size), sizeof(off_t));
}

struct ncp_header create_header(char *filename, off_t content_size) {
    char *user_name = getpwuid(getuid())->pw_name;
    char *hostname = malloc((255 + 1) * sizeof(char));
    gethostname(hostname, (255 + 1) * sizeof(char));
    size_t len = (strlen(user_name) + 1 + strlen(hostname) + 1) * sizeof(char);
    char *from_id = malloc(len);
    bzero(from_id,len);
    strcat(from_id, user_name);
    strcat(from_id, "@");
    strcat(from_id, hostname);
    free(hostname);

    struct ncp_header result = {
            strlen(from_id) + 1,
            from_id,
            strlen(filename) + 1,
            filename,
            content_size
    };
    return result;
}


void free_header(struct ncp_header header) {
    free(header.from_identifier);
    free(header.filename);
}
