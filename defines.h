#ifndef DEFINES_H

#define DEFINES_H
//idk why but g++ complains unless I make a temporary placeholder for the letter 'a'
char a = 'a';
//define current version "0.0.1a"
#define CURRENT_MAJ 0
#define CURRENT_MIN 0
#define CURRENT_PAT 1
#define CURRENT_IND a
//define struct used by preset.cpp (parse_preset) for maintaining a list of all possible (and used) encoder switches
struct sw{
	std::string name;
	bool multi;
	std::string multi_vars;
	bool set;
	unsigned int ln = -1;
	int valid_type;
	int valid_min;
	int valid_max;
	int valid_den;
	int val;
	int den;
};

#endif
