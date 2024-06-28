#include <stdio.h>
#include <arpa/inet.h>
#include "argparser.h"
#include "ncpproto.h"
#include <unistd.h>

void cmd_receive(struct ncp_info info);

void cmd_send(struct ncp_info info);

int main(int argc, char *argv[]) {

    struct ncp_header h = create_header("j22413@192.168.2.3", "test.c", 0x4545454);
    FILE *test = fopen("./test.bin", "r");
    int fd = fileno(test);
/*
    send_header(fd, h);*/
    struct ncp_header hout = {0};
    recv_header(fd,&hout);
    send_header(1,hout);
    fclose(test);


    return 0;
    struct ncp_info info = {0};

    parse_args(argc, argv, &info);

    printf("%s\n", inet_ntoa(info.addr.sin_addr));

    switch (info.mode) {
        case NCP_MODE_RECV:
            cmd_receive(info);
            break;
        case NCP_MODE_SEND:
            cmd_send(info);
            break;
        default:
            fprintf(stderr, "ERR: NO MODE");
            break;
    }

    return 0;
}

void cmd_receive(struct ncp_info info) {
    puts("MODE: Receive");
    info.addr.sin_family = AF_INET;
    int sock_serv = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_serv < 0) {
        perror("socket");
        exit(1);
    }
    if (bind(sock_serv, (struct sockaddr *) &info, sizeof(info)) == -1) {
        perror("bind");
        exit(1);
    }
    listen(sock_serv, 1);
    int sock = accept(sock_serv, NULL, NULL);
    close(sock_serv);


}

void cmd_send(struct ncp_info info) {
    puts("MODE: Send");
}
