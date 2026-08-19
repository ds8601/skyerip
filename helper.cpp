#include <iostream>
#include <unordered_map>
#include "helper.h"
#include "log.hpp"
using namespace std;
string srcf="helper.cpp ";
//FIXME: unknown crashes when ... spaces are involved? or C99 strings? __DATE__ crashes this (my best guess would be a "read out of bounds" bug due to __DATE__ possibly not containing \0)
vector<long unsigned int> locate_char(string in, char find){
	vector<long unsigned int> out;
	for(long unsigned int i=0; i<in.size();i++){
		if(in[i]==find){
			out.push_back(i);
			send_to_log_v2({srcf, "(locate_char): found ", {find}, " at index", uint_to_string(i), "\n"});
		}
	}
	return out;
}

unsigned int uintInSubstr_to_uint(string substr){
	unsigned int dec=1;
	unsigned int output=0;
	if(substr.size()==0) send_to_log_v2({srcf, "(uintInSubstr_to_uint): got a substring with a size of 0\n"});
	while(substr.size()>0){
		output = output + ((substr.back() - '0')*dec);
		dec = dec * 10;
		substr.pop_back();
	}
	return output;
}

int intInSubstr_to_int(string substr){
	int dec=1;
	int out=0;
	if(substr.size()==0 || (substr.size()==1 && substr[0]=='-')) send_to_log_v2({srcf, "(intInSubstr_to_int): got an empty string or just MINUS.\n"});
	while(substr.size()>1){
		out = out + ((substr.back() - '0')*dec);
		dec = dec*10;
		substr.pop_back();
	}
	if (substr[0]=='-') out = out * (-1);
	else out = out + ((substr.back() - '0')*dec);
	return out;
}
//this could be a generic "string with dots to vector of ints" thing, since a char can be an int.
int check_definition_version(string version, int usage, uint *maj, uint *min, uint *pat, char *ind){ //FIXME: I really don't want to copy the contents of the string to- nvm
	string funcn= "(check_definition_version): ";
	vector<long unsigned int> dots = locate_char(version, '.');
	if(dots.size() != 2){
		switch (usage){
			case 1:{
				       send_to_log_v2({srcf, funcn, "more than 2 dots in encoder definition file version.\n"});
				       break;
			       }
			default:{
					send_to_log_v2({srcf, funcn, "more than 2 dots in encoder preset file version.\n"});
					break;
				}
		}
		return -1;
	}
	*ind = version.back();
	version.pop_back();
	*maj = uintInSubstr_to_uint(version.substr(0,dots[0]));
	*min = uintInSubstr_to_uint(version.substr(dots[0]+1,dots[1]-dots[0]-1));
	*pat = uintInSubstr_to_uint(version.substr(dots[1]+1,version.size()-1-dots[1]));
	return 0;
}




//FALLBACK:
//Warning: will break unless locale is set to English, kurwa.
string date_to_isodate(string date){
	long unsigned int spaces[2];
	int c=0;
	for(long unsigned int i = 0; i<date.size(); i++){
		if(date[i] == ' '){
			spaces[c] = i;
			c++;
		}
	}
	string month = date.substr(0,3);
	string day = date.substr(spaces[0]+1,spaces[1]-spaces[0]-1);
	string year = date.substr(spaces[1]+1,date.size()-spaces[1]-1);
	//I'm too tired to debug wtf is crashing with the unordered map, at this point it genuinely might be easier to look into wether the compiler has some switch to define a variable at compile time.
	string monthfinal;
	if(month == "Jan") monthfinal = "01";
	else if(month == "Feb") monthfinal = "02";
	else if(month == "Mar") monthfinal = "03";
	else if(month == "Apr") monthfinal = "04";
	else if(month == "May") monthfinal = "05";
	else if(month == "Jun") monthfinal = "06";
	else if(month == "Jul") monthfinal = "07";
	else if(month == "Aug") monthfinal = "08";
	else if(month == "Sep") monthfinal = "09";
	else if(month == "Oct") monthfinal = "10";
	else if(month == "Nov") monthfinal = "11";
	else monthfinal="12";
	string output = year + "-" + monthfinal + "-" + day;
	return output;
}
