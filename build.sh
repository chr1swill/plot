#!/bin/bash
set -xe;

CC=tcc
SRC=./bin/main
DST=./src/main.c
CFLAGS="-Wall -Wextra -pedantic -ggdb"

${CC} ${CFLAGS} -o ${SRC} ${DST}
