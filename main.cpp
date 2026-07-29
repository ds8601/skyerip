#include <iostream>
#include <fstream>
#include "depchk.cpp"
#include "log.cpp"
using namespace std;
//Day 2 goal: start working on checking if dependencies are installed, and above minimum versions required for vapoursynth (Python 3.12. Technically VS is in pip, but better safe than sorry, and this code will be reused later for actually spawning the encoding process. OR NOT, DEPENDS ON IF OUTPUT PARSING MIGHT BE USEFUL OR NOT.
int main(){
	if(open_log() == 1){
		return 1;
	}
	//TEMP: quick-input for filename, queuing and sensible menus TBD.
	cout << "Filename" << endl;
	string filename; //TODO: implement unicode.
	cin >> filename;
	send_to_log("testing. filename:");
	send_to_log(filename);
	send_to_log("\n a");
	//TODO: make LOG FILE, name = filename+date in iso-8601.
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
