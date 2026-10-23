//Skyerip: mi.cpp
// Separate file for all interactions with MediaInfoLIB. This is done to declutter main.cpp: int main().
// (c) 2026, Skye Wierzchowska (ds8601)
#define _UNICODE
#include <iostream>
#include <vector>
#include <MediaInfo/MediaInfo.h>
#include "helper.h"
#include "log.hpp"
string sfn = u8"mi.cpp";
//space for variables (placed as extern to use outside.
string ividstats = u8"";
string iaudstats = u8"";
vector<size_t> vidIDs;
vector<size_t> audIDs;
//end variables
int parseMediainfo(string fn){
	MediaInfoLib::MediaInfo mi;
	wstring mi_ver = mi.Option(L"Info_version", L"v26.05;SkyeRipCLI;0.0.1-devel");
	send_to_log_v3({mi_ver});
	mi.Option(L"Internet", L"No");
	mi.Open(wstring(begin(fn), end(fn)));
	mi.Option(L"Inform", L"General;%VideoCount%");
	wstring vidCountWstr = mi.Inform();
	size_t vidCnt = 0;
	size_t audCnt = 0;
	wchar_t * endptr;
	if(vidCountWstr == L"") return -1;
	else{
		vidCnt = wcstoul(vidCountWstr.c_str(), &endptr, 10);
		send_to_log_v3({sfn, " (parseMediainfo): Video tracks: ", to_string(vidCnt)});
		ividstats += u8"Video tracks: " + to_string(vidCnt) + u8"\n";
	}
	mi.Option(L"Inform", L"General;%AudioCount%");
	wstring audCountWstr = mi.Inform();
	if(audCountWstr == L"") audCnt = 0;
	else{
		audCnt = wcstoul(audCountWstr.c_str(), &endptr, 10);
	}
	send_to_log_v3({sfn, " (parseMediainfo): Audio tracks: ", to_string(audCnt)});
	iaudstats += u8"Audio tracks: " + to_string(audCnt) + u8"\n";
	mi.Option(L"Inform", L"Video;%ID%#%Format%#%Width%x%Height%#%FrameRate%#%FrameRate_Mode%#%BitRate%#%ScanType%#");
	string iVidMI = wstring_to_string(mi.Inform());
	send_to_log_v3({sfn, " (parseMediainfo): Video detail output: ", iVidMI});
	size_t prevsep = 0;
	size_t endptr2;
	string video_things[7] = {u8"ID:", u8" CODEC:", u8" RES:", u8" FPS:", u8" FPSMODE:", u8" BITRATE:", u8" IS_INT:"};
	for (int i=vidCnt; i>0; i--){
		size_t cursep;
		for(int j=0; j<7; j++){
			ividstats+=video_things[j];
			cursep = iVidMI.find(u8'#', prevsep);
				if(j==0){
					string temp = iVidMI.substr(prevsep, cursep-prevsep);
					size_t id = stoul(temp.c_str(), &endptr2, 10);
					vidIDs.push_back(id);
					ividstats+=to_string(id);
				}
				else{
					ividstats+=iVidMI.substr(prevsep, cursep-prevsep);
			}
			prevsep = cursep+1;
		}
		ividstats+=u8"\n";
	}
	send_to_log_v3({ividstats});
	//Audio stuff begins here
	mi.Option(L"Inform", L"Audio;%ID%#%Format%#%BitRate%#%BitRate_Mode%#%ChannelLayout%#%Delay%#");
	string iAudMI = wstring_to_string(mi.Inform());
	prevsep=0;
	string audio_things[6] = {u8"ID:", u8" CODEC:", u8" BITRATE:", u8" MODE:", u8" LAYOUT:", u8" DELAY:"};
	send_to_log_v3({sfn, " (parseMediainfo): Audio detail output: ", iAudMI});
	for (int i = audCnt; i>0; i--){
		size_t cursep;
		for(int j=0; j<6; j++){
			iaudstats+=audio_things[j];
			cursep = iAudMI.find(u8'#', prevsep);
			if(j==0){ //2026-10-23: clang++ doesn't allow this to be a switch(j) statement, so, enjoy yanderedev levels of code quality.

					string temp = iAudMI.substr(prevsep, cursep-prevsep);
					size_t id = stoul(temp.c_str(), &endptr2, 10);
					audIDs.push_back(id);
					iaudstats+=to_string(id);
			}
			else{
					iaudstats+=iAudMI.substr(prevsep, cursep-prevsep);
			}
			prevsep = cursep+1;
		}
		iaudstats+=u8"\n";
	}
	return 0;
}
