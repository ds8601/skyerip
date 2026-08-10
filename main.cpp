#include <iostream>
#include <fstream>
#include "depchk.cpp"
#include "log.hpp"
#include "preset.h"
#include "defines.h"
using namespace std;
vector<sw_v2> vencsw;
vector<sw_v2> aencsw;
string vencpath;
string fn = "main.cpp ";
string PREFIX=".";
string ENCDEF_DIR="encdefs";
string PRESET_DIR="presets";
int main(){
	if(open_log() == 1){
		return 1;
	}
	//TEMP: quick-input for filename, queuing and sensible menus TBD.
	cout << "Filename: ";
	string filename; //TODO: implement unicode.
	cin >> filename;
	send_to_log_v2({"filename:", filename, "\n"});
	//TODO: make LOG FILE, name = filename+date in iso-8601.
	//Creating this log file will be moved to a launch flag in the future, once queueing is implemented there it won't proove too useful.
	if(check_Deps()){
		cout << "Dependency check failed, exiting with error code." << endl;
		return 1;
	}
	//TODO: Implement preset auto-listing
	cout << "Video Preset: ";
	string sel_preset_v;
	cin >> sel_preset_v;
	sel_preset_v = PREFIX + "/" + PRESET_DIR + "/" + sel_preset_v + ".srps";
	int vppr = parse_preset(sel_preset_v, 0);
	if(vppr != 0){send_to_log_v2({fn, "something went seriously wrong while parsing.\n"});
		return -1;}
	else send_to_log_v2({fn, "parser exited gracefully \n"});
	string vidcommand = "ffmpeg -hide_banner -loglevel error -i ";
	vidcommand+= filename;
	vidcommand+= " -pix_fmt yuv444p10le -strict -1 -f yuv4mpegpipe - | ";
	vidcommand+= vencpath;
	vidcommand+= " ";
	//ALL OF THIS NEEDS TO BE REWRITTEN
	/*
	for(unsigned long int i=0; i<vencsw.size(); i++){
		if(vencsw[i].set == true){
			switch (vencsw[i].type){
				case 0: {
						vidcommand+= "--" + vencsw[i].name;
						break;
					}
				case 1: {
						// int devided by anotyher int. fuck.
						vidcommand+= "--" + vencsw[i].name + "=";
						//FIXME: This needs to be moved to a helper function for code readability.
						int val = vencsw[i].val;
						int den = vencsw[i].valid_den; //FIXME: Either remove vencsw[i].den OR ASK every time for denominator ONLY IF TYPE==double.
						int temp = val%den;
						vidcommand+= int_to_string(val/den);
						val = temp;
						if(val>0){
							vidcommand+=".";
							while(val != 0){
								val = val * 10;
								temp = val%den;
								vidcommand+= int_to_string(val/den);
								val = temp;
							}
						}
						break;
					}
				case 2: {
						vidcommand+= "--" + vencsw[i].name + "=" + vencsw[i].multi_vars;
						send_to_log_v2({fn, "(main): WARN: switch ", vencsw[i].name, " is type 2 (string), WON'T BE VERIFIED FOR VALIDITY, EXPECT ERRORS OR WARNINGS ON FRESH PRESETS\n"});
						break;
					}
				case 3:{
					       send_to_log_v2({fn, "(main): MULTIVAR REMAINS YET TO BE IMPLEMENTED\n"});
					       break;
				       }
				default:{
					send_to_log_v2({fn, "(main): vencsw[i].set is true, but type isn't set to any recognized type. Not sure what to do with this shit, ignoring. \n"});
					break;
					}
			}
			vidcommand+=" ";
		}
	}
	*/
	//TODO: MOVE TO HELPER FUNCTION
	cout << "proposed fopen command:" << vidcommand << endl;
	//TODO: Implement selection manipulation
	//TODO: repeat with audio codec preset selection
	//TODO: Implement actual code using the preset at first with ffmpeg as a viability test. Will be replaced by vapoursynth for v 1.0.
	//possible improvement for later: same preset format for containers.
	////cleanup and exit
	close_log();
	return 0;
}
