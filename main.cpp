#include <iostream>
#include <fstream>
#include "depchk.cpp"
#include "log.hpp"
#include "preset.h"
#include "defines.h"
using namespace std;
vector<sw> *vsw;
extern string encpath;
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
	int vppr = parse_preset(sel_preset_v, 0, vsw);
	//TODO: Implement selection manipulation
	//TODO: repeat with audio codec preset selection
	//TODO: Implement actual code using the preset at first with ffmpeg as a viability test. Will be replaced by vapoursynth for v 1.0.
	//possible improvement for later: same preset format for containers.
	////cleanup and exit
	close_log();
	return 0;
}
