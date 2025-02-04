#include "argparser.h"
#include "ncpproto.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>

void cmd_receive(struct ncp_info info);

void cmd_send(struct ncp_info info);

void print_bytes(unsigned long long bytes);

int main(int argc, char *argv[]) {

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
            break;
    }

    return 0;
}

#define BUF_SIZE 2048

void cmd_receive(struct ncp_info info) {
    puts("MODE: Receive");
    info.addr.sin_family = AF_INET;
    int sock_serv = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_serv < 0) {
        perror("socket");
        exit(1);
    }
    if (bind(sock_serv, (struct sockaddr *) &info.addr, sizeof(info.addr)) == -1) {
        perror("bind");
        exit(1);
    }
    listen(sock_serv, 1);
    int sock = accept(sock_serv, NULL, NULL);
    close(sock_serv);

    struct ncp_header hout = {0};
    recv_header(sock, &hout);
    printf("from     : %s\n", hout.from_identifier);
    printf("filename : %s\n", hout.filename);
    printf("filesize : ");
    print_bytes(hout.content_size);
    printf("\n");
    printf("continue? (y/N)");

    char c = '\0';
    scanf("%c", &c);
    if (c != 'y') {
        int p = BYE;
        puts("Exiting...");
        write(sock, &p, sizeof(int));
        close(sock);
        return;
    }

    int p = ACC;
    write(sock, &p, sizeof(int));
    char *filename = info.filename == NULL ? hout.filename : info.filename;
    printf("Copying to %s...\n", filename);
    FILE *out = fopen(filename, "w");
    if (!out) {
        perror("fopen");
        exit(1);
    }

    char buf[BUF_SIZE];
    ssize_t processed_bytes = 0;
    while (processed_bytes < hout.content_size ) {
        ssize_t bytes = recv(sock, buf, BUF_SIZE, 0);
        //content_sizeの長さにする
        if (processed_bytes+bytes > hout.content_size) {
            bytes = hout.content_size - processed_bytes;
        }
        fwrite(buf, 1, bytes, out);
        processed_bytes += bytes;
        printf("\r\033[2K");
        print_bytes(processed_bytes);
        printf(" / ");
        print_bytes(hout.content_size);
        printf(" (%ld%%)",processed_bytes*100/hout.content_size);
        bzero(buf, BUF_SIZE);
    }
    printf("\n");

    if (fclose(out) == EOF) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }
    free_header(hout);
    close(sock);
}

void cmd_send(struct ncp_info info) {
    puts("MODE: Send");
    FILE *fp = fopen(info.filepath, "r");
    int fd = fileno(fp);
    struct stat buf;
    fstat(fd, &buf);
    off_t size = buf.st_size;

    struct ncp_header h = create_header(info.filename, size);

    info.addr.sin_family = AF_INET;
    int soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (soc < 0) {
        perror("socket");
        exit(1);
    }
    if (connect(soc, (struct sockaddr *) &info.addr, sizeof(info.addr)) == -1) {
        perror("connect");
        exit(1);
    }
    send_header(soc, h);
    enum NCPProtocol p = 0;
    read(soc, &p, sizeof(int));
    switch (p) {
        case ACC:
            puts("Sending...");
            char data[BUF_SIZE] = {0};
            size_t readed;
            while ((readed = fread(data, sizeof(char), BUF_SIZE, fp)) > 0) {
                if (write(soc, data, readed) == -1) {
                    perror("[-] Error in sendung data");
                    exit(1);
                }
                bzero(data, BUF_SIZE);
            }
            int eof = EOF;
            write(soc, &eof, sizeof(int));

            break;
        case BYE:
            break;
    }
    puts("Closed!");
    close(soc);
    fclose(fp);
}

void print_bytes(unsigned long long int bytes) {
    double bytes_double = (double) bytes;
    int unit_index = 0;
    char units[][3] = {"B", "KB", "MB", "GB", "TB"};
    int all_units = sizeof(units) / sizeof(units[0]);

    while (bytes_double / 1024 > 1 && unit_index < all_units) {
        unit_index++;
        bytes_double /= 1024;
    }
    printf("%g %s", bytes_double, units[unit_index]);
}
