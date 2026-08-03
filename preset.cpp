#include <iostream>
#include <fstream>
#include "log.hpp"
#include "defines.h"
#include "helper.h"
using namespace std;
fstream ps;
//we really need a warning type switch in g++ that will warn against unused library inclusion.
//filename and type, futureproofing, because a muxer will need a separate kind of preset format.
int parse_def(string path, string *input, string *output, int t){ //correct according to spec version 0.0.1-draft1 from 2026-08-01, completed 2026-08-03 21:33 CEST - skye
	string funcname = "preset.cpp (parse_def): ";
	fstream p;
	send_to_log_v2({funcname, "opening ", path, "\n"});
	p.open(path, fstream::in);
	if(!p.is_open()){
		send_to_log_v2({funcname, "opening ", path, " failed\n"});
		return -1;
	}
	string *sig = new string;
	getline(p, *sig);
	if(*sig != "sred"){
		send_to_log_v2({funcname, path, " is NOT a skyerip encoder definition file. returning -1.\n"});
		return -1;
	}
	delete sig;
	string *version = new string;
	getline(p, *version);
	uint *maj = new uint, *min = new uint, *pat = new uint;
	char *ind = new char;
	int vcheck = check_definition_version(*version, 0, maj, min, pat, ind);
	if(vcheck == -1){
		send_to_log_v2({funcname, "check_definition_version returned -1, no point in this might not be the file the program asks for. \n"});
		return -1;
	}
	send_to_log_v2({funcname, "definition file version ", uint_to_string(*maj), {'.'}, uint_to_string(*min), {'.'}, uint_to_string(*pat), {ind}, "\n"});
	if(*maj != CURENCD_MAJ || *min != CURENCD_MIN || *pat != CURENCD_PAT || *ind != CURENCD_IND){
		send_to_log_v2({funcname, "definition version doesn't match. UNIMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-08-02 21:05 CEST\n"});
		return -2;
	}
	delete version; //-Wall and -Wextra complained about stacked deletes, so I guess we're stuck with 5 lines of just delete - skye, 2026-08-03 17:25 CEST
	delete maj;
	delete min;
	delete pat;
	delete ind;
	getline(p, vencpath);
	send_to_log_v2({funcname, "set vencpath to: \"", vencpath, "\"\n"});
	getline(p, *input);
	send_to_log_v2({funcname, "set input variable to: \"", *input, "\"\n"});
	getline(p, *output);
	send_to_log_v2({funcname, "set output variable to: \"", *output, "\"\n"});
	//FIXME:possible cleanups: a lot of this depends on finding one character, possibly worth adapting locate_dots() to be find_char(unsigned int *table[], string substr, char *find);
	string swdef; // the one string that I can't dynamically allocate due to ... *swdef[i] comparison against a char not working.
	while (getline(p, swdef)){ //begin spaget. FIXME: this shit is definitely unoptimized.
		if(swdef == "EOF") return 0; //workaround stupidity.
		send_to_log_v2({funcname, "read \"", swdef, "\" from file to *swdef\n"});
		string *name = new string;
		long unsigned int i=0;
		while(swdef[i]!=',' && i<swdef.size()) i++;
		*name = swdef.substr(0,i);
		send_to_log_v2({funcname, "set *name to \"", *name, "\"\n"});
		i++; //multi is a bool, first character is the value.
		bool *multi = new bool;
		*multi = uintInSubstr_to_uint(swdef.substr(i,1));
		send_to_log_v2({funcname, "set *multi to \"", uint_to_string(*multi), "\"\n"});
		i+=3;
		long unsigned int j=0;
		string *multi_var = new string;
		*multi_var = ""; //just in case I forget NOT to check this IF type !=3 || type !=2
		if(*multi == true){
			while(swdef[i+j]!='"' && i+j<swdef.size()) j++;
			*multi_var = swdef.substr(i,j);
			i=i+j+2; //this is accounting for the " symbol.
			send_to_log_v2({funcname, "set *multi_var to \"", *multi_var, "\"\n"});
		}
		unsigned int type = uintInSubstr_to_uint(swdef.substr(i,1));
		send_to_log_v2({funcname, "set type to ", uint_to_string(type), "\n"});
		if(type == 3 || type == 2){
			send_to_log_v2({funcname, "type correpsonds to magic numbers of MULTIVAR or STRING, no more variables left to populate from this line of the definition, pushing back what's read to vector and continuing to next line: ", *name, ",", uint_to_string(*multi), ",", *multi_var, ",0,0,", uint_to_string(type), ",-1,-1,-1,-1,-1", "\n"});
			if(t==0)vencsw.push_back({*name, *multi, *multi_var,0,0,type, -1, -1, -1, -1, -1});
			else aencsw.push_back({*name, *multi, *multi_var, 0, 0, type, -1, -1, -1, -1 ,-1});
			delete name;
			delete multi;
			delete multi_var;
			continue;
		}
		i+=2;
		int *min = new int;
		//FIXME: intInSubstr_to_int() needed
		j=0;
		while(swdef[i+j]!=',' && i+j<swdef.size()) j++;
		*min = uintInSubstr_to_uint(swdef.substr(i,j));
		send_to_log_v2({funcname, "set *min to ", uint_to_string(*min), "\n"}); //FIXME: int_to_string needed. badly.
		i=i+j+1;
		int *max = new int;
		j=0;
		while(swdef[i+j]!=',' && i+j<swdef.size()) j++;
		*max = uintInSubstr_to_uint(swdef.substr(i,j));
		send_to_log_v2({funcname, "set *max to ", uint_to_string(*max), "\n"});
		i=i+j+1;
		int *den = new int;
		//FIXME: this should be unneccesary, I'm just too tired to fix my off-by-one math errors
		j=0;
		while(i+j < swdef.size()) j++;
		*den = uintInSubstr_to_uint(swdef.substr(i,j));
		send_to_log_v2({funcname, "set *den to ", uint_to_string(*den), ". Type corresponds to magic numbers of NOTHING or INT/BOOL/FLOATINGPOINT, no more variables (according to spec) to populate from this line of the definition, pushing back what's read to vector and continuing to next line: ", *name, ",", uint_to_string(*multi), ",", *multi_var, ",0,0,", uint_to_string(type), ",", uint_to_string(*min), ",", uint_to_string(*max), ",", uint_to_string(*den), ",-1,-1\n"});
		if(t==0)vencsw.push_back({*name, *multi, *multi_var, 0, 0, type, *min, *max, *den, -1, -1});
		else aencsw.push_back({*name, *multi, *multi_var, 0, 0, type, *min, *max, *den, -1, -1});
		//this is done to shut up -Wall -Wextra
		delete name;
		delete multi;
		delete multi_var;
		delete min;
		delete max;
		delete den;
	}
	return 0;
}

