#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "structs.h"
#include "util.h"

int main(int argc, char **argv) {
    int socket_fd;
    struct sockaddr_in addr;
    pthread_t recvr, sendr;
    t_args args;

    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err("Socket error");

    if((connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr))) < 0)
        err("Connect error");

    args.fd = socket_fd;

    pthread_create(&recvr, NULL, receiver, (void*)&args);
    pthread_create(&sendr, NULL, sender, (void*)&args);

    pthread_join(recvr, NULL);
    pthread_join(sendr, NULL);

    return 0;
}