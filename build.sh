#!/bin/bash
set -xe;

CC=tcc
CFLAGS="-Wall -Wextra -pedantic -ggdb"

${CC}  -o ./bin/main ./src/main.c;
