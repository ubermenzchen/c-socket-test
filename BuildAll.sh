#!/bin/bash

SRC_DIR=src
BIN_DIR=bin

gcc -Wall -pedantic -o $BIN_DIR/server $SRC_DIR/server.c $SRC_DIR/util.c -lpthread
gcc -Wall -pedantic -o $BIN_DIR/client $SRC_DIR/client.c $SRC_DIR/util.c -lpthread
