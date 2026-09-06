//checks.cpp
// (c) 2026 Skye Wierzchowska
#include <iostream>
#include <sys/stat.h>
using namespace std;

int chkFileExists(string fn){
	struct stat exists;
	if(stat(fn.c_str(), &exists) == 0){
		return 0;
	}
	else{
		return 1;
	}
}
