#include <iostream>
#include <fstream>
#include "log.hpp"
#include "defines.h"
using namespace std;
//TODO: minimise memory footprint by making all temporary variables dynamic (who cares what the preset file version number is, if it's already validated) 
unsigned int dots[2];
bool locate_dots(string pres_ver){
	unsigned int c=0;
	for (int i=0; i<pres_ver.length(); i++){
		if(pres_ver[i] == '.'){
			if(c<2){
			dots[c]=i;
			c++;}
			else return false;
		}
	}
	return true;
}
//TODO: implement string_portion_to_uint(string pres_ver, int start, int stop);
fstream ps;
//filename and type, futureproofing, because a muxer will need a separate kind of preset format.
bool parse_preset(string fn, int t){
	//paranoia, the log function has been superceeded by a better v2 variant.
	send_to_log_v2(5,["preset.cpp (parse_preset): opening \"", fn, "\" type", char(t+'0'), "\n"]);
	ps.open(fn, fstream::in);
	if(!ps.is_open()){
		send_to_log_v2(1, ["preset.cpp (parse_preset): ERROR: PRESET FILE DOESN'T EXIST. RETURNING FALSE\n"]);
		return false;
	}
	//preset file opened as readonly.
	//TODO: MOVE TO VECTOR. 2026-07-31 23:27: the f- u want? the common ones don't need to be in a vector - skye.
	//TODO: REWRITE WITH HELPER FUNCTIONS TO EXTRACT NUMBERS FROM STRING.
	//parse header. version number
	string pres_ver;
	getline(ps, pres_ver);
	send_to_log_v2(3, ["preset.cpp (parse_preset): read version as", pres_ver, "\n"]);
	if(!locate_dots()){
		send_to_log_v2(1, ["preset.cpp (locate_dots): more than 2 dots, preset file corrupt?\n"]);
		return false;
	}
	//declare new int variables to check if the number is lower (or different. to be decided in version 0.0.1-final of preset file spec.)
	unsigned int pres_maj = 0;
	unsigned int pres_min = 0;
	unsigned int pres_patch = 0;
	char pres_ind = '0';
	unsigned int c = 0;
	//read major version
	while(pres_ver[c]!='.') c++;
	unsigned int d = c-1;
	unsigned int dec= 1;
	while(d>=0){
		pres_maj = pres_maj + ((pres_ver[d]-'0')*dec);
		dec = dec * 10;
		d--;
	}
	send_to_log_v2(3, ["preset.cpp (parse_preset): read major version of preset file as ", uint_to_string(pres_maj), "\n"]);
	//read minor version
	c++;
	dec=1;
	while(pres_ver[c]!='.') c++;
	d=c-1;
	while(pres_ver[d]!='.'){
		pres_min = pres_min + ((pres_ver[d]-'0')*dec);
		dec = dec * 10;
		d--;
	}
	dec = 1;
	send_to_log_v2(3, ["preset.cpp (parse_preset): read minor version of preset file as ", uint_to_string(pres_min), "\n"]);
	//read patch version
	c++; //move past last dot
	while(pres_ver[c]<='9' && pres_ver[c]>='0') c++;
	d=c-1;
	while(pres_ver[d]!='.'){
		pres_patch = pres_patch + ((pres_ver[d]-'0')*dec);
		dec = dec * 10;
		d--;
	}
	send_to_log_v2(3, ["preset.cpp (parse_preset): read patch version of preset file as ", uint_to_string(pres_patch), "\n"]);
	//read indicator
	pres_ind=pres_ver[c];
	send_to_log_v2(3, ["preset.cpp (parse_preset): read indicator version of preset file as ", pres_ind, "\n"]);
	//check. TEMP BEHAVIOR: IF DOESN'T MATCH DEFINITIONS AT TOP OF SOURCE FILE, THEN EXIT WITH FALSE WHILE LOGGING "preset file version doesn't match. UNINMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST"
	if(pres_maj != CURRENT_MAJ || pres_min != CURRENT_MIN || pres_patch != CURRENT_PAT || pres_ind != CURRENT_IND){
		send_to_log_v2(1, ["preset.cpp (parse_preset): preset file version doesn't match. UNIMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST\n"]);
		return false;
	}
	//TODO:implement real parsing, halfway done, this code (if cleaned up further using shared functions) is a really good starting point for any future preset-up-conversion code.
	//preset version verified valid. proceeding
	//string path
	string path;
	getline(ps, path);
	send_to_log_v2(3, ["preset.cpp (parse_preset): got encoder path: ", path, "\n"]);
	//FIXME: Check if file is executable, again pre-alpha software things.
	if(!std::filesystem::exists(path)){
		send_to_log_v2(3, ["preset.cpp (parse_preset): specified encoder path \"", path, "\" doesn't exist. RETURNING FALSE.\n"]);
		return false;
	}
	//path check DONE.
	string output_sw="";
	getline(ps, output_sw); //praise the idiot who wrote that spec, no way of verifiying validity.
	send_to_log_v2(3, ["preset.cpp (parse_preset): got output switch \"", output_sw, "\"\n"]);
	//2026-07-31 23:22 CEST: damit my keyboard is really getting worn out - skye
	string enc_opt_def;
	getline(ps, enc_opt_def);
	//TODO: implement encoder option definition parser
	
	return true;
}
