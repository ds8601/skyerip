#define _UNICODE //this is an attempt at coaxing mediainfo into working. TODO: convert all strings into utf-16le (as utf8 is locked away by c++20, whilst I'm trying my hardest to stick to c++17 due to clang still not enabling c++20 by default)
#include <iostream>
#include <fstream>
#include <dirent.h>
//#include <sys/stat.h>
#include "checks.h" //why exactly is this shit even here?
#include "depchk.cpp"
#include "helper.h"
#include "log.hpp"
#include "preset.h"
#include "defines.h"
#include <chrono> //why is this even imported?
#include <MediaInfo/MediaInfo.h>

using namespace std;
string fn = "main.cpp ";
//TODO: move all of this shit into a single shared variables cpp file.
vector<vector<sw_v2>> testAudio;
vector<sw_v2> vencsw; //deprecate this
vector<sw_v2> aencsw;
string vencpath;
string vid_chro;
//Build string
#ifndef __BUILDTIME__
string builddate = date_to_isodate(__DATE__); //this is so fucking stupid that it genuinely deserves a seperate, dedicated rant to __DATE__ and its amazing definition.
#else
string builddate = __BUILDTIME__; //Custom macro defined at compile time. see compile.sh
#endif
string version_string = "Skyerip Version 0.0.1-devel \'Petition\'\n(c) 2026 Skye Wierzchowska (ds8601/xpeq7) AND (currently still potential) contributors\nBuild date: " + builddate +"\n \n"; 
//end variable declarations

int main(){
	cout << version_string;
	if(open_log() == 1){
		cout << "Failed to open logfile, exiting. \n";
		return 1;
	}
	send_to_log_v2({version_string});
	if(check_Deps()){
		cout << "Dependency check failed, exiting with error code." << endl;
		return 1;
	}
	//initialise MediaInfo.
	MediaInfoLib::MediaInfo mi;
	wstring mi_ver = mi.Option(L"Info_version", L"v26.05;SkyeRipCLI;0.0.1-devel");
	send_to_log_v2({mi_ver, L"\n"});
	mi.Option(L"Internet", L"No"); //There's literally no valid reason for a statically linked binary to even try to reach out to the net to get a newer version.
	ask_fn:
	cout << "Filename: ";
	string filename; //TODO: implement unicode.
	getline(cin, filename);
	if(chkFileExists(filename) != 0){
		send_to_log_v2({fn, "(main): specified filename \"", filename, "\" doesn't exist, asking again\n"});
		cout << "Skyerip: File not found." << endl;
		goto ask_fn;
	}
	send_to_log_v2({"filename:", filename, "\n"});
	//Notice me (heh): new code: attempt at using mediainfo to check the input files contents for video and audio tracks.
	wstring temp_filename(begin(filename), end(filename));
	mi.Open(temp_filename);
	mi.Option(L"Inform", L"General;%VideoCount%");
	wstring vidCountWStr = mi.Inform();
	if(vidCountWStr == L""){
		cout << "Skyerip: no video streams in file, wat?\n" << endl;
		send_to_log_v2({fn, "(main): no video streams in file, asking for a different one\n"});
		goto ask_fn;
	}
	wcout << L"Video stream count: "<< vidCountWStr << endl;
	mi.Option(L"Inform", L"General;%AudioCount%");
	wstring audCountWStr = mi.Inform();
	if(audCoountWStr == L""){
		wcout << L"Audio stream count: 0" << endl << L"Skyerip: IF by some offchance your input file has audio, check it with mediainfo, if that shows no audio streams, file a bug there." << endl; 
		send_to_log_v2({fn, "0 Audio Streams\n"});
	}
	else wcout << L"Audio stream count: " << audCountWStr << endl;
	mi.Option(L"Inform", L"Video;%ID%#");
	wstring vidIDs = mi.Inform();
	mi.Option(L"Inform", L"Audio;%ID%#");
	wstring audIDs = mi.Inform();
	//TODO: parse all of this shit along with possible channel mappings 

//this could be moved to a separate "file and folder operations" file to clean this code up.
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
ask_vpres: //goto section not to spam any self-recurrencial functions, hopefully.
	cout << "Video Preset: ";
	string sel_preset_v;
	cin >> sel_preset_v;
	sel_preset_v = PREFIX + "/" + PRESET_DIR + "/" + sel_preset_v + ".srps";
	if(chkFileExists(sel_preset_v) != 0){
		send_to_log_v2({fn, "(main): specified preset \"", sel_preset_v, "\" couldn't be found, (could it be that someone remembered to specify the file extension?)\n"});
		cout << "Skyerip: File not found. Try without typing the \".srps\" file extension\n";
		goto ask_vpres;
	}
	int vppr = parse_preset(sel_preset_v, 0);
	if(vppr != 0){ //FIXME: legacy code with assumption on vppr being bool, switch (parse_preset (etc)) would be a better implementation.
		send_to_log_v2({fn, "something went seriously wrong while parsing.\n"});
		cout << "Skyerip: Video preset or encoder definition file is structured wrong.\n";
		return -1;}
	else send_to_log_v2({fn, "parser exited gracefully \n"});
	string vidcommand = "ffmpeg -hide_banner -loglevel error -i \"" + filename + "\" -pix_fmt " + vid_chro + " -strict -1 -f yuv4mpegpipe - | " + vencpath + " " + vin + " ";

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
	//
ask_output:
	cout << "Output (RAW ENCODER OUTPUT AT THE MOMENT): ";
	string out;
	cin.ignore();
	getline(cin, out);
	if(chkFileExists(out) == 0){
		send_to_log_v2({fn, "(main): \"", out, "\" already exists. Asking again for an output file\n"});
		cout << "\"" << out << "\" already exists, gimmie a different output name" << endl;
		goto ask_output;
	}
	vidcommand += " " + vou + " " + out;
	cout << "proposed fopen command: " << vidcommand << endl;
	//TODO: Implement selection manipulation
	//TODO: repeat with audio codec preset selection
	//TODO: Implement actual code using the preset at first with ffmpeg as a viability test. Will be replaced by vapoursynth for v 1.0.
	//possible improvement for later: same preset format for containers.
	////cleanup and exit
	close_log();
	return 0;
}
