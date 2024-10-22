#include "argparser.h"

void show_help() {
    puts("usage: ncp [--receive | -r] [--send | -s] [--file | -f file] [--rename | -R filename] [host] [port]");
}

void parse_error(char *cause) {
    fprintf(stderr, "%s", cause);
    show_help();
    exit(1);
}

void parse_args(int argc, char **argv, struct ncp_info *info) {
    const struct option longopts[] = {
            //{    *name,           has_arg, *flag, val },
            {"receive", no_argument,       NULL, 'r'},
            {"send",    no_argument,       NULL, 's'},
            {"file",    required_argument, NULL, 'f'},
            {"rename",  required_argument, NULL, 'R'},
            {0, 0, 0,                            0}, // termination
    };
    int opt;
    int longindex;

    while ((opt = getopt_long(argc, argv, "rsf:R:", longopts, &longindex)) != -1) {
        switch (opt) {
            case 'r':
                if (info->mode != NCP_MODE_NONE) parse_error("モードを同時に指定しないでください\n");
                info->mode = NCP_MODE_RECV;
                break;
            case 's':
                if (info->mode != NCP_MODE_NONE) parse_error("モードを同時に指定しないでください\n");
                info->mode = NCP_MODE_SEND;
                break;
            case 'f': {
                char *filepath = optarg;
                if (access(filepath, F_OK) != 0) {
                    parse_error("file not found\n");
                    exit(1);
                }
                info->filepath = optarg;
                break;
            }
            case 'R':
                info->filename = optarg;
                break;
            default:
                show_help();
        }
    }
    if (info->mode == NCP_MODE_NONE) {
        parse_error("No mode specified\n");
    }
    char *hostname = NULL;
    char *port_str = NULL;
    char **nonoption_args[] = {
            &hostname, &port_str
    };
    if (argc - optind != sizeof(nonoption_args) / sizeof(nonoption_args[0])) {
        parse_error("Invalid argument\n");
    }
    for (int index = optind; index < argc; index++) {
        *(nonoption_args[index - optind]) = argv[index];
    }

    set_host_port(hostname, port_str, &info->addr);
    if (info->filename == NULL) {
        info->filename = basename(info->filepath);
    }
}
