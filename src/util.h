#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdint.h>
#include <sys/types.h>
#include "structs.h"

#define unpack(buf, n) n = buf[3] << 24 | buf[2] << 16 | buf[1] << 8 | buf[0];

#define pack(n, buf) buf[3] = (n & 0xFF000000) >> 24; \
                     buf[2] = (n & 0x00FF0000) >> 16; \
                     buf[1] = (n & 0x0000FF00) >> 8; \
                     buf[0] = n & 0x000000FF;

message* get_stdin_input(void);
void err(char *msg);
void writeAll(int fd, uint8_t *buf, ssize_t size);
void readAll(int fd, uint8_t *buf, ssize_t size);
void* receiver(void *args);
void* sender(void *args);

#endif