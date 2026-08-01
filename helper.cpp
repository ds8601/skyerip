#include <iostream>
#include "helper.h"

using namespace std;

bool locate_dots(string ver, unsigned int num, unsigned int *dots){
	unsigned int c=0;
	for (int i=0; i<ver.length(); i++){
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
	while(substr.size()!=0){
		output = output + ((substr.back() - '0')*dec);
		dec = dec * 10;
		substr.pop_back();
	}
	return output;
}
