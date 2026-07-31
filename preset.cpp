#include <iostream>
#include <fstream>
#include "log.hpp"
using namespace std;
//define current preset (type 0) format version, if opened preset is newer EXIT WITH ERROR CODE.
#define CURRENT_MAJ 0
#define CURRENT_MIN 0
#define CURRENT_PAT 1
#define CURRENT_IND a

fstream ps;
//filename and type, futureproofing, because a muxer will need a separate kind of preset format.
bool parse_preset(string fn, int t){
	//paranoia, also I need to fix the log function. or just export the log as an extern variable as this kind of send_to_log() spam is really getting on my nerves.
	send_to_log_v2(5,["preset.cpp (parse_preset): opening \"", fn, "\" type", char(t+'0'), "\n"]);
	//end of spam. now the real file opening begins.
	ps.open(fn, fstream::in);
	if(!ps.is_open()){
		send_to_log_v2(1, ["preset.cpp (parse_preset): ERROR: PRESET FILE DOESN'T EXIST. RETURNING FALSE\n"]);
		return false;
	}
	//preset file opened as readonly.
	//TODO: MOVE TO VECTOR.

	//TODO: REWRITE WITH HELPER FUNCTIONS TO EXTRACT NUMBERS FROM STRING.
	//parse header. version number
	string pres_ver;
	getline(ps, pres_ver);
	send_to_log_v2(3, ["preset.cpp (parse_preset): read version as", pres_ver, "\n"]);
	//declare new int variables to check if the number is lower (or different. to be decided in version 0.0.1-final of preset file spec.)
	unsigned int pres_maj = 0;
	unsigned int pres_min = 0;
	unsigned int pres_patch = 0;
	char pres_ind = 0;
	int c = 0;
	//read major version
	while(pres_ver[c]!='.') c++;
	int d = c-1;
	int dec= 1;
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
	}
	//TODO: 
}
