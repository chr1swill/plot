#!/bin/bash
set -xe;

CC=tcc
CFLAGS="-Wall -Wextra -pedantic -ggdb"
SRC=./bin/main
DST=./src/main.c

${CC} ${CFLAGS} -o ${SRC} ${DST}
