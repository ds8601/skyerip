#include <iostream>
#include <fstream>
#include <filesystem>
#include "log.hpp"
#include "defines.h"
#include "helper.h"
using namespace std;
unsigned int dots[2];
fstream ps;
//filename and type, futureproofing, because a muxer will need a separate kind of preset format.
int parse_def(string path, vector<sw> *switches){
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
	string *version = new string;
	getline(p, *version);
	uint *maj = new uint;
	uint *min = new uint;
	uint *pat = new uint;
	char *ind = new char;
	int vcheck = check_definition_version(*version, 0, maj, min, pat, ind);
	if(vcheck == -1){
		send_to_log_v2({"preset.cpp (parse_def): check_definition_version returned -1, no point in this might not be the file the program asks for. \n"});
		return -1;
	}
	send_to_log_v2({"preset.cpp (parse_def): definition file version ", uint_to_string(*maj), {'.'}, uint_to_string(*min), {'.'}, uint_to_string(*pat), {ind}, "\n"});
	//TODO: COMPLETE PARSER
	delete version, maj, min, pat, ind;
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
	unsigned int *maj = new unsigned int;
	unsigned int *min = new unsigned int;
	unsigned int *pat = new unsigned int;
	char *ind = new char;
	getline(ps, *pres_ver);
	int vcheck = check_definition_version(*pres_ver, 1, maj, min, pat, ind);
	send_to_log_v2({"preset.cpp (parse_preset): preset file version ", uint_to_string(*maj), {'.'}, uint_to_string(*min), {'.'}, uint_to_string(*pat), {ind}, "\n"});
	//check. TEMP BEHAVIOR: IF DOESN'T MATCH DEFINITIONS AT TOP OF SOURCE FILE, THEN EXIT WITH FALSE WHILE LOGGING "preset file version doesn't match. UNINMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST"
	if(*maj != CURENCP_MAJ || *min != CURENCP_MIN || *pat != CURENCP_PAT || *ind != CURENCP_IND){
		send_to_log_v2({"preset.cpp (parse_preset): preset file version doesn't match. UNIMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST\n"});
		return -3;
	}
	delete pres_ver, maj, min, pat, ind;
	string *path = new string;
	getline(ps, *path);
	send_to_log_v2({"preset.cpp (parse_preset): got encoder definition path: ", *path, "\n"});
	int *res = new int;
	*res = parse_def(*path, switches);
	if(*res == -1){
		send_to_log_v2({"preset.cpp (parse_preset): parse_def() exited with error code corresponding to unrecognised header.\n"});
		return 1;
	}
	delete path, res;
	string output_sw="";
	getline(ps, output_sw); //praise the idiot who wrote that spec, no way of verifiying validity.
	send_to_log_v2({"preset.cpp (parse_preset): got output switch \"", output_sw, "\"\n"});
	string enc_opt_def;
	getline(ps, enc_opt_def);
	//TODO: implement encoder option definition parser. Multidimentional vector needed.
	//til' string is empty find =, name= string.substr(0,=-1), remove first 4 characters, 
	return true;
}
