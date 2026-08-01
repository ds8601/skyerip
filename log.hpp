#ifndef LOG_H

#define LOG_H
#include <vector>
using namespace std;
bool open_log();
void send_to_log_v2(vector<string> inputs);
void close_log();
string uint_to_string(unsigned int input);
#endif
