#ifndef HELPER_H
#define HELPER_H
using namespace std;
#include "common_includes.h"
bool locate_dots(string ver, unsigned int num, unsigned int *dots);
vector<long unsigned int> locate_char(string in, char find);
unsigned int uintInSubstr_to_uint(string substr);
int intInSubstr_to_int(string substr);
int check_definition_version(string version, int usage, uint *maj, uint *min, uint *pat, char *ind);
#endif
