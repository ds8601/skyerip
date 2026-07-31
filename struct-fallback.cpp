//THIS IS UNUSED,
#include <iostream>
struct sw{
	bool set; //Don't include in final command if not set
	bool multi; //if multi then define list of vars that will be checked instead
	std::string multi_vars;
	int ln = -1; //Line number in .srps file where it's set. set to -1 by default.
	std::string name; //self-explanatory
	int valid_type; //0 - no vars, 1 - int or double, 2 - string (Won't be checked for validity)
	int valid_min; 
	int valid_max;
	int valid_den; //denominator, if int then it's set to 1, simple as that.
	int val;
	int den;
};
