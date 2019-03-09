#include "util.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

message* get_stdin_input(void) {
  message *msg = (message*)malloc(sizeof(message));
  uint8_t *buffer = NULL;
  uint8_t t;
  ssize_t size = 0;
  while(t != '\n') {
    read(STDIN_FILENO, &t, 1);
    buffer = realloc(buffer, ++size);
    buffer[size-1] = t;
  }
  buffer[size-1] = '\n';
  msg->buffer = buffer;
  msg->size = size;
  return msg;
}

void err(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

void writeAll(int fd, uint8_t *buf, ssize_t size) {
    ssize_t i = 0;
    while(i < size) {
        i = write(fd, &buf[i], size);
        if(i < 0)
            err("Write error");
    }
}

void readAll(int fd, uint8_t *buf, ssize_t size) {
    ssize_t i = 0;
    while(i < size) {
        i = read(fd, &buf[i], size);
        if(i < 0)
            err("Write error");
    }
}

void* receiver(void *args) {
    client *cli = (client*)args;
    uint8_t *buffer;

    while(1) {
        uint8_t bsize[4];
        uint32_t size;
        if(read(cli->fd, bsize, 4) < 0)
            err("Write error");
        unpack(bsize, size);
        buffer = calloc(sizeof(uint8_t), size);
        readAll(cli->fd, buffer, size);
        printf("%s", (char*)buffer);
        free(buffer);
    }
}

void* sender(void *args) {
    client *cli = (client*)args;
    message *msg;

    while(1) {
        msg = get_stdin_input();
        uint8_t size[4];
        pack(msg->size, size);
        if(write(cli->fd, size, 4) < 0)
            err("Write error");
        writeAll(cli->fd, msg->buffer, msg->size);
        free(msg->buffer);
        free(msg);
    }
}