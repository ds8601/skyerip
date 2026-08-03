#include <iostream>
#include <algorithm>
#include <vector>
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

void send_to_log_v2(vector<string> inputs){
	long unsigned int i=0; //shut -Wall -Wextra up.
	while(i<inputs.size()){
		logfile << inputs[i];
		i++;
	}
}


void close_log(){
	logfile.close();
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
