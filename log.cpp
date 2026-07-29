#include <iostream>
#include <string.h>
using namespace std;
fstream log;
bool open_log(){
    string logfile = "logfile.log";
    log.open(logfile, fstream::in);
    if(log.is_open()){
        cout << "FIXME: LOG ROTATION REQUIRED." << endl;
        return 1; //EXIT: LOGFILE EXISTS. COULD BE FIXED, BUT THAT'S FOR LATER.
    }
    log.close();
    log.open(logfile, fstream::out);

    return 0;
}

void send_to_log(string input){
    log << input;
}
