#!/bin/sh
if command -v clang++ > /dev/null
then #clang usually gives smaller binary files. time executed remains to be tested
	clang++ main.cpp log.cpp preset.cpp helper.cpp defines.cpp -Os -march=native -Wall -Wextra -fno-common -fno-unroll-loops -D __BUILDTIME__=\"`date "+%Y-%m-%d"`\" -o a.out
else
	g++ main.cpp log.cpp preset.cpp helper.cpp defines.cpp -O2 -march=native -Wall -Wextra -fno-common -D __BUILDTIME__=\"`date "+%Y-%m-%d"`\" -o a.out
fi
# Might be worth testing if this will solve my woes with __DATE__ being ..... DOGSHIT DEFINED FOR MAXIMUM ANNOYANCE. https://stackoverflow.com/questions/37389857/create-time-and-date-by-hand#comment62297316_37394077
#strip --strip-all a.out
#g++ -ggdb main.cpp log.cpp preset.cpp helper.cpp -o a.out
