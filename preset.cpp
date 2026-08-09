//preset.cpp, PRE-PROTOTYPE CODE
// (c) 2026, Skye Wierzchowska (ds8601), license to be decided.
#include <iostream>
#include <fstream>
#include <unordered_map>
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
		for(int i=1; i<chroma_commas.size(); i++){
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
		cur.type = intInSubstr_to_int(swdef.substr(commas[1]+1,1));
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

int parse_preset(string fn, int t){
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
	string *path = new string;
	getline(ps, *path);
	send_to_log_v2({funcname, "got encoder definition path: ", *path, "\n"});
	int *res = new int;
	*res = parse_def(*path, t);
	if(*res != 0){
		send_to_log_v2({funcname, "parse_def() exited with error code corresponding to unrecognised header.\n"});
		return -4;
	}
	delete path;
	delete res;
	string sw; //maybe hash maps would've been a better pick, to be researched after I have a working prototype.
	while(getline(ps, sw)){
		if(sw == "EOF"){
			send_to_log_v2({funcname, "saw EOF, ending fun with reading this\n"});
			break;
		}
		else if (sw[0] == '$'){
			send_to_log_v2({funcname, "$INPUT HANDLING UNIMPLEMENTED\n"});
			continue;
		}
		else{
			vector <long unsigned int> commas = locate_char(sw, ',');
			string name = sw.substr(0,commas[0]);
			unsigned int type = uintInSubstr_to_uint(sw.substr(commas[0]+1,1));
			switch (type){
				case 0: {//shut clang++ up
					send_to_log_v2({funcname, "enabling ", name, "\n"});
					vencsw[vswloc[name]].set = true;
					break;
					}
				case 1:
					{
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
					vencsw[vswloc[name]].multi_vars = value;
					break;
					}
				case 3: {
					//fuckaroo. this will need a special handler to push back populating it til the very end, then and only then gathering the values for it into the multi_var string while unsetting the used variables. that's really the only way. OR it should have a special option to signify it's a multivar variable and thus should be placed in the multivar portion of the appropriate - .... or there needs to be a "belongs to" field in the struct, possibly replacing the unused line number one.
					send_to_log_v2({funcname, "MULTIVAR handling remains to be implemented\n"});
					break;
					}
			}
			continue;
		}
	}
	//random cleanup shit introduced while debugging another of the skye-special index off by one every-fucking-where errors.
	sw="";
	vswloc.clear();
	aswloc.clear();
	send_to_log_v2({funcname, "successfully got to end of function without any funny biz \n"});
	ps.close();
	return 0;
} //valgrind notes an invalid read of size 1 being here. tf?
