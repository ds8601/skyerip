#include <iostream>
#include <fstream>
#include "depchk.cpp"
using namespace std;
//Day 2 goal: start working on checking if dependencies are installed, and above minimum versions required for vapoursynth (Python 3.12. Technically VS is in pip, but better safe than sorry, and this code will be reused later for actually spawning the encoding process. OR NOT, DEPENDS ON IF OUTPUT PARSING MIGHT BE USEFUL OR NOT.
int main(){
	//TEMP: quick-input for filename, queuing and sensible menus TBD.
	cout << "Filename" << endl;
	string filename; //TODO: implement unicode.
	cin >> filename;
	//TODO: make LOG FILE, name = filename+date in iso-8601.
	string logfile = "logfile.log";
	fstream log;
	log.open(logfile, fstream::in);
	if(log.is_open()){
		cout << "FIXME: LOG ROTATION REQUIRED." << endl;
		return 1; //EXIT: LOGFILE EXISTS. COULD BE FIXED, BUT THAT'S FOR LATER.
	}
	log.close();
	log.open(logfile, fstream::out);
	log << "testing. filename:" << filename << endl;
	//Creating this log file will be moved to a launch flag in the future, once queueing is implemented there it won't proove too useful.
	if(check_Deps()){
		cout << "Dependency check failed, exiting with error code." << endl;
		return 1;
	}


	//
	////cleanup and exit
	log.close();
	return 0;
}
