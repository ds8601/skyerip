#ifndef HELPER_H
#define HELPER_H
using namespace std;
bool locate_dots(string ver, unsigned int num, unsigned int *dots);
unsigned int uintInSubstr_to_uint(string substr);
int check_definition_version(string *version, int usage, uint *maj, uint *min, uint *pat, char *ind);
#endif
