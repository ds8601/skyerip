#include <iostream>
#include <fstream>
using namespace std;

int main(){
	//dummy code.
	cout << "Filename" << endl;
	string filename; //TODO: implement unicode.
	cin >> filename;
	cout << filename << endl; //TODO: make LOG FILE, name = filename+date in iso-8601.
	string logfile = "logfile.log"; //TODO: absolutely shit code.
	fstream log;
	log.open(logfile, fstream::in);
	if(log.is_open()){
		cout << "FIXME: LOG ROTATION REQUIRED." << endl;
		return 1;
	}
	log.close();
	log.open(logfile, fstream::out);
	log << "testing. filename:" << filename << endl;
	log.close();
	return 0;
	//code here.
}
