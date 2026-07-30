#include <iostream>
#include "log.hpp"
using namespace std;

bool check_Deps(){
	bool return_num = true;
	bool p = true;
	int req_minvernum=12;
	FILE* console = popen("python3 --version", "r"); //THIS IS GARBAGE, THIS WILL COMPLAIN ABOUT REDEFINITIONS, A LOT.
	string tempstring = "";
	char python[20];
	if(fgets(python, sizeof python, console) != NULL){
		for(int i =0; i< 6;i++){
			tempstring += python[i];
		}
		if(tempstring != "Python"){
			cout << "Python 3 is missing" << endl;
			return 1;
		}
		//If we're still runnning, that means Python 3.x is installed, and in path.
		//[7].[9][10] is enough.
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
		send_to_log("Detected ");
		send_to_log(python);
		send_to_log(" installed, OK\n");
	}
	pclose(console);
	if(p == false){
		return_num = false;
	}
	return return_num;
}
