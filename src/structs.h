#ifndef __STRUCTS_H__
#define __STRUCTS_H__
#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>

typedef struct{
    pthread_t receiver;
    pthread_t sender;
} t_conn;

typedef struct{
    int fd, id;
} client;

typedef struct{
    int fd;
} t_args;

typedef struct{
  uint8_t *buffer;
  ssize_t size;
} message;

#endif