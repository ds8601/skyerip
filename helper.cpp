#include <iostream>
#include "helper.h"
#include "log.hpp"
using namespace std;

bool locate_dots(string ver, unsigned int num, unsigned int *dots){
	unsigned int c=0;
	for (long unsigned int i=0; i<ver.length(); i++){
		if(ver[i] == '.'){
			if(c<num){
			dots[c]=i;
			c++;}
			else return false;
		}
	}
	return true;
}
unsigned int uintInSubstr_to_uint(string substr){
	unsigned int dec=1;
	unsigned int output=0;
	if(substr.size()==0){
		send_to_log_v2({"helper.cpp (uintInSubstr_to_uint): got a substring with a size of 0\n"});
	}
	while(substr.size()>0){
		output = output + ((substr.back() - '0')*dec);
		dec = dec * 10;
		substr.pop_back();
	}
	return output;
}
int check_definition_version(string version, int usage, uint *maj, uint *min, uint *pat, char *ind){ //FIXME: I really don't want to copy the contents of the string to- nvm
	unsigned int *dots = new unsigned int[2];
	if(!locate_dots(version, 2, dots)){
		if(usage == 1){
			send_to_log_v2({"helper.cpp (check_definition_version): more than 2 dots in encoder definition file version. Time for a fsck?\n"});
			}
		else send_to_log_v2({"helper.cpp (check_definition_version): more than 2 dots in encoder preset file version. wrong file?\n"});
		return -1;}
	*ind = version.back();
	version.pop_back();
	*maj = uintInSubstr_to_uint(version.substr(0,dots[0]));
	*min = uintInSubstr_to_uint(version.substr(dots[0]+1,dots[1]-dots[0]-1));
	*pat = uintInSubstr_to_uint(version.substr(dots[1]+1,version.size()-1-dots[1]));
	delete[] dots;
	return 0;
}
