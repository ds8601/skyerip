//preset.cpp, PRE-PROTOTYPE CODE
// (c) 2026, Skye Wierzchowska (ds8601), license to be decided.
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include "log.hpp"
#include "defines.h"
#include "helper.h"
using namespace std;
fstream ps;
unordered_map<string, int> vswloc;
unordered_map<string, int> aswloc;
string vin;
string vou;
vector<string> supchr;
string ain; //FIXME: multi-track audio is common-place, thus a vector of strings here would do a much better job.
string aou; //FIXME: again, a string vector here would be better
//we really need a warning type switch in g++ that will warn against unused library inclusion.
//filename and type, futureproofing, because a muxer will need a separate kind of preset format.
//TODO: this still could be simplified even further by placing the push_back statements at the very end and branching off at the very beginning of the function, even possibly moving back the type intiger in the spec and removing *multi entirely.
int parse_def(string path, int t){ //NOTE: THIS CODE IS IN THE PROCESS OF BEING MODERNIZED FOR struct sw_v2, start 2028-08-09 21:51, end 2026-08-09 23:31 CEST
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
	send_to_log_v2({funcname, "definition file version ", uint_to_string(*maj), {'.'}, uint_to_string(*min), {'.'}, uint_to_string(*pat), {*ind}, "\n"});
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
	if(t==0) getline(p, vin);
	else getline(p, ain);
	send_to_log_v2({funcname, "set input variable to: \"", vin, "\"\n"}); //FIXME: AUDIO SUPPORT;
	if(t==0) getline(p, vou);
	else getline(p, aou);
	send_to_log_v2({funcname, "set output variable to: \"", vou, "\"\n"}); //FIXME: AUDIO SUPPORT.
	//end header
	string chroma;
	getline(p, chroma);
	send_to_log_v2({funcname, "got supported chroma string \"", chroma, "\"\n"});
	vector<long unsigned int> chroma_commas = locate_char(chroma, ',');
	if(chroma_commas.size()==0){
		send_to_log_v2({funcname, "NO commas, means only ONE SUPPORTED INPUT CHROMA, pushing back \"", chroma, "\"\n"});
		supchr.push_back(chroma);
	}
	else{
		send_to_log_v2({funcname, "pushing back supchr[0] \"", chroma.substr(0,chroma_commas[0]), "\"\n"});
		supchr.push_back(chroma.substr(0,chroma_commas[0]));
		for(unsigned long i=1; i<chroma_commas.size(); i++){
			send_to_log_v2({funcname, "pushing back supchr[", int_to_string(i), "] \"", chroma.substr(chroma_commas[i-1]+1,chroma_commas[i]-chroma_commas[i-1]-1), "\"\n"});
			supchr.push_back(chroma.substr(chroma_commas[i-1]+1,chroma_commas[i]-chroma_commas[i-1]-1));
		}
		send_to_log_v2({funcname, "pushing back last supchr \"", chroma.substr(chroma_commas.back()+1,chroma.size()-chroma_commas.back()), "\"\n"});
		supchr.push_back(chroma.substr(chroma_commas.back()+1,chroma.size()-chroma_commas.back()));
	}
	string swdef; // the one string that I can't dynamically allocate due to ... *swdef[i] comparison against a char not working.
	while (getline(p, swdef)){ //begin spaget. FIXME: this shit is definitely unoptimized.
		if(swdef == "EOF") return 0; //workaround stupidity.
		send_to_log_v2({funcname, "read \"", swdef, "\" from file to *swdef\n"});
		sw_v2 cur;
		vector<long unsigned int> commas = locate_char(swdef, ',');
		cur.in_mvar = intInSubstr_to_int(swdef.substr(0,1));
		send_to_log_v2({funcname, "set cur.in_mvar to ", int_to_string(cur.in_mvar), "\n"});
		cur.name = swdef.substr(commas[0]+1, commas[1]-commas[0]-1);
		send_to_log_v2({funcname, "set cur.name to \"", cur.name, "\"\n"});
		cur.type = intInSubstr_to_int(swdef.substr(commas[1]+1,commas[2]-commas[1]-1));
		send_to_log_v2({funcname, "set cur.type to ", int_to_string(cur.type), "\n"});
		switch (cur.type){
			case -1:{
					cur.excl = swdef.substr(commas[2]+1,swdef.size()-commas[2]);
					send_to_log_v2({funcname, "set cur.excl to \"", cur.excl, "\", that's everything for type -1, pushing back\n"});
					break;
				}
			case 0:{
				       send_to_log_v2({funcname, "type is 0, pushing back\n"});
					break;
				}
			case 1:{
				       cur.min = intInSubstr_to_int(swdef.substr(commas[2]+1,commas[3]-commas[2]-1));
				       send_to_log_v2({funcname, "set cur.min to ", int_to_string(cur.min), "\n"});
				       cur.max = intInSubstr_to_int(swdef.substr(commas[3]+1, swdef.size()-commas[3]));
				       send_to_log_v2({funcname, "set cur.max to ", int_to_string(cur.max), ", pushing back\n"});
				       break;
			       }
			case 2:{
				       send_to_log_v2({funcname, "type is 2, pushing back\n"});
				       break;
			       }
			case 3:{
				       cur.mvar = swdef.substr(commas[2]+2, swdef.size() - commas[2] - 1);
				       send_to_log_v2({funcname, "set cur.mvar to \"", cur.mvar, "\", pushing back\n"});
				       break;
			       }
			case 4:{ //unfortunate code duplication
				 cur.min = intInSubstr_to_int(swdef.substr(commas[2]+1,commas[3]-commas[2]-1));
				 send_to_log_v2({funcname, "set cur.min to ", int_to_string(cur.min), "\n"});
				 cur.max = intInSubstr_to_int(swdef.substr(commas[3]+1, swdef.size()-commas[3]));
				 send_to_log_v2({funcname, "set curr max to ", int_to_string(cur.max), ", pushing back\n"});
				 break;
			       }
			default:{
					send_to_log_v2({funcname, "how did we get to a non-defined type? \n"});
					return -3;
				}
		}
		if(t==0){
			vencsw.push_back(cur);
			vswloc[cur.name] = vencsw.size()-1;
		}
		else{
			aencsw.push_back(cur); //FIXME: AUDIO SUPPORT.
			aswloc[cur.name] = aencsw.size()-1;
		}
		}
	send_to_log_v2({funcname, "EXITING SUCCESFULLY.\n"});
	p.close();
	return 0;
}

