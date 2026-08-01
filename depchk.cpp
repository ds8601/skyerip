#include <iostream>
#include "log.hpp"
using namespace std;

bool check_Deps(){
	bool return_num = true;
	bool p = true; //python
	int req_minvernum=12;
	FILE* console = popen("python3 --version", "r"); //THIS IS GARBAGE, THIS WILL COMPLAIN ABOUT REDEFINITIONS, A LOT.
	string tempstring = "";
	char python[20];
	//because of POSIX being heavily C, and my aversion to boost libs (past experiences on gentoo taught me to avoid them at all cost), this will be implemented with fgets. In a C++ program.
	if(fgets(python, 20, console) != NULL){
		pclose(console);
		for(int i =0; i< 6;i++){
			tempstring += python[i];
		}
		if(tempstring != "Python"){
			cout << "Python 3 is missing" << endl;
			return 1;
		}
		//If we're still runnning, that means Python 3.x is installed, and in path.
		//[7].[9][10] is enough.
		// TODO: Rewrite as parse_version_numbers();
		if(python[7] != '3'){
			cout << "python3 is NOT Python 3.x. What?" << endl;
			return 1;
		}
		//This is the correct way of doing this, maybe sub-optimal in the algorithmic sense, but way better than creating a 2 char array to store '1' and '2' to compare against.
		int sub=0;
		int c=1;
		while(python[c+8] != '.'){
			c++;
		}
		c--;
		int dec=1;
		for(;c>0;c--){
			sub = sub + ((int(python[c+8])-'0')*dec);
			dec = dec * 10;
		}
		if(sub>=req_minvernum){
			p = false;
		}
		//FIXME: char array substring needed for prettier log output.
		send_to_log_v2({"Detected ", python, " installed, OK \n"});
	}
	else { //sanity check
		pclose(console);
		send_to_log_v2({"depchk.cpp (check_deps): popen() failed or returned absolutely NOTHING. This is unexpected behaviour, returning \"TRUE\" (failing check)\n"});
		return true;
	}
	if(p == false){
		return_num = false;
	}
	return return_num;
}
