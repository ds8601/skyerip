#!/bin/sh
g++ main.cpp log.cpp preset.cpp helper.cpp -O2 -march=native -Wall -Wextra -fno-common -o a.out
#strip --strip-all a.out