int handle_multivar(string name){
	// code will go here tomorrow.
	return 0;
}


int parse_preset(string fn, int t){ //begin sw_v2 rewrite: 2026-08-10 20:19 CEST. FIXME: NONE OF THIS SHIT SUPPORTS AUDIO.
	string funcname = "preset.cpp (parse_preset): ";
	send_to_log_v2({funcname, "opening \"", fn, "\" type ", {char(t+'0')}, "\n"});
	ps.open(fn, fstream::in);
	if(!ps.is_open()){
		send_to_log_v2({funcname, "ERROR: PRESET FILE DOESN'T EXIST. RETURNING -1\n"});
		return -1; //magic numbers, yay
	}
	string *preset = new string;
	getline(ps, *preset);
	if (*preset != "srps"){
		send_to_log_v2({funcname, "not a recognised preset file format. returning -2\n"});
		return -2;
	}
	delete preset;
	string* pres_ver = new string;
	unsigned int *maj = new unsigned int, *min = new unsigned int, *pat = new unsigned int;
	char *ind = new char; //valgrind is complaining that this is 0 bytes after getting 1 byte assigned to it. TF?
	getline(ps, *pres_ver);
	int vcheck = check_definition_version(*pres_ver, 1, maj, min, pat, ind);
	if(vcheck == -1){
		send_to_log_v2({funcname, "preset file version header doesn't match defined standard (more than 2 dots), returning -2\n"});
		return -2;
	}
	send_to_log_v2({funcname, "preset file version ", uint_to_string(*maj), {'.'}, uint_to_string(*min), {'.'}, uint_to_string(*pat), {ind}, "\n"});
	//check. TEMP BEHAVIOR: IF DOESN'T MATCH DEFINITIONS AT TOP OF SOURCE FILE, THEN EXIT WITH FALSE WHILE LOGGING "preset file version doesn't match. UNINMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST"
	if(*maj != CURENCP_MAJ || *min != CURENCP_MIN || *pat != CURENCP_PAT || *ind != CURENCP_IND){
		send_to_log_v2({funcname, "preset file version doesn't match. UNIMPLEMENTED. POSSIBLY CORRUPTED FILE. MESSAGE INTRODUCED 2026-07-31 16:12 CEST\n"});
		return -3;
	}
	delete pres_ver; //again, longer less pretty form to shut -Wall -Wextra up.
	delete maj;
	delete min;
	delete pat;
	delete ind;
	string path;
	getline(ps, path);
	path = PREFIX + "/" + ENCDEF_DIR + "/" + path;
	send_to_log_v2({funcname, "got encoder definition path: ", path, "\n"});
	int *res = new int;
	*res = parse_def(path, t);
	if(*res != 0){
		send_to_log_v2({funcname, "parse_def() exited with error code corresponding to unrecognised header.\n"});
		return -4;
	}
	delete res;
	string chroma;
	getline (ps, chroma);
	bool is_okay = false;
	for(int i=0; i<supchr.size(); i++){
		if(supchr[i] == chroma){
			is_okay = true;
			break;
		}
	}
	if(is_okay == false){
		send_to_log_v2({funcname, "chroma subsampling value \"", chroma, "\" IS NOT SUPPORTED ACCORDING TO ENCODER DEFINITION, FIX EITHER.\n"});
		return -4;
	}
	queue<string> multivar;
	string sw;
	//for(auto i = vswloc.begin(); i != vswloc.end(); i++){
	//	send_to_log_v2({funcname, "DEBUG: \"", i->first, "\" EXISTS\n"});
	//}
	while(getline(ps, sw)){
		if(sw == "EOF"){
			send_to_log_v2({funcname, "saw EOF, ending fun with reading this\n"});
			break;
		}
		else{
			vector <long unsigned int> commas = locate_char(sw, ',');
			string name = sw.substr(0,commas[0]);
			if(vswloc.find(name)==vswloc.end()){
				send_to_log_v2({funcname, "preset declares a switch \"", name, "\" not defined by encoder definition, exiting before std::unsorted_map does UB\n"});
				return -5;
			}
			int type = intInSubstr_to_int(sw.substr(commas[0]+1,1));
			switch (type){ //rewrite this shit
				case -1: {
						 //check IF THE OPPOSITE IS DEFINED.
						 if(vswloc.find(vencsw[vswloc[name]].excl)!=vswloc.end() && vencsw[vswloc[vencsw[vswloc[name]].excl]].set == true){ //first part is a safeguard against my OR ANYONE ELSE'S STUPIDITY IN FILLING OUT THE DEFINITION FILE.
							send_to_log_v2({funcname, "OPTION WHICH EXCLUSE USE OF", name, "EXITING BEFORE WE GET AN ERROR\n"});
							return -5;
						 }
						 else {
							 vencsw[vswloc[name]].set = true;
							 send_to_log_v2({funcname, "set vencsw[vswloc[name]].set to TRUE\n"});
						 }
						 break;
					 }
				case 0: {
					send_to_log_v2({funcname, "enabling ", name, "\n"});
					vencsw[vswloc[name]].set = true;
					break;
					}
				case 1: {
					int value = intInSubstr_to_int(sw.substr(commas[1]+1,sw.size()-commas[1]));
					send_to_log_v2({funcname, "detected value as \"", int_to_string(value), "\", setting that in vector.\n"});
					vencsw[vswloc[name]].set = true;
					vencsw[vswloc[name]].val = value;
					break;
					}
				case 2: {
					string value = sw.substr(commas[1]+1, sw.size()-commas[1]);
					send_to_log_v2({funcname, "detected value as \"", value, "\", setting that in vector as multi_var.\n"});
					vencsw[vswloc[name]].set = true;
					vencsw[vswloc[name]].mvar = value;
					break;
					}
				case 3: {
					send_to_log_v2({funcname, name, " is MULTIVAR, putting in queue\n"});
					multivar.push(name);
					break;
					}
				case 4: {
					int value = intInSubstr_to_int(sw.substr(commas[1]+1, commas[2]-commas[1]-1));
					send_to_log_v2({funcname, "set value to ", int_to_string(value), "\n"});
					int den = intInSubstr_to_int(sw.substr(commas[2]+1, sw.size()-commas[1]));
					send_to_log_v2({funcname, "set denominator to ", int_to_string(den), "\n"});
					vencsw[vswloc[name]].val = value;
					vencsw[vswloc[name]].den = den;
					break;
					}
				default:{
						send_to_log_v2({funcname, "UNEXPECTED TYPE ON ", name, " EXITING.\n"});
						return -5;
					}
			}
			continue;
		}
	}
	sw="";
	for(; !multivar.empty(); multivar.pop()){
		if(handle_multivar(multivar.front()) != 0){
			send_to_log_v2({funcname, "something went wrong in multivar population handler\n"});
			return -6;
		}
	}
	vswloc.clear();
	aswloc.clear();
	send_to_log_v2({funcname, "successfully got to end of function without any funny biz \n"});
	ps.close();
	return 0;
} //valgrind notes an invalid read of size 1 being here. tf?
