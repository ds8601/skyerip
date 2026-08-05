#include <iostream>
#include <fstream>
#include "depchk.cpp"
#include "log.hpp"
#include "preset.h"
#include "defines.h"
using namespace std;
vector<sw> vencsw;
vector<sw> aencsw;
string vencpath;
string fn = "main.cpp ";
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
	int vppr = parse_preset(sel_preset_v, 0);
	if(vppr != 0){send_to_log_v2({fn, "something went seriously wrong while parsing.\n"});
		return -1;}
	else send_to_log_v2({fn, "parser exited gracefully \n"});
	string vidcommand = "ffmpeg -hide_banner -loglevel error -i ";
	vidcommand+= filename;
	vidcommand+= " -pix_fmt yuv444p10le -strict -1 -f yuv4mpegpipe - |";
	vidcommand+= vencpath;
	vidcommand+= " ";
	for(int i=0; i<vencsw.size(); i++){
		if(vencsw[i].set == true){
			//TODO: FINISH PROTOTYPE
		}
	}
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
