#include <iostream>
#include <fstream>
#include "log.hpp"
#include "defines.h"
#include "helper.h"
using namespace std;
fstream ps;
//we really need a warning type switch in g++ that will warn against unused library inclusion.
//filename and type, futureproofing, because a muxer will need a separate kind of preset format.
int parse_def(string path, vector<sw> *switches, string *input, string *output){
	fstream p;
	send_to_log_v2({"preset.cpp (parse_def): opening ", path, "\n"});
	p.open(path, fstream::in);
	if(!p.is_open()){
		send_to_log_v2({"preset.cpp (parse_def): opening ", path, " failed\n"});
		return -1;
	}
	string *sig = new string;
	getline(p, *sig);
	if(*sig != "sred"){
		send_to_log_v2({"preset.cpp (parse_def): ", path, " is NOT a skyerip encoder definition file. returning -1.\n"});
		return -1;
	}
	delete sig;
	string *version = new string;
	getline(p, *version);
	uint *maj = new uint, *min = new uint, *pat = new uint;
	char *ind = new char;
	int vcheck = check_definition_version(*version, 0, maj, min, pat, ind);
	if(vcheck == -1){
		send_to_log_v2({"preset.cpp (parse_def): check_definition_version returned -1, no point in this might not be the file the program asks for. \n"});
		return -1;
	}
	send_to_log_v2({"preset.cpp (parse_def): definition file version ", uint_to_string(*maj), {'.'}, uint_to_string(*min), {'.'}, uint_to_string(*pat), {ind}, "\n"});
	if(*maj != CURENCD_MAJ || *min != CURENCD_MIN || *pat != CURENCD_PAT || *ind != CURENCD_IND){
		send_to_log_v2({"preset.cpp (parse_def): definition version doesn't match. UNIMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-08-02 21:05 CEST\n"});
		return -2;
	}
	delete version; //-Wall and -Wextra complained about stacked deletes, so I guess we're stuck with 5 lines of just delete - skye, 2026-08-03 17:25 CEST
	delete maj;
	delete min;
	delete pat;
	delete ind;
	getline(p, vencpath);
	send_to_log_v2({"preset.cpp (parse_def): set vencpath to: \"", vencpath, "\"\n"});
	getline(p, *input);
	send_to_log_v2({"preset.cpp (parse_def): set input variable to: \"", *input, "\"\n"});
	getline(p, *output);
	send_to_log_v2({"preset.cpp (parse_def): set output variable to: \"", *output, "\"\n"});
	string swdef; // the one string that I can't dynamically allocate due to ... *swdef[i] comparison against a char not working.
	while (swdef != "EOF"){ //begin spaget. FIXME: this shit is definitely unoptimized.
		getline(p, swdef);
		send_to_log_v2({"preset.cpp (parse_def): read \"", swdef, "\" from file to *swdef\n"});
		string *name = new string;
		//switches->push_back(sw());
		long unsigned int i=0;
		while(swdef[i]!=',' && i<swdef.size()){
			i++;
		}
		*name = swdef.substr(0,i-1);
		send_to_log_v2({"preset.cpp (parse_def): set *name to \"", *name, "\"\n"});
		swdef = swdef.substr(i+1, swdef.size());
		i=0; //multi is a bool, first character is the value.
		bool *multi = new bool;
		*multi = uintInSubstr_to_uint(swdef.substr(0,1));
		swdef = swdef.substr(2,swdef.size());
		while(swdef[i]!=',' && i<swdef.size());
		//TODO: parse the individual switch deifinitions and do switches.push_back({name,multi,multi_vars,(valid_type),min,max,den)
	}
	return 0;
}

int parse_preset(string fn, int t, vector<sw> *switches){
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
	*res = parse_def(*path, switches, input, output);
	if(*res == -1){
		send_to_log_v2({"preset.cpp (parse_preset): parse_def() exited with error code corresponding to unrecognised header.\n"});
		return 1;
	}
	delete path;
	delete res;
	//TODO: COMPLETE PARSER
	return true;
}
