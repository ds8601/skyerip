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
//define struct for general switches, updated 2026-08-08 21:30 CEST
struct sw_v2{
	bool set;
	bool in_mvar;
	std::string name;
	int type;
	std::string mvar;
	std::string excl;
	int min;
	int max;
	int val;
	int den;
};
//WORKAROUND FOR #define refusing to allow strings.
extern std::string PREFIX;
extern std::string PRESET_DIR;
extern std::string ENCDEF_DIR;
extern string vencpath;
extern string vid_chro;
extern vector<sw_v2> vencsw;
extern vector<sw_v2> aencsw;
extern string vou;
extern string vin;
#endif
