#ifndef DEFINES_H

#define DEFINES_H
//define current encoder preset version "0.0.1a"
#define CURENCP_MAJ 0
#define CURENCP_MIN 0
#define CURENCP_PAT 1
#define CURENCP_IND 'a'
//define current encoder definition version "0.0.1a"
#define CURENCD_MAJ 0
#define CURENCD_MIN 0
#define CURENCD_PAT 1
#define CURENCD_IND 'a'
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

extern string vencpath;
#endif
