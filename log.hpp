#ifndef LOG_H

#define LOG_H
using namespace std;
//extern fstream logfile;
bool open_log();
void send_to_log(string input);
void send_to_log_v2(int argc, char* argv[]);
void close_log();
string uint_to_string(int input);
#endif