int parse_preset(string fn, int t){
	send_to_log_v2({"preset.cpp (parse_preset): opening \"", fn, "\" type ", {char(t+'0')}, "\n"});
	ps.open(fn, fstream::in);
	if(!ps.is_open()){
		send_to_log_v2({"preset.cpp (parse_preset): ERROR: PRESET FILE DOESN'T EXIST. RETURNING -1\n"});
		return -1; //magic numbers, yay
	}
	string *preset = new string;
	getline(ps, *preset);
	if (*preset != "srps"){
		send_to_log_v2({"preset.cpp (parse_preset): not a recognised preset file format. returning -2"});
		return -2;
	}
	delete preset;
	string* pres_ver = new string;
	unsigned int *maj = new unsigned int, *min = new unsigned int, *pat = new unsigned int;
	char *ind = new char;
	getline(ps, *pres_ver);
	int vcheck = check_definition_version(*pres_ver, 1, maj, min, pat, ind);
	if(vcheck == -1){
		send_to_log_v2({"preset.cpp (parse_preset): preset file version header doesn't match defined standard (more than 2 dots), returning -2\n"});
		return -2;
	}
	send_to_log_v2({"preset.cpp (parse_preset): preset file version ", uint_to_string(*maj), {'.'}, uint_to_string(*min), {'.'}, uint_to_string(*pat), {ind}, "\n"});
	//check. TEMP BEHAVIOR: IF DOESN'T MATCH DEFINITIONS AT TOP OF SOURCE FILE, THEN EXIT WITH FALSE WHILE LOGGING "preset file version doesn't match. UNINMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST"
	if(*maj != CURENCP_MAJ || *min != CURENCP_MIN || *pat != CURENCP_PAT || *ind != CURENCP_IND){
		send_to_log_v2({"preset.cpp (parse_preset): preset file version doesn't match. UNIMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST\n"});
		return -3;
	}
	delete pres_ver; //again, longer less pretty form to shut -Wall -Wextra up.
	delete maj;
	delete min;
	delete pat;
	delete ind;
	string *path = new string, *input = new string, *output = new string;
	getline(ps, *path);
	send_to_log_v2({"preset.cpp (parse_preset): got encoder definition path: ", *path, "\n"});
	int *res = new int;
	*res = parse_def(*path, input, output, t);
	if(*res == -1){
		send_to_log_v2({"preset.cpp (parse_preset): parse_def() exited with error code corresponding to unrecognised header.\n"});
		return 1;
	}
	delete path;
	delete res;
	//TODO: COMPLETE PARSER
	return true;
}
