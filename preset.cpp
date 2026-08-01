#include <iostream>
#include <fstream>
#include <filesystem>
#include "log.hpp"
#include "defines.h"
#include "helper.h"
using namespace std;
//TODO: minimise memory footprint by making all temporary variables dynamic (who cares what the preset file version number is, if it's already validated) 
unsigned int dots[2];
fstream ps;
//filename and type, futureproofing, because a muxer will need a separate kind of preset format.
bool parse_preset(string fn, int t){
	send_to_log_v2({"preset.cpp (parse_preset): opening \"", fn, "\" type ", {char(t+'0')}, "\n"});
	ps.open(fn, fstream::in);
	if(!ps.is_open()){
		send_to_log_v2({"preset.cpp (parse_preset): ERROR: PRESET FILE DOESN'T EXIST. RETURNING FALSE\n"});
		return false;
	}
	//parse header. version number
	string pres_ver;
	getline(ps, pres_ver);
	send_to_log_v2({"preset.cpp (parse_preset): read version as ", pres_ver, "\n"});
	if(!locate_dots(pres_ver, 2, dots)){
		send_to_log_v2({"preset.cpp (locate_dots): more than 2 dots, preset file corrupt?\n"});
		return false;
	}
	//declare new int variables to check if the number is lower (or different. to be decided in version 0.0.1-final of preset file spec.)
	char pres_ind = pres_ver.back(); //read indicator first despite being checked last, for easier version parsing
	pres_ver.pop_back(); //delete the indicator. TODO: delete entire string after converting it to uint's.
	unsigned int pres_maj = uintInSubstr_to_uint(pres_ver.substr(0,1));
	unsigned int pres_min = uintInSubstr_to_uint(pres_ver.substr(dots[0]+1, 1));
	unsigned int pres_patch = uintInSubstr_to_uint(pres_ver.substr(dots[1]+1,pres_ver.size()));
	send_to_log_v2({"preset.cpp (parse_preset): read major version of preset file as ", uint_to_string(pres_maj), "\n"});
	send_to_log_v2({"preset.cpp (parse_preset): read minor version of preset file as ", uint_to_string(pres_min), "\n"});
	send_to_log_v2({"preset.cpp (parse_preset): read patch version of preset file as ", uint_to_string(pres_patch), "\n"});
	send_to_log_v2({"preset.cpp (parse_preset): read indicator version of preset file as ", {pres_ind}, "\n"});
	//check. TEMP BEHAVIOR: IF DOESN'T MATCH DEFINITIONS AT TOP OF SOURCE FILE, THEN EXIT WITH FALSE WHILE LOGGING "preset file version doesn't match. UNINMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST"
	if(pres_maj != CURRENT_MAJ || pres_min != CURRENT_MIN || pres_patch != CURRENT_PAT || pres_ind != CURRENT_IND){
		send_to_log_v2({"preset.cpp (parse_preset): preset file version doesn't match. UNIMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST\n"});
		return false;
	}
	//TODO:implement real parsing, halfway done, this code (if cleaned up further using shared functions) is a really good starting point for any future preset-up-conversion code.
	//preset version verified valid. proceeding
	string path;
	getline(ps, path);
	send_to_log_v2({"preset.cpp (parse_preset): got encoder path: ", path, "\n"});
	//FIXME: Check if file is executable, again pre-alpha software things.
	if(!std::filesystem::exists(path)){
		send_to_log_v2({"preset.cpp (parse_preset): specified encoder path \"", path, "\" doesn't exist. RETURNING FALSE.\n"});
		return false;
	}
	//path check DONE.
	string output_sw="";
	getline(ps, output_sw); //praise the idiot who wrote that spec, no way of verifiying validity.
	send_to_log_v2({"preset.cpp (parse_preset): got output switch \"", output_sw, "\"\n"});
	string enc_opt_def;
	getline(ps, enc_opt_def);
	//TODO: implement encoder option definition parser. Multidimentional vector needed.
	return true;
}
