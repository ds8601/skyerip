#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <ctime>
#include <sys/stat.h>
#include <variant>
#include <locale>
#include "log.hpp"
#include "helper.h"

using namespace std;
fstream logfile;
wfstream wlogfile;
string LOGDIR="/var/tmp/skyerip";
bool open_log(){
	time_t secs = std::time(NULL);
    string fn = "skyerip-";
    fn = LOGDIR+"/"+fn+int_to_string(secs)+".log"; //this is jank, ideally I'd use UTC TIME, but THIS WORKS FOR THE PURPOSE OF NEVER NEEDING TO DELETE THE DAMN LOGS AGAIN.
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
	return 0;
}

void send_to_log_v2(vector<string> inputs){
	long unsigned int i=0; //shut -Wall -Wextra up.
	while(i<inputs.size()){
		logfile << inputs[i];
		i++;
	}
	logfile<<std::flush;
}

void send_to_log_v3(vector<std::variant<string,wstring>> inputs){
	std::setlocale(LC_ALL, "en_US.utf8");
	long unsigned int i=0;
	while (i<inputs.size()){
		string temp="";
		try{
			temp = std::get<string>(inputs[i]);
		}
		catch (const std::bad_variant_access&){ //FIXME: if there is a C++ way of doing this, avoiding C strings, replace this code with it.
			wstring temp1 = std::get<wstring>(inputs[i]); //based on https://stackoverflow.com/questions/25738497/character-conversion-using-iconv-without-the-unicode-byte-order-mark
			temp = wstring_to_string(temp1);
		}
		logfile << temp;
		i++;
	}
	logfile<<std::flush;
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
			
