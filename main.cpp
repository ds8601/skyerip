#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include "depchk.cpp"
#include "log.hpp"
#include "preset.h"
#include "defines.h"
using namespace std;
vector<sw_v2> vencsw;
vector<sw_v2> aencsw;
string vencpath;
string vid_chro;
string fn = "main.cpp ";
string PREFIX=".";
string ENCDEF_DIR="encdefs";
string PRESET_DIR="presets";
//fix the following string and compile script to pass along the current date to make sure it displays the build date in UTC time
string version_string = "Skyerip Version 0.0.1-devel 'Petition' \n(c) 2026 Skye Wierzchowska (ds8601/xpeq7) AND (currently still potential) contributors \nBuild date (FIXME: load this from date): 2026-08-11\n \n"; 
int main(){
	struct stat exists_check; // this exists to allow checking if files exist.
	cout << version_string;
	if(open_log() == 1){
		return 1;
	}
	send_to_log_v2({version_string});
	ask_fn:
	//TEMP: quick-input for filename, queuing and sensible menus TBD.
	cout << "Filename: ";
	string filename; //TODO: implement unicode.
	getline(cin, filename);
	if(stat(filename.c_str(), &exists_check) != 0){
		send_to_log_v2({fn, "(main): specified filename \"", filename, "\" doesn't exist, asking again\n"});
		cout << "Skyerip: File not found." << endl;
		goto ask_fn;
	}
	send_to_log_v2({"filename:", filename, "\n"});
	if(check_Deps()){
		cout << "Dependency check failed, exiting with error code." << endl;
		return 1;
	}
	//TODO: Implement preset auto-listing
	cout << "list of files in \"" << PREFIX << "/" << PRESET_DIR << "/\"" << endl;
	//shamelessly based off https://stackoverflow.com/a/612176 AND https://www.man7.org/linux/man-pages/man3/readdir.3p.html
	string pdir = PREFIX + "/" + PRESET_DIR;
	DIR *dir;
	struct dirent *ent;
	if((dir = opendir(pdir.c_str())) != NULL){
		while((ent = readdir(dir)) != NULL){
			cout << ent->d_name << " ";
		}
	}
	closedir(dir);
	cout << endl;
ask_vpres:
	cout << "Video Preset: ";
	string sel_preset_v;
	cin >> sel_preset_v;
	sel_preset_v = PREFIX + "/" + PRESET_DIR + "/" + sel_preset_v + ".srps";
	if(stat(sel_preset_v.c_str(), &exists_check) != 0){
		send_to_log_v2({fn, "(main): specified preset \"", sel_preset_v, "\" couldn't be found, (could it be that someone remembered to specify the file extension?)\n"});
		cout << "Skyerip: File not found. Try without typing the \".srps\" file extension\n";
		goto ask_vpres;
	}
	int vppr = parse_preset(sel_preset_v, 0);
	if(vppr != 0){send_to_log_v2({fn, "something went seriously wrong while parsing.\n"});
		return -1;}
	else send_to_log_v2({fn, "parser exited gracefully \n"});
	string vidcommand = "ffmpeg -hide_banner -loglevel error -i \"";
	vidcommand+= filename;
	vidcommand+= "\" -pix_fmt "+vid_chro+" -strict -1 -f yuv4mpegpipe - | ";
	vidcommand+= vencpath;
	vidcommand+= " ";
	vidcommand+= vin + " ";
	for(unsigned long int i=0; i<vencsw.size(); i++){
		if(vencsw[i].set == true && vencsw[i].in_mvar == false){
			switch (vencsw[i].type){
				case -1: //intentional rollover to case 0
				case 0:{ //NOTHING
					       vidcommand+= "--" + vencsw[i].name;
					       break;
				       }
				case 1:{ //INT
					       vidcommand+= "--" + vencsw[i].name + " " + int_to_string(vencsw[i].val);
					       break;
				       }
				case 3: //again, intentional rollover to case 2.
				case 2:{ //STRING
					 vidcommand+= "--" + vencsw[i].name + "=" + vencsw[i].mvar;
					 break;
				       }
				case 4:{ //FLOATING POINT
					int val = vencsw[i].val;
					int den = vencsw[i].den;
					int temp = val%den;
					string res = int_to_string(val/den);
					if(temp != 0){
						val = temp;
						res+=".";
						while(val != 0){
							val = val *10;
							temp = val%den;
							res+=int_to_string(val/den);
							val = temp;
						}
					}
					vidcommand+= "--" + vencsw[i].name + " " + res;
					break;
				       }
				default:{
						send_to_log_v2({fn, "(main): ", vencsw[i].name, " is of an UNKNOWN TYPE. halp\n"});
						return 1;
					}
			}
			vidcommand+= " ";
		}
	}
	//TODO: MOVE TO HELPER FUNCTION
ask_output:
	cout << "Output (RAW ENCODER OUTPUT AT THE MOMENT): ";
	string out;
	cin.ignore();
	getline(cin, out);
	if(stat(out.c_str(), &exists_check) == 0){
		send_to_log_v2({fn, "(main): \"", out, "\" already exists. Asking again for an output file\n"});
		cout << "\"" << out << "\" already exists, gimmie a different output name" << endl;
		goto ask_output;
	}
	vidcommand += " " + vou + out;
	cout << "proposed fopen command: " << vidcommand << endl;
	//TODO: Implement selection manipulation
	//TODO: repeat with audio codec preset selection
	//TODO: Implement actual code using the preset at first with ffmpeg as a viability test. Will be replaced by vapoursynth for v 1.0.
	//possible improvement for later: same preset format for containers.
	////cleanup and exit
	close_log();
	return 0;
}
