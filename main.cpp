#include <iostream>
#include <fstream>
#include "depchk.cpp"
#include "log.hpp"
using namespace std;
//Day 3: small code cleanup, moved log closing to log.cpp, new spec definitions and preparation for implementaton.
int main(){
	if(open_log() == 1){
		return 1;
	}
	//TEMP: quick-input for filename, queuing and sensible menus TBD.
	cout << "Filename: ";
	string filename; //TODO: implement unicode.
	cin >> filename;
	send_to_log("filename:");
	send_to_log(filename);
	send_to_log("\n");
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
	//TODO: Implement selection manipulation
	//paranoia
	send_to_log("sel_preset_v:");
	send_to_log(sel_preset_v);
	send_to_log("\n");
	//TODO: repeat with audio codec preset selection
	//TODO: Implement actual code using the preset at first with ffmpeg as a viability test. Will be replaced by vapoursynth for v 1.0.
	//possible improvement for later: same preset format for containers.
	////cleanup and exit
	close_log();
	return 0;
}
