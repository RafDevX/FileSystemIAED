#!/usr/bin/sh

gcc -ansi -pedantic -Wall -Wextra src/*.c -o filesystem.debug -O0 -g
valgrind --leak-check=yes ./filesystem.debug