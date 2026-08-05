#!/bin/sh
if command -v clang++ > /dev/null
then #clang usually gives smaller binary files. time executed remains to be tested
	clang++ main.cpp log.cpp preset.cpp helper.cpp -O2 -march=native -Wall -Wextra -fno-common -o a.out
else
	g++ main.cpp log.cpp preset.cpp helper.cpp -O2 -march=native -Wall -Wextra -fno-common -o a.out
fi
#strip --strip-all a.out
#g++ -ggdb main.cpp log.cpp preset.cpp helper.cpp -o a.out
