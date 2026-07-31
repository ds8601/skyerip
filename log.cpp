#include <iostream>
#include <algorithm>
#include <fstream>

#include "log.hpp"
using namespace std;
fstream logfile;
bool open_log(){
    string fn = "logfile.log";
    logfile.open(fn, fstream::in);
    if(logfile.is_open()){
        cout << "FIXME: LOG ROTATION REQUIRED." << endl;
        return 1; //EXIT: LOGFILE EXISTS. COULD BE FIXED, BUT THAT'S FOR LATER.
    }
    logfile.close();
    logfile.open(fn, fstream::out);

    return 0;
}
//2026-07-31 15:31 CEST : THIS IS LEGACY CODE, YES, 4 DAYS IN AND I'M ALREADY DEPRECATING THE CRAPPY LOG FUNCTION. NVM. JUST MAKING IT WORSE, SINCE SAFELY DEFINING THE LOG FILE AS EXTERN IS AN IMPOSSIBILITY.
void send_to_log(string input){
    logfile << input;
}

void send_to_log_v2(int argc, char* argv[]){
	int i=0;
	while(i<argc){
		logfile << argv[i];
		i++;
	}
}


void close_log(){
	logfile.close();
}

string uint_to_string(unsigned int input){
	string output="";
	while(input>0){
		output+=char((input%10)+'0');
		input = input / 10;
	}
	reverse(output.begin(), output.end());
	return output;
}
