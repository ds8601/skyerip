//Todo: convert log file into utf16 (because utf8 is unfortunately locked behind c++20);
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <ctime>
#include <sys/stat.h>
#include "log.hpp"
using namespace std;
fstream logfile;
wfstream wlogfile;
string LOGDIR="/var/tmp/skyerip";
bool open_log(){
	time_t secs = std::time(NULL);
    string fn = "skyerip-";
    fn = LOGDIR+"/"+fn+int_to_string(secs)+".log"; //this is jank, ideally I'd use UTC TIME, but THIS WORKS FOR THE PURPOSE OF NEVER NEEDING TO DELETE THE DAMN LOGS AGAIN.
	secs = std::time(NULL); //hehe
	string fn2 = LOGDIR+"/"+"skyerip-wchar-"+int_to_string(secs)+".log";
    logfile.open(fn, fstream::in);
    if(logfile.is_open()){
        cout << "FIXME: LOG ROTATION REQUIRED." << endl;
        return 1; //EXIT: LOGFILE EXISTS. COULD BE FIXED, BUT THAT'S FOR LATER.
    }
    logfile.close();
    logfile.open(fn, fstream::out);
    if(logfile.is_open());
	else{
		mkdir(LOGDIR.c_str(), 0777);
		logfile.open(fn, fstream::out);
		if(logfile.is_open());
		else {
		cout << "FAILED TO OPEN LOG FILE" << endl;
		return 1;
	}}
	wlogfile.open(fn2, wfstream::in);
	if(wlogfile.is_open()){
		cout << "FIXME: Possible timing bug. wlogfile exists?\n";
		return 1;
	}
	wlogfile.close();
	wlogfile.open(fn2, wfstream::out);
	if(wlogfile.is_open()) return 0;
	else{
		cout << "FIXME: something makes absolutely no fucking sense here, can't create wlogfile, but LOGDIR exists\n";
		return 1;
	}
}

void send_to_log_v2(vector<string> inputs){
	long unsigned int i=0; //shut -Wall -Wextra up.
	while(i<inputs.size()){
		logfile << inputs[i];
		i++;
	}
	logfile<<std::flush;
}

void send_to_log_v2(vector<wstring> inputs){
	long unsigned int i=0;
	while(i<inputs.size()){
		wlogfile<<inputs[i];
		i++;
	}
	wlogfile<<std::flush;
}

void close_log(){
	logfile.close();
	wlogfile.close();

}

string uint_to_string(unsigned int input){
	string output="";
	if(input == 0) return "0";
	else{
	while(input>0){
		output+=char((input%10)+'0');
		input = input / 10;
	}
	reverse(output.begin(), output.end());
	return output;}
}

string int_to_string(int input){
	if(input == 0) return "0";
	else{
		if(input>0) return uint_to_string(input); //reuse existing code, it'll be fine.
		else{
			string out = "-";
			string result = uint_to_string((input * -1)); //get rid of abs, where it's pointless.
			out+= result;
			return out;
		}
	}
	return "Unreachable";
}
			
