/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *  DirectDraw Wrapper                                                             *
   *  version 1.0, August 6th, 2010                                                  *
   *                                                                                 *
   *  Copyright (C) 2010 Jari Komppa                                                 *
   *                                                                                 *
   *  This software is provided 'as-is', without any express or implied              *
   *  warranty.  In no event will the authors be held liable for any damages         *
   *  arising from the use of this software.                                         *
   *                                                                                 *
   *  Permission is granted to anyone to use this software for any purpose,          *
   *  including commercial applications, and to alter it and redistribute it         *
   *  freely, subject to the following restrictions:                                 *
   *                                                                                 *
   *  1. The origin of this software must not be misrepresented; you must not        *
   *     claim that you wrote the original software. If you use this software        *
   *     in a product, an acknowledgment in the product documentation would be       *
   *     appreciated but is not required.                                            *
   *  2. Altered source versions must be plainly marked as such, and must not be     *
   *     misrepresented as being the original software.                              *
   *  3. This notice may not be removed or altered from any source distribution.     *
   *                                                                                 *
   *  Jari Komppa; http://iki.fi/sol/                                                *
   *                                                                                 *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                                                 *
   *  Based on a zlib/libpng licensed source code found on the net,                  *
   *  http://www.mikoweb.eu/index.php?node=28                                        *
   *  re-worked so much that there's fairly little left of the original.             *
   *                                                                                 *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                                                 *
   *  Modified by Doix - not the original wrapper                                    *
   *  More modifications made by Someone else                                        *
   *                                                                                 *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                                                 *
   *  Some modifications made by zort; https://github.com/zort/lf2-ai-scriptengine   *
   *  Cloned by Seiko Santana; https://github.com/seikosantana/lf2-ai-scriptengine   *
   *                                                                                 *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                                                 *
   *  More modifications and .cpp rearrangement by Mesujin.                          *
   *                                                                                 *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Includes
 #include "Includes\stdafx.h"
 #include "Includes\detours.h"
 #include "Includes\gameenums.h"
 #include "Includes\gamearrays.h"
 #include "Includes\Add-On (by zort)\scriptmath\scriptmath.h"
 #include "Includes\Add-On (by zort)\scriptarray\scriptarray.h"
 #include "Includes\Add-On (by zort)\scriptmath3d\scriptmath3d.h"
 #include "Includes\Add-On (by zort)\scriptbuilder\scriptbuilder.h"
 #include "Includes\Add-On (by zort)\scriptmath\scriptmathcomplex.h"
 #include "Includes\Add-On (by zort)\scriptstdstring\scriptstdstring.h"
 #include <map>
 #include <string>
 #include <fstream>
 #include <sstream>
 #include <iostream>
 #include <filesystem>
 #include <stdio.h>
 #include <direct.h>
 #include <string.h>
 #include <stdlib.h>
 #include <Windows.h>
 #include <WinUser.h>
 #include <varargs.h>
 #include <sys/stat.h>
 #include <shlobj_core.h>
//-//

//Global Variables
 #pragma data_seg (".ddraw_shared")
 HINSTANCE           gl_hOriginalDll;
 HINSTANCE           gl_hThisInstance;
 #pragma data_seg ()
//-//

//Variables
 namespace fs = std::filesystem;
 char PlayerName[15] = "Mesujin";
 int GameData[300];
 int mode;
 int difficulty;
 int elapsed_time;
 int background = -1;
 int bg_width;
 int bg_zwidth1;
 int bg_zwidth2;
 int stage_bound;
 int current_phase;
 int current_phase_count;
 int current_stage;
 char stage_clear;
 char VFPPath[MAX_PATH];
 unsigned int log_length[100];
 int log_Redudantcy;
 sGame* game = (sGame*)0x458b00;
 HANDLE hConsole;
 COORD coordScreen = {0, 0};
 DWORD cCharsWritten;
 CONSOLE_SCREEN_BUFFER_INFO csbi;
 DWORD dwConSize;
//-//

//Info
 class Info
 {
  public:
   int x; int y; int z;
   Vector3 position; double x_real; double y_real; double z_real;
   char facing;
   char holding_up; char holding_down; char holding_left; char holding_right; char holding_a; char holding_j; char holding_d;
   char up; char down; char left; char right; char A; char J; char D;
   char DrA; char DlA; char DuA; char DdA; char DrJ; char DlJ; char DuJ; char DdJ; char DJA;
   int hp; int dark_hp; int max_hp; int mp;
   int frame;
   int bdefend;
   int fall;
   int team;
   int id;
   int blink;
   int state;
   int weapon_type; int weapon_held; int weapon_holder;
   int shake;
   int wait_counter;
   int num;
   int ctimer;
   int arest;
   char vrests[400];
   int vrest;
   Vector3 velocity; double x_velocity; double y_velocity; double z_velocity;
   int clone;
   int type;
   int reserve;
   sDataFile* data;
   Info(){num = -1; type = -1; RefCount = 1;}
   Info(int object_num){Info(); operator=(object_num);}
   Info(const Info& info){Info(); operator=(info.num);}
   void AddRef(){RefCount++;}
   void RelRef(){if(--RefCount == 0) delete this;}
   int operator=(int object_num)
   {
	if(object_num < 0 || !game->exists[object_num]) return -1;
	x = game->objects[object_num]->x;
	y = game->objects[object_num]->y;
	z = game->objects[object_num]->z;
	x_real = game->objects[object_num]->x_real;
	y_real = game->objects[object_num]->y_real;
	z_real = game->objects[object_num]->z_real;
	position = Vector3(x_real, y_real, z_real);
	facing = game->objects[object_num]->facing;
	holding_up = game->objects[object_num]->holding_up;
	holding_down = game->objects[object_num]->holding_down;
	holding_left = game->objects[object_num]->holding_left;
	holding_right = game->objects[object_num]->holding_right;
	holding_a = game->objects[object_num]->holding_a;
	holding_j = game->objects[object_num]->holding_j;
	holding_d = game->objects[object_num]->holding_d;
	up = game->objects[object_num]->up;
	down = game->objects[object_num]->down;
	left = game->objects[object_num]->left;
	right = game->objects[object_num]->right;
	A = game->objects[object_num]->A;
	J = game->objects[object_num]->J;
	D = game->objects[object_num]->D;
	DrA = game->objects[object_num]->DrA;
	DlA = game->objects[object_num]->DlA;
	DuA = game->objects[object_num]->DuA;
	DdA = game->objects[object_num]->DdA;
	DrJ = game->objects[object_num]->DrJ;
	DlJ = game->objects[object_num]->DlJ;
	DuJ = game->objects[object_num]->DuJ;
	DdJ = game->objects[object_num]->DdJ;
	DJA = game->objects[object_num]->DJA;
	hp = game->objects[object_num]->hp;
	dark_hp = game->objects[object_num]->dark_hp;
	max_hp = game->objects[object_num]->max_hp;
	mp = game->objects[object_num]->mp;
	frame = game->objects[object_num]->frame1;
	fall = game->objects[object_num]->fall;
	bdefend = game->objects[object_num]->bdefend;
	team = game->objects[object_num]->team;
	id = game->objects[object_num]->data->id;
	blink = game->objects[object_num]->blink;
	state = game->objects[object_num]->data->frames[frame].state;
	weapon_type = game->objects[object_num]->weapon_type;
	if (weapon_type == 0) { weapon_held = -1; }
	else { weapon_held = game->objects[object_num]->weapon_held; }
	weapon_holder = game->objects[object_num]->weapon_holder;
	shake = game->objects[object_num]->shake;
	wait_counter = game->objects[object_num]->wait_counter;
	num = object_num;
	ctimer = game->objects[object_num]->ctimer;
	arest = game->objects[object_num]->arest;
	memcpy(vrests, game->objects[object_num]->vrests, 400);
	vrest = (int)(*(char*)(*(int*)(0x458c94 + object_num * 4) + 0xF0));
	if (vrest < (int)(*(char*)(*(int*)(0x458c94 + object_num * 4) + 0xF3))) vrest = (int)(*(char*)(*(int*)(0x458c94 + object_num * 4) + 0xF3));
	x_velocity = game->objects[object_num]->x_velocity;
	y_velocity = game->objects[object_num]->y_velocity;
	z_velocity = game->objects[object_num]->z_velocity;
	velocity = Vector3(x_velocity, y_velocity, z_velocity);
	clone = game->objects[object_num]->clone;
	type = game->objects[object_num]->data->type;
	reserve = game->objects[object_num]->reserve;
	data = game->objects[object_num]->data;
	return type;
   }
   int operator=(const Info& info){return operator=(info.num);}
   int operator+=(int object_num){return operator=(num + object_num);}
   int operator+=(const Info& info){return operator=(num + info.num);}
   int operator-=(int object_num){return operator=(num - object_num);}
   int operator-=(const Info& info){return operator=(num - info.num);}
   int operator++(){return operator=(num + 1);}
   int operator--(){return operator=(num - 1);}
  private:
   unsigned int RefCount;
 };
 Info* Info_Factory(){return new Info();}
 Info* Info_Factory(int object_num){return new Info(object_num);}
 Info* Info_Factory(const Info& info){return new Info(info);}
 Info self;
 Info target;
 asIScriptModule* ScriptModule;
 asIScriptEngine* ScriptEngine;
 asIScriptContext* ScriptContext;
//-//

//Main Functions
 std::map<int,FILETIME> ModuleTimes;
 typedef enum {INVALID, VALID, MODULE} FileValidity;
 void startup();
 void cleanup();
 int (__stdcall *AI_o)(int target_num, int object_num, int x, int y, int z, int a, int b);
 int (__stdcall *AIa_o)(int object_num, int unkwn1);

 bool mEsG = false;
 int dirExists(const char *path){struct stat info; if(stat(path, &info) != 0) return 0; else if(info.st_mode & S_IFDIR) return 1; else return 0;}
 void RepackDatIntoData(char DFPath[], char NFPath[])
 {
  std::ifstream OldData(DFPath);
  std::ofstream NewData(NFPath);
  if(OldData.is_open() && NewData.is_open())
  {
   std::string RTexting;
   std::string RTexting2;
   int NTexting = 0;
   int CenX = 0;
   int CenY = 0;
   int YAxisSingularity = 1000;
   NewData << "# LF2 Enchanted Data Re-Generator by Mesujin # https://github.com/Mesujin #\n";
   while(OldData){OldData >> RTexting; if(RTexting.compare("#Startl") == 0) goto ReTexting;}
   ReTexting:
   while(OldData)
   {
	OldData >> RTexting;
	RTexting2 = *RTexting.begin(); if(RTexting2.compare("#") == 0)
	{
	 if(RTexting.compare("##") == 0 ){getline(OldData, RTexting); goto CommandFound;}
     if(RTexting.compare("#Call:Pic+140") == 0){NewData << "file(0-140): sprite\\Other\\none.bmp w: 1 h: 1 row: 1 col: 140 "; goto CommandFound;}
	 if(RTexting.compare("#Call:PerfectBody") == 0){NewData << "bdy: kind: 0 x: " << CenX - 5 << " y: " << CenY - 10 << " w: 10 h: 10 bdy_end: "; goto CommandFound;}
	 if(RTexting.compare("#Endl") == 0) goto TextEnd;
	 CommandFound:
	 goto ReTexting;
	}
	if(RTexting.compare("<frame>") == 0){CenX = 0; CenY = 0; goto TextFound;}
	if(RTexting.compare("centerx:") == 0){NewData << RTexting << " "; OldData >> CenX; NewData << CenX << " "; goto ReTexting;}
	if(RTexting.compare("centery:") == 0){NewData << RTexting << " "; OldData >> CenY; NewData << CenY - YAxisSingularity << " "; goto ReTexting;}
	if(RTexting.compare("cpoint:") == 0){while(OldData){NewData << RTexting << " "; OldData >> RTexting; if(RTexting.compare("y:") == 0){NewData << RTexting << " "; OldData >> NTexting; NTexting -= YAxisSingularity; NewData << NTexting << " "; goto ReTexting;}} goto TextEnd;}
	if(RTexting.compare("wpoint:") == 0){while(OldData){NewData << RTexting << " "; OldData >> RTexting; if(RTexting.compare("y:") == 0){NewData << RTexting << " "; OldData >> NTexting; NTexting -= YAxisSingularity; NewData << NTexting << " "; goto ReTexting;}} goto TextEnd;}
	if(RTexting.compare("opoint:") == 0){while(OldData){NewData << RTexting << " "; OldData >> RTexting; if(RTexting.compare("y:") == 0){NewData << RTexting << " "; OldData >> NTexting; NTexting -= YAxisSingularity; NewData << NTexting << " "; goto ReTexting;}} goto TextEnd;}
	TextFound:
	NewData << RTexting << " ";
   }
   TextEnd:
   OldData.close();
   NewData << "\n# ======================================================================= #";
   NewData.close();
  }
 }
 enum SPNList {Openning, Prologue, FirstChapter, SecondChapter, ThirdChapter, FourthChapter, FifthChapter, SixthChapter, SeventhChapter, EighthChapter, NinthChapter, TenthChapter, LeeonRide, CostumStage01, CostumStage02, CostumStage03, CostumStage04, CostumStage05, CostumStage06, CostumStage07, CostumStage08};
 static std::map<std::string, SPNList> SPNLCheck;
 void SPNLInitalizing()
 {
  SPNLCheck["Openning"] = Openning;
  SPNLCheck["Prologue"] = Prologue;
  SPNLCheck["FirstChapter"] = FirstChapter;
  SPNLCheck["SecondChapter"] = SecondChapter;
  SPNLCheck["ThirdChapter"] = ThirdChapter;
  SPNLCheck["FourthChapter"] = FourthChapter;
  SPNLCheck["FifthChapter"] = FifthChapter;
  SPNLCheck["SixthChapter"] = SixthChapter;
  SPNLCheck["SeventhChapter"] = SeventhChapter;
  SPNLCheck["EighthChapter"] = EighthChapter;
  SPNLCheck["NinthChapter"] = NinthChapter;
  SPNLCheck["TenthChapter"] = TenthChapter;
  SPNLCheck["LeeonRide"] = LeeonRide;
  SPNLCheck["CostumStage01"] = CostumStage01;
  SPNLCheck["CostumStage02"] = CostumStage02;
  SPNLCheck["CostumStage03"] = CostumStage03;
  SPNLCheck["CostumStage04"] = CostumStage04;
  SPNLCheck["CostumStage05"] = CostumStage05;
  SPNLCheck["CostumStage06"] = CostumStage06;
  SPNLCheck["CostumStage07"] = CostumStage07;
  SPNLCheck["CostumStage08"] = CostumStage08;
 }
 void SPNLCReturn(std::string VarbSPNLCR)
 {
  switch (SPNLCheck[VarbSPNLCR])
  {
   case Openning: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\0 - Openning"); break;
   case Prologue: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\1 - Prologue"); break;
   case FirstChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\2 - First Chapter"); break;
   case SecondChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\3 - Second Chapter"); break;
   case ThirdChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\4 - Third Chapter"); break;
   case FourthChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\5 - Fourth Chapter"); break;
   case FifthChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\6 - Fifth Chapter"); break;
   case SixthChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\7 - Sixth Chapter"); break;
   case SeventhChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\8 - Seventh Chapter"); break;
   case EighthChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\9 - Eighth Chapter"); break;
   case NinthChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\10 - Ninth Chapter"); break;
   case TenthChapter: strcat(VFPPath, "Story Version's Parts\\1 - Main Story\\11 - Tenth Chapter"); break;
   case LeeonRide: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\1 - Lee on Ride"); break;
   case CostumStage01: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\11 - Costum Stage 01"); break;
   case CostumStage02: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\12 - Costum Stage 02"); break;
   case CostumStage03: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\13 - Costum Stage 03"); break;
   case CostumStage04: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\14 - Costum Stage 04"); break;
   case CostumStage05: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\15 - Costum Stage 05"); break;
   case CostumStage06: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\16 - Costum Stage 06"); break;
   case CostumStage07: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\17 - Costum Stage 07"); break;
   case CostumStage08: strcat(VFPPath, "Story Version's Parts\\3 - Extras\\18 - Costum Stage 08"); break;
   default: GetCurrentDirectory(MAX_PATH, VFPPath); for(size_t Varb929 = strlen(VFPPath) - 9; Varb929 < strlen(VFPPath); ++Varb929){VFPPath[Varb929] = '\0';} strcat(VFPPath, "\\Set-up\\Story Version\\Slots"); break;
  }
 }
 void VerDatClassic(char gamePath[])
 {
  char classicPath[MAX_PATH];
  char toPath[MAX_PATH];
  std::string verChecking;

  strcpy(classicPath, gamePath);
  strcat(classicPath, "\\Set-up\\Base Database\\Database\\data\\23.as");
  std::ifstream AInSystemRebuild(classicPath);
  if(AInSystemRebuild.is_open())
  {
   strcpy(toPath, gamePath);
   strcat(toPath, "\\Database\\data\\23.as");
   std::ofstream RebuildingAInSystem(toPath);
   char verCheckout[10000];

   while(AInSystemRebuild){getline(AInSystemRebuild, verChecking); RebuildingAInSystem << verChecking << "\n"; if(verChecking.compare("//Main System") == 0) goto RebuildSystem;}
   RebuildSystem:
   while(AInSystemRebuild){AInSystemRebuild >> verCheckout; if(strcmp(verCheckout, "//MainEnd") == 0) goto RebuildEnd; verChecking = verCheckout[0]; verChecking += verCheckout[1]; if(verChecking.compare("//") == 0){getline(AInSystemRebuild, verChecking); goto RebuildSystem;} RebuildingAInSystem << verCheckout << " ";}
   RebuildEnd:
   RebuildingAInSystem << "\n//-//";

   RebuildingAInSystem.close();
   AInSystemRebuild.close();
  }

  strcpy(classicPath, gamePath);
  strcat(classicPath, "\\Database\\ver.txt");
  std::ifstream verCheck(classicPath);
  if(verCheck.is_open())
  {
   verCheck >> verChecking; if(verChecking.compare("classic") == 0){verCheck.close(); return;}
  }
  strcpy(classicPath, gamePath);
  strcat(classicPath, "\\Set-up\\Base Database\\Database");
  strcpy(toPath, gamePath);
  strcat(toPath, "\\Database");
  const auto copyOptions = fs::copy_options::overwrite_existing | fs::copy_options::recursive;
  fs::copy(classicPath, toPath, copyOptions);
  strcpy(classicPath, gamePath);
  strcat(classicPath, "\\Set-up\\Classic Version\\Database");
  fs::copy(classicPath, toPath, copyOptions);
  strcpy(classicPath, gamePath);
  strcat(classicPath, "\\Database\\ver.txt");
  std::ofstream verInit(classicPath);
  verInit << "classic";
  verInit.close();
 }
 void VerDatStory(char gamePath[])
 {
  char storyPath[MAX_PATH];
  char toPath[MAX_PATH];
  std::string verChecking;
  strcpy(storyPath, gamePath);
  strcat(storyPath, "\\Database\\ver.txt");
  std::ifstream verCheck(storyPath);
  if(verCheck.is_open())
  {
   verCheck >> verChecking; if(verChecking.compare("story") == 0){verCheck.close(); return;}
  }
  strcpy(storyPath, gamePath);
  strcat(storyPath, "\\Set-up\\Base Database\\Database");
  strcpy(toPath, gamePath);
  strcat(toPath, "\\Database");
  const auto copyOptions = fs::copy_options::overwrite_existing | fs::copy_options::recursive;
  fs::copy(storyPath, toPath, copyOptions);
  strcpy(storyPath, gamePath);
  strcat(storyPath, "\\Set-up\\Story Version\\Database");
  fs::copy(storyPath, toPath, copyOptions);
  strcpy(storyPath, gamePath);
  strcat(storyPath, "\\Database\\ver.txt");
  std::ofstream verInit(storyPath);
  verInit << "story";
  verInit.close();

  strcpy(storyPath, gamePath);
  strcat(storyPath, "\\Set-up\\Base Database\\Database\\data\\23.as");
  std::ifstream AInSystemRebuild(storyPath);
  if(AInSystemRebuild.is_open())
  {
   strcpy(toPath, gamePath);
   strcat(toPath, "\\data");
   std::ofstream RebuildingAInSystem(toPath);
   char verCheckout[10000];

   while(AInSystemRebuild){getline(AInSystemRebuild, verChecking); RebuildingAInSystem << verChecking << "\n"; if(verChecking.compare("//Main System") == 0) goto RebuildSystem;}
   RebuildSystem:
   while(AInSystemRebuild){AInSystemRebuild >> verCheckout; if(strcmp(verCheckout, "//MainEnd") == 0) goto RebuildEnd; verChecking = verCheckout[0]; verChecking += verCheckout[1]; if(verChecking.compare("//") == 0) goto RebuildSystem; RebuildingAInSystem << verChecking << " ";}
   RebuildEnd:
   RebuildingAInSystem << "\n//-//";

   RebuildingAInSystem.close();
   AInSystemRebuild.close();
  }
 }
 void VersionControl()
 {
  char gamePath[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, gamePath);
  for(size_t Varb929 = strlen(gamePath) - 9; Varb929 < strlen(gamePath); ++Varb929){gamePath[Varb929] = '\0';}
  char VFCPath[MAX_PATH];
  char VFPath[MAX_PATH];
  strcpy(VFPath, gamePath); strcat(VFPath, "\\readme.txt");
  strcpy(VFCPath, gamePath); strcat(VFCPath, "\\Database");
  VFCRetry:
  std::ifstream VerData(VFPath);
  if(VerData.is_open())
  {
   std::string VDText;
   std::string VDText2;
   std::string VDText3;
   std::string VDCheck;
   int GetData = 0;
   while(VerData){VerData >> VDText; if(VDText.compare("Version:") == 0){GetData = 1; break;}}
   if(GetData == 0) goto VFCRemake;
   VerData >> VDText; if(VDText.compare("Classic") == 0){VerDatClassic(gamePath);} else {if(VDText.compare("Story") == 0){GetData = 2;} else {goto VFCRemake;}}
   if(GetData == 2)
   {
	VerDatStory(gamePath); SPNLInitalizing();
	const auto copyOptions = fs::copy_options::overwrite_existing | fs::copy_options::recursive;
	while(VerData){VerData >> VDText; if(VDText.compare("Slot1:") == 0){GetData = -1; break;}}
	VerData >> VDText >> VDText2; if(VDText2.compare("Slot2:") != 0){GetData = -2; VerData >> VDText3; if(VDText3.compare("Slot2:") != 0){GetData = -3;}}
	strcpy(VFPPath, gamePath);
	strcat(VFPPath, "\\Set-up\\");
	if(GetData <= -1) VDCheck = VDText;
	if(GetData <= -2) VDCheck += VDText2;
	if(GetData <= -3) VDCheck += VDText3;
	SPNLCReturn(VDCheck); strcat(VFPPath, "\\Slot 1\\Database");
	fs::copy(VDCheck, VFCPath, copyOptions);
	if(GetData == 2){GetData = 0; goto GetDataEnd;}
	if(GetData <= -3){GetData = 2; while(VerData){VerData >> VDText; if(VDText.compare("Slot2:") == 0){GetData = -1; break;}}} else {GetData = -1;}
	VerData >> VDText >> VDText2; if(VDText2.compare("Slot3:") != 0){GetData = -2; VerData >> VDText3; if(VDText3.compare("Slot3:") != 0){GetData = -3;}}
	strcpy(VFPPath, gamePath);
	strcat(VFPPath, "\\Set-up\\");
	if(GetData <= -1) VDCheck = VDText;
	if(GetData <= -2) VDCheck += VDText2;
	if(GetData <= -3) VDCheck += VDText3;
	SPNLCReturn(VDCheck); strcat(VFPPath, "\\Slot 2\\Database");
	fs::copy(VDCheck, VFCPath, copyOptions);
	if(GetData == 2){GetData = 0; goto GetDataEnd;}
	if(GetData <= -3){GetData = 2; while(VerData){VerData >> VDText; if(VDText.compare("Slot3:") == 0){GetData = -1; break;}}} else {GetData = -1;}
	VerData >> VDText >> VDText2; if(VDText2.compare("Slot4:") != 0){GetData = -2; VerData >> VDText3; if(VDText3.compare("Slot4:") != 0){GetData = -3;}}
	strcpy(VFPPath, gamePath);
	strcat(VFPPath, "\\Set-up\\");
	if(GetData <= -1) VDCheck = VDText;
	if(GetData <= -2) VDCheck += VDText2;
	if(GetData <= -3) VDCheck += VDText3;
	SPNLCReturn(VDCheck); strcat(VFPPath, "\\Slot 3\\Database");
	fs::copy(VDCheck, VFCPath, copyOptions);
	if(GetData == 2){GetData = 0; goto GetDataEnd;}
	if(GetData <= -3){GetData = 2; while(VerData){VerData >> VDText; if(VDText.compare("Slot4:") == 0){GetData = -1; break;}}} else {GetData = -1;}
	VerData >> VDText >> VDText2; if(VDText2.compare("Slot5:") != 0){GetData = -2; VerData >> VDText3; if(VDText3.compare("Slot5:") != 0){GetData = -3;}}
	strcpy(VFPPath, gamePath);
	strcat(VFPPath, "\\Set-up\\");
	if(GetData <= -1) VDCheck = VDText;
	if(GetData <= -2) VDCheck += VDText2;
	if(GetData <= -3) VDCheck += VDText3;
	SPNLCReturn(VDCheck); strcat(VFPPath, "\\Slot 4\\Database");
	fs::copy(VDCheck, VFCPath, copyOptions);
	if(GetData == 2){GetData = 0; goto GetDataEnd;}
	if(GetData <= -3){GetData = 2; while(VerData){VerData >> VDText; if(VDText.compare("Slot5:") == 0){GetData = -1; break;}}} else {GetData = -1;}
	VerData >> VDText >> VDText2; if(VDText2.compare("Slot6:") != 0){GetData = -2; VerData >> VDText3; if(VDText3.compare("Slot6:") != 0){GetData = -3;}}
	strcpy(VFPPath, gamePath);
	strcat(VFPPath, "\\Set-up\\");
	if(GetData <= -1) VDCheck = VDText;
	if(GetData <= -2) VDCheck += VDText2;
	if(GetData <= -3) VDCheck += VDText3;
	SPNLCReturn(VDCheck); strcat(VFPPath, "\\Slot 5\\Database");
	fs::copy(VDCheck, VFCPath, copyOptions);
	if(GetData == 2){GetData = 0; goto GetDataEnd;}
	if(GetData <= -3){GetData = 2; while(VerData){VerData >> VDText; if(VDText.compare("Slot6:") == 0){GetData = -1; break;}}} else {GetData = -1;}
	VerData >> VDText >> VDText2; if(VDText2.compare("Slot7:") != 0){GetData = -2; VerData >> VDText3; if(VDText3.compare("Slot7:") != 0){GetData = -3;}}
	strcpy(VFPPath, gamePath);
	strcat(VFPPath, "\\Set-up\\");
	if(GetData <= -1) VDCheck = VDText;
	if(GetData <= -2) VDCheck += VDText2;
	if(GetData <= -3) VDCheck += VDText3;
	SPNLCReturn(VDCheck); strcat(VFPPath, "\\Slot 6\\Database");
	fs::copy(VDCheck, VFCPath, copyOptions);
	if(GetData == 2){GetData = 0; goto GetDataEnd;}
	if(GetData <= -3){GetData = 2; while(VerData){VerData >> VDText; if(VDText.compare("Slot7:") == 0){GetData = -1; break;}}} else {GetData = -1;}
	VerData >> VDText >> VDText2; if(VDText2.compare("Slot8:") != 0){GetData = -2; VerData >> VDText3; if(VDText3.compare("Slot8:") != 0){GetData = -3;}}
	strcpy(VFPPath, gamePath);
	strcat(VFPPath, "\\Set-up\\");
	if(GetData <= -1) VDCheck = VDText;
	if(GetData <= -2) VDCheck += VDText2;
	if(GetData <= -3) VDCheck += VDText3;
	SPNLCReturn(VDCheck); strcat(VFPPath, "\\Slot 7\\Database");
	fs::copy(VDCheck, VFCPath, copyOptions);
	if(GetData == 2){GetData = 0; goto GetDataEnd;}
	if(GetData <= -3){GetData = 2; while(VerData){VerData >> VDText; if(VDText.compare("Slot8:") == 0){GetData = -1; break;}}} else {GetData = -1;}
	VerData >> VDText >> VDText2; if(VDText2.compare("=========================") != 0){GetData = -2; VerData >> VDText3; if(VDText3.compare("=========================") != 0){GetData = -3;}}
	strcpy(VFPPath, gamePath);
	strcat(VFPPath, "\\Set-up\\");
	if(GetData <= -1) VDCheck = VDText;
	if(GetData <= -2) VDCheck += VDText2;
	if(GetData <= -3) VDCheck += VDText3;
	SPNLCReturn(VDCheck); strcat(VFPPath, "\\Slot 8\\Database");
	fs::copy(VDCheck, VFCPath, copyOptions);
	if(GetData == 2){GetData = 0; goto GetDataEnd;}
   }
   GetDataEnd:
   if(GetData == 0) goto VFCRemake;
   VerData.close();
  } else
  {
   VFCRemake:
   std::ofstream NVerData(VFPath);
   NVerData << "=====================================================================================================\n                                   Little Fighter 2 Enchanted 3rd\n                                 A Modification of Little Fighter 2\n\n                                Version : 0.81.220924 - Release Ver.\n                          All rights belong to Marti Wong and Starsky Wong\n=====================================================================================================\n\n!-!-!-!-!-!-!-!-!-!- Only the Classic Version that was available in v0.81.220924 -!-!-!-!-!-!-!-!-!-!\n\n How to set-up the version :\n\n • Set To Classic Version\n   Simply change the name of the 'Version' in the \"Setting\" into \"Classic\".\n\n • Set To Story Version\n   Simply change the name of the 'Version' in the \"Setting\" into \"Story\". Plus set the 'Slot' with\n   the sub-stage that you're gonna play (LF2 has a limited memory, that's why it should be sparated),\n   just insert the name (make sure the lettercase are right) of the sub-stage without it's number,\n   see the example. You can see the list of the sub-stage inside \"Story Version's Parts\" folder in the\n   \"Set-up\" folder.\n\n =========Setting=========\n Version: Classic\n Slot1: Openning\n Slot2: First Chapter\n Slot3: none\n Slot4: none\n Slot5: none\n Slot6: none\n Slot7: none\n Slot8: none\n =========================\n\n If the game error while loading, try to not fulfill all the slots or remove some of the R-Series\n character (since some of the R-Series character cost alot of memory).\n Please do not cheating and play wisely, cheating the progress will just kill the fun of this mod.\n\n=============================================== Credits =============================================\n\n Original LF2 by Marti Wong & Starsky Wong\n Background Musics - BRS The Game's BGM\n Easier Data-Editor (STM93 Version) by Luigi600\n ddraw.dll for AI by Lord Silva(Boop), Som1Lse, zort, and everyone else who take an effort in it.\n Modded Executable(.exe) by darkfiren\n Some Cool Sprites => Little Fighter 2 neuer Stern by luminance\n Some Cool Sprites => LF2: Another Dimension by VicAdward\n Advice ; STM93 and ppl in LFE's Discord.\n\n More credits can be found in the Thread of this mod.\n | https://lf-empire.de/forum/showthread.php?tid=11058 |\n\n=====================================================================================================";
   NVerData.close();
   goto VFCRetry;
  }
 }
 void StartDataControl()
 {
  char gamePath[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, gamePath);
  for(size_t Varb929 = strlen(gamePath) - 9; Varb929 < strlen(gamePath); ++Varb929)
  {
   gamePath[Varb929] = '\0';
  }
  char RPath[MAX_PATH];
  char RFPath01[MAX_PATH];
  char RFPath02[MAX_PATH];
  char RFPath03[MAX_PATH];
  char RFPath04[MAX_PATH];
  char RFPath05[MAX_PATH];
  char RFPath06[MAX_PATH];
  char RFPath07[MAX_PATH];
  char RFPath08[MAX_PATH];
  char RFPath09[MAX_PATH];
  char RFPath10[MAX_PATH];
  char RFPath11[MAX_PATH];
  char RFPath12[MAX_PATH];
  char RFPath13[MAX_PATH];
  char RFPath14[MAX_PATH];
  char RFPath15[MAX_PATH];
  char RFPath16[MAX_PATH];
  char RFPath17[MAX_PATH];
  char RFPath18[MAX_PATH];
  char RFPath19[MAX_PATH];
  char RFPath20[MAX_PATH];
  strcpy(RPath, gamePath); strcat(RPath, "\\R-Series");
  int RelocateTimes = 0;
  int NumberOfDatas = 0;
  int RNText;
  std::string RText;
  std::string RText2;
  std::string RText3;
  std::string RedynamicEntry;
  if(dirExists(RPath) == 1) for(const auto& DynamicEntry : fs::directory_iterator(RPath))
  {
   char RPaths[MAX_PATH];
   strcpy(RPaths, DynamicEntry.path().string().c_str());
   strcat(RPaths, "\\instalation.txt");
   std::ifstream RIns(RPaths);
   if(RIns.is_open())
   {
	switch (RelocateTimes)
	{
	 case 0: strcpy(RFPath01, RPaths); break;
	 case 1: strcpy(RFPath02, RPaths); break;
	 case 2: strcpy(RFPath03, RPaths); break;
	 case 3: strcpy(RFPath04, RPaths); break;
	 case 4: strcpy(RFPath05, RPaths); break;
	 case 5: strcpy(RFPath06, RPaths); break;
	 case 6: strcpy(RFPath07, RPaths); break;
	 case 7: strcpy(RFPath08, RPaths); break;
	 case 8: strcpy(RFPath09, RPaths); break;
	 case 9: strcpy(RFPath10, RPaths); break;
	 case 10: strcpy(RFPath11, RPaths); break;
	 case 11: strcpy(RFPath12, RPaths); break;
	 case 12: strcpy(RFPath13, RPaths); break;
	 case 13: strcpy(RFPath14, RPaths); break;
	 case 14: strcpy(RFPath15, RPaths); break;
	 case 15: strcpy(RFPath16, RPaths); break;
	 case 16: strcpy(RFPath17, RPaths); break;
	 case 17: strcpy(RFPath18, RPaths); break;
	 case 18: strcpy(RFPath19, RPaths); break;
	 case 19: strcpy(RFPath20, RPaths); break;
	 default: RelocateTimes = -1; break;
	}
	RelocateTimes += 1;
	while(RIns)
	{
	 RIns >> RText; if(RText.compare("<file>") == 0)
	 {
	  while(RIns)
	  {
	   while(RIns){RIns >> RText; if(RText.compare("locate:") == 0) break; if(RText.compare("<file_end>") == 0) goto REnd;}
	   RIns >> RText;
	   while(RIns){RIns >> RText2; if(RText2.compare("to:") == 0) break;}
	   RIns >> RText2;
	   while(RIns){RIns >> RText3; if(RText3.compare("filename:") == 0) break;}
	   RIns >> RText3;
	   char RFC1[MAX_PATH];
	   strcpy(RFC1, gamePath);
	   strcat(RFC1, "\\R-Series\\");
	   strcat(RFC1, RText.c_str());
	   char RFC2[MAX_PATH];
	   strcpy(RFC2, gamePath);
	   strcat(RFC2, "\\Database\\");
	   strcat(RFC2, RText2.c_str());
	   char RFC3[MAX_PATH];
	   strcpy(RFC3, RFC2);
	   strcat(RFC3, "\\");
	   strcat(RFC3, RText3.c_str());
	   if (dirExists(RFC2) == 0) fs::create_directories(RFC2);
	   const auto copyOptions = fs::copy_options::update_existing;
	   fs::copy_file(RFC1, RFC3, copyOptions);
	  }
	 }
	}
	REnd:
	while(RIns){RIns >> RText; if(RText.compare("<data>") == 0) break;}
	while(RIns){RIns >> RText; if(RText.compare("datas:") == 0) break;}
	RIns >> RNText;
    NumberOfDatas += RNText;
	RIns.close();
   }
  }
  char DataPath[MAX_PATH];
  strcpy(DataPath, gamePath); strcat(DataPath, "\\Database\\data\\oridata.txt");
  std::ifstream OData(DataPath);
  if(OData.is_open() == false) return;
  strcpy(DataPath, gamePath);
  strcat(DataPath, "\\Database\\data\\data.txt");
  std::ofstream NData(DataPath);
  if(NData.is_open() == false) return;
  while(OData){OData >> RText; if(RText.compare("<data>") == 0) break;}
  NData << "<object>\n";
  while(OData){OData >> RText; if(RText.compare("datas:") == 0) break;}
  OData >> RNText;
  NumberOfDatas += RNText; int CNOData = 1;
  DRetry:
  while(OData){OData >> RText; if(RText.compare("#r-series") == 0) goto DRSeries; RText2 = *RText.begin(); if(RText2.compare("#") == 0) goto DRetry; if(RText.compare("<data_end>") == 0) goto DEnd; NData << RText << " "; if(RText.compare("file:") == 0) break;}
  OData >> RText;
  strcpy(DataPath, gamePath);
  strcat(DataPath, "\\Database\\");
  if(CNOData < 10){strcat(DataPath, "0"); NData << "0";}
  if(CNOData < 100){strcat(DataPath, "0"); NData << "0";}
  strcat(DataPath, std::to_string(CNOData).c_str()); NData << CNOData;
  CreateDirectory(DataPath, NULL);
  CNOData += 1;
  strcat(DataPath, "\\"); NData << "\\";
  if(NumberOfDatas < 10){strcat(DataPath, "0"); NData << "0";}
  if(NumberOfDatas < 100){strcat(DataPath, "0"); NData << "0";}
  strcat(DataPath, std::to_string(NumberOfDatas).c_str()); NData << NumberOfDatas;
  strcat(DataPath, ".data"); NData << ".data\n";
  char RDID[MAX_PATH];
  strcpy(RDID, RText.c_str());
  RepackDatIntoData(RDID, DataPath);
  goto DRetry;
  DEnd:
  NData << "<object_end>\n\n<file_editing>\ndata\\nothing.txt\n<file_editing_end>\n\n<background>\nid: 1  file: data\\map_coliseum.dat\nid: 2  file: data\\map_lion_forest.dat\nid: 3  file: data\\map_prison.dat\nid: 4  file: data\\map_great_wall.dat\nid: 5  file: data\\map_magical_island.dat\nid: 6  file: data\\map_fortress.dat\nid: 7  file: data\\map_akira's_warship.dat\nid: 8  file: data\\map_cave.dat\nid: 9  file: data\\map_starry_dimension.dat\nid: 10 file: data\\map_elq's_dimension.dat\nid: 11 file: data\\map_fortress'_gate.dat\nid: 12 file: data\\map_swampy.dat\nid: 13 file: data\\map_dead_village.dat\nid: 14 file: data\\map_main_city.dat\nid: 15 file: data\\map_ex-preset.dat\nid: 16 file: data\\map_ex-costum.dat\n<background_end>\n\nid: 100~199 drop weapon";
  OData.close();
  NData.close();
  return;
  DRSeries:
  int RFNum = 10;
  if(strlen(RFPath01) != 0){RFNum = 0; goto RDataSet;}
  RFile02: if (strlen(RFPath02) != 0){RFNum = 1; goto RDataSet;}
  RFile03: if (strlen(RFPath03) != 0){RFNum = 2; goto RDataSet;}
  RFile04: if (strlen(RFPath04) != 0){RFNum = 3; goto RDataSet;}
  RFile05: if (strlen(RFPath05) != 0){RFNum = 4; goto RDataSet;}
  RFile06: if (strlen(RFPath06) != 0){RFNum = 5; goto RDataSet;}
  RFile07: if (strlen(RFPath07) != 0){RFNum = 6; goto RDataSet;}
  RFile08: if (strlen(RFPath08) != 0){RFNum = 7; goto RDataSet;}
  RFile09: if (strlen(RFPath09) != 0){RFNum = 8; goto RDataSet;}
  RFile10: if (strlen(RFPath10) != 0){RFNum = 9; goto RDataSet;}
  RFile11: if (strlen(RFPath11) != 0){RFNum = 10; goto RDataSet;}
  RFile12: if (strlen(RFPath12) != 0){RFNum = 11; goto RDataSet;}
  RFile13: if (strlen(RFPath13) != 0){RFNum = 12; goto RDataSet;}
  RFile14: if (strlen(RFPath14) != 0){RFNum = 13; goto RDataSet;}
  RFile15: if (strlen(RFPath15) != 0){RFNum = 14; goto RDataSet;}
  RFile16: if (strlen(RFPath16) != 0){RFNum = 15; goto RDataSet;}
  RFile17: if (strlen(RFPath17) != 0){RFNum = 16; goto RDataSet;}
  RFile18: if (strlen(RFPath18) != 0){RFNum = 17; goto RDataSet;}
  RFile19: if (strlen(RFPath19) != 0){RFNum = 18; goto RDataSet;}
  RFile20: if (strlen(RFPath20) != 0){RFNum = 19; goto RDataSet;}
  goto DRetry;
  RDataSet:
  char RDataPath[MAX_PATH];
  switch (RFNum)
  {
   case 0: strcpy(RDataPath, RFPath01); break;
   case 1: strcpy(RDataPath, RFPath02); break;
   case 2: strcpy(RDataPath, RFPath03); break;
   case 3: strcpy(RDataPath, RFPath04); break;
   case 4: strcpy(RDataPath, RFPath05); break;
   case 5: strcpy(RDataPath, RFPath06); break;
   case 6: strcpy(RDataPath, RFPath07); break;
   case 7: strcpy(RDataPath, RFPath08); break;
   case 8: strcpy(RDataPath, RFPath09); break;
   case 9: strcpy(RDataPath, RFPath10); break;
   case 10: strcpy(RDataPath, RFPath11); break;
   case 11: strcpy(RDataPath, RFPath12); break;
   case 12: strcpy(RDataPath, RFPath13); break;
   case 13: strcpy(RDataPath, RFPath14); break;
   case 14: strcpy(RDataPath, RFPath15); break;
   case 15: strcpy(RDataPath, RFPath16); break;
   case 16: strcpy(RDataPath, RFPath17); break;
   case 17: strcpy(RDataPath, RFPath18); break;
   case 18: strcpy(RDataPath, RFPath19); break;
   case 19: strcpy(RDataPath, RFPath20); break;
  }
  std::ifstream RDataO(RDataPath);
  if(RDataO.is_open())
  {
   while(RDataO){RDataO >> RText; if(RText.compare("<data>") == 0) break;}
   while(RDataO){RDataO >> RText; if(RText.compare("id:") == 0) break;}
   NData << RText << " ";
   RDRetry:
   while(RDataO){RDataO >> RText; RText2 = *RText.begin(); if(RText2.compare("#") == 0) goto RDRetry; if(RText.compare("<data_end>") == 0) goto RDEnd; NData << RText; NData << " "; if(RText.compare("file:") == 0) break;}
   RDataO >> RText;
   strcpy(DataPath, gamePath);
   strcat(DataPath, "\\Database\\");
   if(CNOData < 10){strcat(DataPath, "0"); NData << "0";}
   if(CNOData < 100){strcat(DataPath, "0"); NData << "0";}
   strcat(DataPath, std::to_string(CNOData).c_str()); NData << CNOData;
   CreateDirectory(DataPath, NULL);
   CNOData += 1;
   strcat(DataPath, "\\"); NData << "\\";
   if(NumberOfDatas < 10){strcat(DataPath, "0"); NData << "0";}
   if(NumberOfDatas < 100){strcat(DataPath, "0"); NData << "0";}
   strcat(DataPath, std::to_string(NumberOfDatas).c_str()); NData << NumberOfDatas;
   strcat(DataPath, ".txt"); NData << ".txt\n";
   strcpy(RDID, RText.c_str());
   RepackDatIntoData(RDID, DataPath);
   goto RDRetry;
   RDEnd:
   RDataO.close();
  }
  switch (RFNum)
  {
   case 0: goto RFile02; break;
   case 1: goto RFile03; break;
   case 2: goto RFile04; break;
   case 3: goto RFile05; break;
   case 4: goto RFile06; break;
   case 5: goto RFile07; break;
   case 6: goto RFile08; break;
   case 7: goto RFile09; break;
   case 8: goto RFile10; break;
   case 9: goto RFile11; break;
   case 10: goto RFile12; break;
   case 11: goto RFile13; break;
   case 12: goto RFile14; break;
   case 13: goto RFile15; break;
   case 14: goto RFile16; break;
   case 15: goto RFile17; break;
   case 16: goto RFile18; break;
   case 17: goto RFile19; break;
   case 18: goto RFile20; break;
   default: goto DRetry; break;
  }
 }
 int DataControl(int Varb)
 {
  char fischl[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, fischl);
  for(size_t Varb929 = strlen(fischl) - 9; Varb929 < strlen(fischl); ++Varb929){fischl[Varb929] = '\0';}
  if(Varb != 0)
  {
   switch (abs(Varb))
   {
	case 1: strcat(fischl, "\\Datasave01.data"); break;
	case 2: strcat(fischl, "\\Datasave02.data"); break;
	case 3: strcat(fischl, "\\Datasave03.data"); break;
	case 4: strcat(fischl, "\\Datasave04.data"); break;
	case 5: strcat(fischl, "\\Datasave05.data"); break;
	case 6: strcat(fischl, "\\Datasave06.data"); break;
	case 7: strcat(fischl, "\\Datasave07.data"); break;
	case 8: strcat(fischl, "\\Datasave08.data"); break;
	case 9: strcat(fischl, "\\Datasave09.data"); break;
	case 10: strcat(fischl, "\\Autosave.data"); break;
   }
   if(Varb < 0)
   {
	std::ofstream file_(fischl);
	if(file_.is_open())
	{
	 file_ << PlayerName << " ";
	 for(int Varb991 = 0; Varb991 < 300; ++Varb991) file_ << GameData[Varb991] << " ";
	 file_.close();
    }
   }
   if (Varb > 0)
   {
    std::ifstream file_(fischl);
    if(file_.is_open())
    {
	 file_ >> PlayerName; 
	 for(int Varb991 = 0; Varb991 < 300; ++Varb991) file_ >> GameData[Varb991];
 	 file_.close();
    }
   }
  }
  return 0;
 }
 /////////////////////////////

 /////////////////////////////
 void LoadingImg(bool Typen)
 {
  if(Typen)
  {
  } else
  {
  
  }
 }
//-//

//Functions
 int random(int max) //'Random' function that works with replays
 {
  int result;    // eax@2
  signed int v3; // eax@3
  DWORD &dword_450C34 = *((DWORD *)0x450C34);
  DWORD &dword_450BCC = *((DWORD *)0x450BCC);
  if(max > 0)
  {
   dword_450C34 = (dword_450C34 + 1) % 1234;
   v3 = dword_450C34 + (unsigned __int8)*(((BYTE *)0x44FF90) + (dword_450BCC + 1) % 3000);
   dword_450BCC = (dword_450BCC + 1) % 3000;
   result = v3 % max;
  } else
  {
   result = 0;
  }
  return result;
 }
 int loadTarget(int target_num){return target = target_num;}
 int setEnemy(int object_num)
 {
  if(object_num < 0 || object_num >= 400 || !game->exists[object_num]) return -1;
  return game->objects[self.num]->enemy = object_num;
 }
 int Camera(){return *(int*)0x450bc4;}
 int Cnvrt_F_I(float vit){return static_cast<int>(vit);}
 int Cnvrt_D_I(double vit){return static_cast<int>(vit);}
 double Cnvrt_F_D(float vit){return static_cast<double>(vit);}
 double Cnvrt_I_D(int vit){return static_cast<double>(vit);}
 float Cnvrt_D_F(double vit){return static_cast<float>(vit);}
 float Cnvrt_I_F(int vit){return static_cast<float>(vit);}

 void A(int vit, char key, char holding)    {game->objects[vit]->A = key; game->objects[vit]->holding_a = holding;}
 void D(int vit, char key, char holding)    {game->objects[vit]->D = key; game->objects[vit]->holding_d = holding;}
 void J(int vit, char key, char holding)    {game->objects[vit]->J = key; game->objects[vit]->holding_j = holding;}
 void up(int vit, char key, char holding)   {game->objects[vit]->up = key; game->objects[vit]->holding_up = holding;}
 void left(int vit, char key, char holding) {game->objects[vit]->left = key; game->objects[vit]->holding_left = holding;}
 void down(int vit, char key, char holding) {game->objects[vit]->down = key; game->objects[vit]->holding_down = holding;}
 void right(int vit, char key, char holding){game->objects[vit]->right = key; game->objects[vit]->holding_right = holding;}
 void DJAR(int vit)                         {game->objects[vit]->DrA = 0; game->objects[vit]->DlA = 0; game->objects[vit]->DuA = 0; game->objects[vit]->DdA = 0; game->objects[vit]->DrJ = 0; game->objects[vit]->DlJ = 0; game->objects[vit]->DuJ = 0; game->objects[vit]->DdJ = 0; game->objects[vit]->DJA = 0;}
 
 void Control_MHP(int vit, int agi)            {game->objects[vit]->max_hp = agi;}
 void Control_HP(int vit, int agi)             {game->objects[vit]->hp = agi;}
 void Control_DHP(int vit, int agi)            {game->objects[vit]->dark_hp = agi;}
 void Control_MP(int vit, int agi)             {game->objects[vit]->mp = agi;}
 void Control_X(int vit, int agi)              {game->objects[vit]->x = agi;}
 void Control_Y(int vit, int agi)              {game->objects[vit]->y = agi;}
 void Control_Z(int vit, int agi)              {game->objects[vit]->z = agi;}
 void Control_XR(int vit, float agi)           {game->objects[vit]->x_real = agi;}
 void Control_YR(int vit, float agi)           {game->objects[vit]->y_real = agi;}
 void Control_ZR(int vit, float agi)           {game->objects[vit]->z_real = agi;}
 void Control_VX(int vit, float agi)           {game->objects[vit]->x_velocity = agi;}
 void Control_VY(int vit, float agi)           {game->objects[vit]->y_velocity = agi;}
 void Control_VZ(int vit, float agi)           {game->objects[vit]->z_velocity = agi;}
 void Control_AVX(int vit, float agi)          {game->objects[vit]->x_acceleration = agi;}
 void Control_AVY(int vit, float agi)          {game->objects[vit]->y_acceleration = agi;}
 void Control_AVZ(int vit, float agi)          {game->objects[vit]->z_acceleration = agi;}
 void Control_Frame1(int vit, int agi)         {game->objects[vit]->frame1 = agi;}
 void Control_Facing(int vit, int agi)         {if(agi > 125){agi = 125;} else {if(agi < -125) agi = -125;} game->objects[vit]->facing = static_cast<char>(agi);}
 void Control_Team(int vit, int agi)           {if(agi > 125){agi = 125;} else {if(agi < -125) agi = -125;} game->objects[vit]->team = static_cast<char>(agi);}
 void Control_Shake(int vit, int agi)          {game->objects[vit]->shake = agi;}
 void Control_Fall(int vit, int agi)           {game->objects[vit]->fall = agi;}
 void Control_Blink(int vit, int agi)          {game->objects[vit]->blink = agi;}
 void Control_WaitCounter(int vit, int agi)    {game->objects[vit]->wait_counter = agi;}
 void Control_Walk(int vit, int agi)           {game->objects[vit]->move_counter = agi;}
 void Control_Run(int vit, int agi)            {game->objects[vit]->run_counter = agi;}
 void Control_FCDAct(int vit, int agi, int dex){game->objects[vit]->data->frames[agi].cpoint.daction = dex;}
 void Control_FWait(int vit, int agi, int dex) {game->objects[vit]->data->frames[agi].wait = dex;}
 void Control_FOKind(int vit, int agi, int dex){game->objects[vit]->data->frames[agi].opoint.kind = dex;}
 void Control_Arest(int vit, int agi)          {game->objects[vit]->arest = agi;}
 void Control_Vrest(int vit, int agi, int dex) {if(dex > 125){dex = 125;} else {if(dex < -125) dex = -125;} game->objects[vit]->vrests[agi] = static_cast<char>(dex);}
 void Control_InputNone(int vit)               {game->objects[vit]->A = 0; game->objects[vit]->D = 0; game->objects[vit]->J = 0; game->objects[vit]->up = 0; game->objects[vit]->left = 0; game->objects[vit]->down = 0; game->objects[vit]->right = 0; game->objects[vit]->DrA = 0; game->objects[vit]->DlA = 0; game->objects[vit]->DuA = 0; game->objects[vit]->DdA = 0; game->objects[vit]->DrJ = 0; game->objects[vit]->DlJ = 0; game->objects[vit]->DuJ = 0; game->objects[vit]->DdJ = 0; game->objects[vit]->DJA = 0;}

 void Control_BG(int agi)          {*(int*)0x44d024 = agi;}
 void Control_BGT(int vit, int agi){if(agi > 125){agi = 125;} else {if(agi < -125) agi = -125;} game->files->backgrounds[*(int*)0x44d024].unkwn1[vit] = static_cast<char>(agi);}
 void Control_BGW(int agi)         {game->files->backgrounds[*(int*)0x44d024].bg_width = agi;}
 void Control_BGZ1(int agi)        {game->files->backgrounds[*(int*)0x44d024].bg_zwidth1 = agi;}
 void Control_BGZ2(int agi)        {game->files->backgrounds[*(int*)0x44d024].bg_zwidth2 = agi;}
 void Control_BGx(int vit, int agi){game->files->backgrounds[*(int*)0x44d024].layer_x[vit] = agi;}
 void Control_BGy(int vit, int agi){game->files->backgrounds[*(int*)0x44d024].layer_y[vit] = agi;}
 void Control_BGh(int vit, int agi){game->files->backgrounds[*(int*)0x44d024].layer_height[vit] = agi;}
 void Control_PrePhase(int agi)    {*(int*)0x44f880 = agi;}

 void printAddr(void *Addr){printf("%p", Addr);}
 void printReset()
 {
  if(hConsole == INVALID_HANDLE_VALUE) return;
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  FillConsoleOutputAttribute(hConsole, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coordScreen, &cCharsWritten);
  SetConsoleCursorPosition(hConsole, coordScreen);
 }
 void printOut()
 {
  std::string PrintText; std::string PrePrintText; std::ifstream PrintLog("log.txt"); bool NewLine = false;
  if(PrintLog.is_open())
  {
   int Redudantcy = -1;
   while(PrintLog)
   {
    Redudantcy += 1; if(NewLine) PrintText += "\n"; NewLine = true;
	PrePrintText = ""; getline(PrintLog, PrePrintText); PrintText.append(PrePrintText, 0, PrePrintText.length());
	unsigned int OldTextLength = PrintText.length();
	if(OldTextLength > log_length[Redudantcy]) log_length[Redudantcy] = OldTextLength;
	for(unsigned int repulish = 1; repulish <= log_length[Redudantcy] - OldTextLength; ++repulish) PrintText += " ";
	log_length[Redudantcy] = OldTextLength;
   }
   if(Redudantcy > log_Redudantcy) log_Redudantcy = Redudantcy;
   for(int fullrepulish = 1; fullrepulish < log_Redudantcy - Redudantcy; ++fullrepulish)
   {
	Redudantcy += 1; if(NewLine) PrintText += "\n"; NewLine = true;
	for(unsigned int repulish = 1; repulish <= log_length[Redudantcy]; ++repulish) PrintText += " ";
   }
   printReset(); printf("%s", PrintText.c_str()); PrintLog.close();
  }
  std::ofstream ResetLog("log.txt"); ResetLog << ""; ResetLog.close();
 }
 void print(bool p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  if(p){PrintLog << "true";} else {PrintLog << "false";}
  PrintLog.close();
 }
 void print(char p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(short p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(int p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(long long p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(unsigned char p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(unsigned short p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(unsigned int p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(unsigned long long p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(double p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(float p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }
 void print(const std::string &p)
 {
  std::ofstream PrintLog; PrintLog.open("log.txt", std::ofstream::app);
  PrintLog << p;
  PrintLog.close();
 }

 std::string getFileName()
 {
  std::ostringstream oss; oss << "data\\23.as";
  return oss.str();
 }
 FileValidity isValidFileName(std::string file)
 {
  if(file.length() < 7) return INVALID;
  for(unsigned int i = 0; i < file.length(); ++i){if(file.at(i) >= 'A' && file.at(i) <= 'Z') file.at(i) += 'a' - 'A';}
  if(file.substr(0, 3) != "data\\") return INVALID;
  if(file.at(3) == '_') return MODULE;
  return VALID;
 }
 std::string getModuleName(const std::string& file){return file.substr(3, file.length() - 1);}

 bool hasFileTimeChanged(const char *File, FILETIME *Time)
 {
  HANDLE hFile;
  hFile = CreateFile(File, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  FILETIME ftWrite;
  if(hFile == INVALID_HANDLE_VALUE)
  {
   ftWrite.dwLowDateTime = 0; ftWrite.dwHighDateTime = 0;
  } else
  {
   if(!GetFileTime(hFile, NULL, NULL, &ftWrite)){CloseHandle(hFile); return false;}
   CloseHandle(hFile);
  }
  if(CompareFileTime(&ftWrite, Time) != 0){(*Time) = ftWrite; return true;}
  return false;
 }
 void rebuildIfUpdated(int id_int, const std::string &fileName)
 {
  if(ModuleTimes.count(id_int))
  {
   if(hasFileTimeChanged(fileName.c_str(), &(ModuleTimes[id_int])))
   {
	CScriptBuilder Builder;
	Builder.DefineWord("debug");
	if(fileName.at(0) == '_'){Builder.DefineWord("module");} else {Builder.DefineWord("character");}
    #ifdef DEBUG_VERSION
	 ScriptEngine->WriteMessage(fileName.c_str(), 0, 0, asMSGTYPE_INFORMATION, "Rebuilding.");
    #endif
	Builder.StartNewModule(ScriptEngine, getModuleName(fileName).c_str());
	int r = Builder.AddSectionFromFile(fileName.c_str());
	if(r < 0)
    {
     #ifdef DEBUG_VERSION
	  ScriptEngine->WriteMessage(fileName.c_str(), 0, 0, asMSGTYPE_ERROR, "Unable to load file.");
     #endif
	 ScriptEngine->DiscardModule(getModuleName(fileName).c_str());
	 return;
	}
    r = Builder.BuildModule();
	if( r < 0)
    {
     #ifdef DEBUG_VERSION
	  ScriptEngine->WriteMessage(fileName.c_str(), 0, 0, asMSGTYPE_ERROR, "Unable to build module.");
     #endif
	 ScriptEngine->DiscardModule(getModuleName(fileName).c_str());
	 return;
	}
   }
  } else
  {
   FILETIME Time = {0, 0};
   if(hasFileTimeChanged(fileName.c_str(), &Time))
   {
	ModuleTimes.insert(std::pair<int, FILETIME>(id_int, Time));
	CScriptBuilder Builder;
	Builder.DefineWord("debug");
    #ifdef DEBUG_VERSION
     ScriptEngine->WriteMessage(fileName.c_str(), 0, 0, asMSGTYPE_INFORMATION, "Building.");
    #endif
    Builder.StartNewModule(ScriptEngine, getModuleName(fileName).c_str());
	int r = Builder.AddSectionFromFile(fileName.c_str());
	if(r < 0)
    {
     #ifdef DEBUG_VERSION
	  ScriptEngine->WriteMessage(fileName.c_str(), 0, 0, asMSGTYPE_ERROR, "Unable to load file.");
     #endif
	 ScriptEngine->DiscardModule(getModuleName(fileName).c_str());
	 return;
	}
    r = Builder.BuildModule();
	if( r < 0)
    {
     #ifdef DEBUG_VERSION
	  ScriptEngine->WriteMessage(fileName.c_str(), 0, 0, asMSGTYPE_ERROR, "Unable to build module.");
     #endif
	 ScriptEngine->DiscardModule(getModuleName(fileName).c_str());
	 return;
	}
   }
  }
 }
 int getIdFromFileName(const std::string &file){int x; std::stringstream(getModuleName(file)) >> x; return x;}
 
 int callEgo(int object_num, int target_num, asIScriptFunction *Function)
 {
  elapsed_time = *(int *)0x450b8c;
  difficulty = *(int *)0x450c30;
  mode = *(int *)0x451160;
  if(background != *(int *)0x44d024)
  {
   background = *(int *)0x44d024;
   bg_width = game->files->backgrounds[background].bg_width;
   bg_zwidth1 = game->files->backgrounds[background].bg_zwidth1;
   bg_zwidth2 = game->files->backgrounds[background].bg_zwidth2;
  }
  if(mode == 1)
  {
   stage_bound = *(int *)0x450bb4;
   stage_clear = stage_bound == 0;
  } else 
  {
   stage_bound = bg_width;
   stage_clear = false;
  }
  current_phase = *(int *)0x44fb6c;
  current_phase_count = *(int *)0x44f880;
  current_stage = *(int *)0x450b94;
  if((self = object_num) == -1)
  {
   #ifdef DEBUG_VERSION
	ScriptEngine->WriteMessage(getFileName().c_str(),0,0,asMSGTYPE_ERROR,"Unable to prepare self.");
   #endif
  }
  if((target = target_num) == -1)
  {
   #ifdef DEBUG_VERSION
    ScriptEngine->WriteMessage(getFileName().c_str(),0,0,asMSGTYPE_ERROR,"Unable to prepare target.");
   #endif
  }
  int r = ScriptContext->Prepare(Function);
  if(r < 0)
  {
   #ifdef DEBUG_VERSION
	ScriptEngine->WriteMessage(getFileName().c_str(),0,0,asMSGTYPE_ERROR,"Unable to prepare the context.");
   #endif
   return 0;
  }
  r = ScriptContext->Execute();
  if(r < 0)
  {
   #ifdef DEBUG_VERSION
	ScriptEngine->WriteMessage(getFileName().c_str(),0,0,asMSGTYPE_ERROR,"Unable to execute the context.");
   #endif
   return 0;
  }
  return ScriptContext->GetReturnDWord();
 }
 void callId(int object_num, asIScriptFunction *Function)
 {
  elapsed_time = *(int *)0x450b8c;
  difficulty = *(int *)0x450c30;
  mode = *(int *)0x451160;
  if(background != *(int *)0x44d024)
  {
   background = *(int *)0x44d024;
   bg_width = game->files->backgrounds[background].bg_width;
   bg_zwidth1 = game->files->backgrounds[background].bg_zwidth1;
   bg_zwidth2 = game->files->backgrounds[background].bg_zwidth2;
  }
  if(mode == 1)
  {
   stage_bound = *(int *)0x450bb4;
   stage_clear = stage_bound == 0;
  } else 
  {
   stage_bound = bg_width;
   stage_clear = false;
  }
  current_phase = *(int *)0x44fb6c;
  current_phase_count = *(int *)0x44f880;
  current_stage = *(int *)0x450b94;
  if((self = object_num) == -1)
  {
   #ifdef DEBUG_VERSION
	ScriptEngine->WriteMessage(getFileName().c_str(),0,0,asMSGTYPE_ERROR,"Unable to prepare self.");
   #endif
  }
  int r = ScriptContext->Prepare(Function);
  if(r < 0)
  {
   #ifdef DEBUG_VERSION
	ScriptEngine->WriteMessage(getFileName().c_str(),0,0,asMSGTYPE_ERROR,"Unable to prepare the context.");
   #endif
   return;
  }
  r = ScriptContext->Execute();
  #ifdef DEBUG_VERSION
  if(r < 0)
   {
	ScriptEngine->WriteMessage(getFileName().c_str(),0,0,asMSGTYPE_ERROR,"Unable to execute the context.");
   }
  #endif
 }
 int __stdcall AI(int target_num, int object_num, int unkwn1, int unkwn2, int unkwn3, int unkwn4, int unkwn5)
 {
  DWORD return_value;
  DWORD unkwn6;
  __asm
  {
   mov dword ptr ds:[unkwn6],ecx;
  }
  std::string fileName = getFileName();
  ScriptModule = ScriptEngine->GetModule(getModuleName(fileName).c_str());
  if(ScriptModule)
  {
   asIScriptFunction *Function = ScriptModule->GetFunctionByDecl("int ego()");
   if(Function){return callEgo(object_num, target_num, Function);}
  }
  __asm
  {
   mov ecx, dword ptr ds:[unkwn5];
   push ecx;
   mov ecx, dword ptr ds:[unkwn4];
   push ecx;
   mov ecx, dword ptr ds:[unkwn3];
   push ecx;
   mov ecx, dword ptr ds:[unkwn2];
   push ecx;
   mov ecx, dword ptr ds:[unkwn1];
   push ecx;
   mov ecx, dword ptr ds:[object_num];
   push ecx;
   mov ecx, dword ptr ds:[target_num];
   push ecx;
   mov ecx, dword ptr ds:[unkwn6];
   call AI_o;
   mov dword ptr ds:[return_value], eax;
  }
  return return_value;
 }
 void __stdcall AIa(int object_num, int unkwn1)
 {
  DWORD unkwn2;
  _asm
  {
   mov dword ptr ds:[unkwn2], ecx;
  }
  std::string fileName = getFileName();
  rebuildIfUpdated(23, fileName);
  ScriptModule = ScriptEngine->GetModule(getModuleName(fileName).c_str());
  if(ScriptModule)
  {
   asIScriptFunction *Function = ScriptModule->GetFunctionByDecl("void id()");
   if(Function){callId(object_num, Function); return;}
   #ifdef DEBUG_VERSION
    else
    {
	 if(!ScriptModule->GetFunctionByDecl("int ego()"))
     {
	  ScriptEngine->WriteMessage(getFileName().c_str(), 0, 0, asMSGTYPE_ERROR, "Neither 'void id()' nor 'int ego()' are defined.");
	 }
	}
   #endif
  }
  _asm
  {
   mov ecx, dword ptr ds:[unkwn1];
   push ecx;
   mov ecx, dword ptr ds:[object_num];
   push ecx;
   mov ecx, dword ptr ds:[unkwn2];
   call AIa_o;
  }
 }

 #ifdef DEBUG_VERSION
  void MessageCallback(const asSMessageInfo *msg, void *)
  {
   if(msg->section[0] == '\0')
   {
	printf("%s: %s\n", msg->type == asMSGTYPE_ERROR?"ERROR":msg->type == asMSGTYPE_WARNING?"WARNING":"INFORMATION", msg->message);
   } else
   {
    if(msg->row == 0 && msg->col == 0)
    {
	 printf("%s: %s : %s\n", msg->section, msg->type == asMSGTYPE_ERROR?"ERROR":msg->type == asMSGTYPE_WARNING?"WARNING":"INFORMATION", msg->message);
	} else
    {
	 printf("%s(%d, %d): %s : %s\n", msg->section, msg->row, msg->col, msg->type == asMSGTYPE_ERROR?"ERROR":msg->type == asMSGTYPE_WARNING?"WARNING":"INFORMATION", msg->message);
	}
   }
  }
 #endif
 void RegisterScriptFunctions()
 {
  RegisterScriptMath(ScriptEngine);
  RegisterScriptMathComplex(ScriptEngine);
  RegisterScriptMath3D(ScriptEngine);
  RegisterStdString(ScriptEngine);
  RegisterScriptArray(ScriptEngine, true);
  RegisterStdStringUtils(ScriptEngine);
  RegisterGameEnums(ScriptEngine);
  RegisterGameArrays(ScriptEngine);
  
  ScriptEngine->RegisterGlobalFunction("int Cnvrt_F_I(float vit)", asFUNCTION(Cnvrt_F_I), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("int Cnvrt_D_I(double vit)", asFUNCTION(Cnvrt_D_I), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("double Cnvrt_F_D(float vit)", asFUNCTION(Cnvrt_F_D), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("double Cnvrt_I_D(int vit)", asFUNCTION(Cnvrt_I_D), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("float Cnvrt_D_F(double vit)", asFUNCTION(Cnvrt_D_F), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("float Cnvrt_I_F(int vit)", asFUNCTION(Cnvrt_I_F), asCALL_CDECL);

  ScriptEngine->RegisterGlobalFunction("int Camera()", asFUNCTION(Camera), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_MHP(int vit, int agi)", asFUNCTION(Control_MHP), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_HP(int vit, int agi)", asFUNCTION(Control_HP), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_DHP(int vit, int agi)", asFUNCTION(Control_DHP), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_MP(int vit, int agi)", asFUNCTION(Control_MP), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_X(int vit, int agi)", asFUNCTION(Control_X), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Y(int vit, int agi)", asFUNCTION(Control_Y), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Z(int vit, int agi)", asFUNCTION(Control_Z), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_XR(int vit, float agi)", asFUNCTION(Control_XR), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_YR(int vit, float agi)", asFUNCTION(Control_YR), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_ZR(int vit, float agi)", asFUNCTION(Control_ZR), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_VX(int vit, float agi)", asFUNCTION(Control_VX), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_VY(int vit, float agi)", asFUNCTION(Control_VY), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_VZ(int vit, float agi)", asFUNCTION(Control_VZ), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_AVX(int vit, float agi)", asFUNCTION(Control_AVX), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_AVY(int vit, float agi)", asFUNCTION(Control_AVY), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_AVZ(int vit, float agi)", asFUNCTION(Control_AVZ), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Frame1(int vit, int agi)", asFUNCTION(Control_Frame1), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Facing(int vit, int agi)", asFUNCTION(Control_Facing), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Team(int vit, int agi)", asFUNCTION(Control_Team), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Shake(int vit, int agi)", asFUNCTION(Control_Shake), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Fall(int vit, int agi)", asFUNCTION(Control_Fall), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Blink(int vit, int agi)", asFUNCTION(Control_Blink), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_WaitCounter(int vit, int agi)", asFUNCTION(Control_WaitCounter), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Walk(int vit, int agi)", asFUNCTION(Control_Walk), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Run(int vit, int agi)", asFUNCTION(Control_Run), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_FWait(int vit, int agi, int dex)", asFUNCTION(Control_FWait), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_FCDAct(int vit, int agi, int dex)", asFUNCTION(Control_FCDAct), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_FOKind(int vit, int agi, int dex)", asFUNCTION(Control_FOKind), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Arest(int vit, int agi)", asFUNCTION(Control_Arest), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_Vrest(int vit, int agi, int dex)", asFUNCTION(Control_Vrest), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_InputNone(int vit)", asFUNCTION(Control_InputNone), asCALL_CDECL);

  ScriptEngine->RegisterGlobalFunction("void Control_BG(int agi)", asFUNCTION(Control_BG), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_BGT(int vit, int agi)", asFUNCTION(Control_BGT), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_BGW(int agi)", asFUNCTION(Control_BGW), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_BGZ1(int agi)", asFUNCTION(Control_BGZ1), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_BGZ2(int agi)", asFUNCTION(Control_BGZ2), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_BGx(int vit, int agi)", asFUNCTION(Control_BGx), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_BGy(int vit, int agi)", asFUNCTION(Control_BGy), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void Control_BGh(int vit, int agi)", asFUNCTION(Control_BGh), asCALL_CDECL);
  
  ScriptEngine->RegisterGlobalFunction("void printOut()", asFUNCTION(printOut), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(bool p)", asFUNCTIONPR(print, (bool p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(int8 p)", asFUNCTIONPR(print, (char p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(int16 p)", asFUNCTIONPR(print, (short p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(int32 p)", asFUNCTIONPR(print, (int p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(int64 p)", asFUNCTIONPR(print, (long long p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(uint8 p)", asFUNCTIONPR(print, (unsigned char p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(uint16 p)", asFUNCTIONPR(print, (unsigned short p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(uint32 p)", asFUNCTIONPR(print, (unsigned int p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(uint64 p)", asFUNCTIONPR(print, (unsigned long long p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(float p)", asFUNCTIONPR(print, (float p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(double p)", asFUNCTIONPR(print, (double p), void), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void print(const string &in p)", asFUNCTIONPR(print, (const std::string & p), void), asCALL_CDECL);

  ScriptEngine->RegisterGlobalFunction("int loadTarget(int object_num)", asFUNCTION(loadTarget), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("int setEnemy(int object_num)", asFUNCTION(setEnemy), asCALL_CDECL);

  ScriptEngine->RegisterGlobalFunction("void up(int vit, int8 key = 1, int8 holding = 0)", asFUNCTION(up), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void up(int vit, bool key, int8 holding = 0)", asFUNCTION(up), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void up(int vit, int8 key, bool holding)", asFUNCTION(up), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void up(int vit, bool key, bool holding)", asFUNCTION(up), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void down(int vit, int8 key = 1, int8 holding = 0)", asFUNCTION(down), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void down(int vit, bool key, int8 holding = 0)", asFUNCTION(down), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void down(int vit, int8 key, bool holding)", asFUNCTION(down), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void down(int vit, bool key, bool holding)", asFUNCTION(down), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void left(int vit, int8 key = 1, int8 holding = 0)", asFUNCTION(left), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void left(int vit, bool key, int8 holding = 0)", asFUNCTION(left), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void left(int vit, int8 key, bool holding)", asFUNCTION(left), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void left(int vit, bool key, bool holding)", asFUNCTION(left), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void right(int vit, int8 key = 1, int8 holding = 0)", asFUNCTION(right), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void right(int vit, bool key, int8 holding = 0)", asFUNCTION(right), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void right(int vit, int8 key, bool holding)", asFUNCTION(right), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void right(int vit, bool key, bool holding)", asFUNCTION(right), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void A(int vit, int8 key = 1, int8 holding = 0)", asFUNCTION(A), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void A(int vit, bool key, int8 holding = 0)", asFUNCTION(A), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void A(int vit, int8 key, bool holding)", asFUNCTION(A), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void A(int vit, bool key, bool holding)", asFUNCTION(A), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void J(int vit, int8 key = 1, int8 holding = 0)", asFUNCTION(J), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void J(int vit, bool key, int8 holding = 0)", asFUNCTION(J), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void J(int vit, int8 key, bool holding)", asFUNCTION(J), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void J(int vit, bool key, bool holding)", asFUNCTION(J), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void D(int vit, int8 key = 1, int8 holding = 0)", asFUNCTION(D), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void D(int vit, bool key, int8 holding = 0)", asFUNCTION(D), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void D(int vit, int8 key, bool holding)", asFUNCTION(D), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void D(int vit, bool key, bool holding)", asFUNCTION(D), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void DJAR(int vit)", asFUNCTION(DJAR), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("int rand(int n)", asFUNCTION(random), asCALL_CDECL);

  ScriptEngine->RegisterGlobalProperty("const int mode", &mode);
  ScriptEngine->RegisterGlobalProperty("const int difficulty", &difficulty);
  ScriptEngine->RegisterGlobalProperty("const int elapsed_time", &elapsed_time);
  ScriptEngine->RegisterGlobalProperty("const int background", &background);
  ScriptEngine->RegisterGlobalProperty("const int bg_width", &bg_width);
  ScriptEngine->RegisterGlobalProperty("const int bg_zwidth1", &bg_zwidth1);
  ScriptEngine->RegisterGlobalProperty("const int bg_zwidth2", &bg_zwidth2);
  ScriptEngine->RegisterGlobalProperty("const int stage_bound", &stage_bound);
  ScriptEngine->RegisterGlobalProperty("const bool stage_clear", &stage_clear);
  ScriptEngine->RegisterGlobalProperty("const int current_phase", &current_phase);
  ScriptEngine->RegisterGlobalProperty("const int current_phase_count", &current_phase_count);
  ScriptEngine->RegisterGlobalProperty("const int current_stage", &current_stage);

  ScriptEngine->RegisterObjectType("Info", 0, asOBJ_REF);
  ScriptEngine->RegisterObjectBehaviour("Info", asBEHAVE_ADDREF, "void f()", asMETHOD(Info, AddRef), asCALL_THISCALL);
  ScriptEngine->RegisterObjectBehaviour("Info", asBEHAVE_RELEASE, "void f()", asMETHOD(Info, RelRef), asCALL_THISCALL);
  ScriptEngine->RegisterObjectBehaviour("Info", asBEHAVE_FACTORY, "Info @f()", asFUNCTIONPR(Info_Factory, (), Info*), asCALL_CDECL);
  ScriptEngine->RegisterObjectBehaviour("Info", asBEHAVE_FACTORY, "Info @f(int)", asFUNCTIONPR(Info_Factory, (int object_num), Info*), asCALL_CDECL);
  ScriptEngine->RegisterObjectBehaviour("Info", asBEHAVE_FACTORY, "Info @f(const Info &in info)", asFUNCTIONPR(Info_Factory, (const Info & info), Info*), asCALL_CDECL);

  ScriptEngine->RegisterObjectMethod("Info", "int opAssign(int object_num)", asMETHODPR(Info, operator=, (int object_num), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opAssign(const Info &in info)", asMETHODPR(Info, operator=, (const Info & info), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opAddAssign(int object_num)", asMETHODPR(Info, operator+=, (int object_num), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opAddAssign(const Info &in info)", asMETHODPR(Info, operator+=, (const Info & info), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opSubAssign(int object_num)", asMETHODPR(Info, operator-=, (int object_num), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opSubAssign(const Info &in info)", asMETHODPR(Info, operator-=, (const Info & info), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opPreInc()", asMETHODPR(Info, operator++, (), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opPreSub()", asMETHODPR(Info, operator--, (), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opPostInc()", asMETHODPR(Info, operator++, (), int), asCALL_THISCALL);
  ScriptEngine->RegisterObjectMethod("Info", "int opPostSub()", asMETHODPR(Info, operator--, (), int), asCALL_THISCALL);

  ScriptEngine->RegisterObjectProperty("Info", "int x", asOFFSET(Info, x));
  ScriptEngine->RegisterObjectProperty("Info", "int y", asOFFSET(Info, y));
  ScriptEngine->RegisterObjectProperty("Info", "int z", asOFFSET(Info, z));
  ScriptEngine->RegisterObjectProperty("Info", "vector3 position", asOFFSET(Info, position));
  ScriptEngine->RegisterObjectProperty("Info", "double x_real", asOFFSET(Info, x_real));
  ScriptEngine->RegisterObjectProperty("Info", "double y_real", asOFFSET(Info, y_real));
  ScriptEngine->RegisterObjectProperty("Info", "double z_real", asOFFSET(Info, z_real));
  ScriptEngine->RegisterObjectProperty("Info", "bool facing", asOFFSET(Info, facing));
  ScriptEngine->RegisterObjectProperty("Info", "bool holding_up", asOFFSET(Info, holding_up));
  ScriptEngine->RegisterObjectProperty("Info", "bool holding_down", asOFFSET(Info, holding_down));
  ScriptEngine->RegisterObjectProperty("Info", "bool holding_left", asOFFSET(Info, holding_left));
  ScriptEngine->RegisterObjectProperty("Info", "bool holding_right", asOFFSET(Info, holding_right));
  ScriptEngine->RegisterObjectProperty("Info", "bool holding_a", asOFFSET(Info, holding_a));
  ScriptEngine->RegisterObjectProperty("Info", "bool holding_j", asOFFSET(Info, holding_j));
  ScriptEngine->RegisterObjectProperty("Info", "bool holding_d", asOFFSET(Info, holding_d));
  ScriptEngine->RegisterObjectProperty("Info", "bool up", asOFFSET(Info, up));
  ScriptEngine->RegisterObjectProperty("Info", "bool down", asOFFSET(Info, down));
  ScriptEngine->RegisterObjectProperty("Info", "bool left", asOFFSET(Info, left));
  ScriptEngine->RegisterObjectProperty("Info", "bool right", asOFFSET(Info, right));
  ScriptEngine->RegisterObjectProperty("Info", "bool A", asOFFSET(Info, A));
  ScriptEngine->RegisterObjectProperty("Info", "bool J", asOFFSET(Info, J));
  ScriptEngine->RegisterObjectProperty("Info", "bool D", asOFFSET(Info, D));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DrA", asOFFSET(Info, DrA));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DlA", asOFFSET(Info, DlA));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DuA", asOFFSET(Info, DuA));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DdA", asOFFSET(Info, DdA));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DrJ", asOFFSET(Info, DrJ));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DlJ", asOFFSET(Info, DlJ));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DuJ", asOFFSET(Info, DuJ));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DdJ", asOFFSET(Info, DdJ));
  ScriptEngine->RegisterObjectProperty("Info", "int8 DJA", asOFFSET(Info, DJA));
  ScriptEngine->RegisterObjectProperty("Info", "int hp", asOFFSET(Info, hp));
  ScriptEngine->RegisterObjectProperty("Info", "int dark_hp", asOFFSET(Info, dark_hp));
  ScriptEngine->RegisterObjectProperty("Info", "int max_hp", asOFFSET(Info, max_hp));
  ScriptEngine->RegisterObjectProperty("Info", "int mp", asOFFSET(Info, mp));
  ScriptEngine->RegisterObjectProperty("Info", "int frame", asOFFSET(Info, frame));
  ScriptEngine->RegisterObjectProperty("Info", "int bdefend", asOFFSET(Info, bdefend));
  ScriptEngine->RegisterObjectProperty("Info", "int fall", asOFFSET(Info, fall));
  ScriptEngine->RegisterObjectProperty("Info", "int team", asOFFSET(Info, team));
  ScriptEngine->RegisterObjectProperty("Info", "int id", asOFFSET(Info, id));
  ScriptEngine->RegisterObjectProperty("Info", "int blink", asOFFSET(Info, blink));
  ScriptEngine->RegisterObjectProperty("Info", "int state", asOFFSET(Info, state));
  ScriptEngine->RegisterObjectProperty("Info", "int weapon_type", asOFFSET(Info, weapon_type));
  ScriptEngine->RegisterObjectProperty("Info", "int weapon_held", asOFFSET(Info, weapon_held));
  ScriptEngine->RegisterObjectProperty("Info", "int weapon_holder", asOFFSET(Info, weapon_holder));
  ScriptEngine->RegisterObjectProperty("Info", "int shake", asOFFSET(Info, shake));
  ScriptEngine->RegisterObjectProperty("Info", "int wait_counter", asOFFSET(Info, wait_counter));
  ScriptEngine->RegisterObjectProperty("Info", "int num", asOFFSET(Info, num));
  ScriptEngine->RegisterObjectProperty("Info", "int ctimer", asOFFSET(Info, ctimer));
  ScriptEngine->RegisterObjectProperty("Info", "int arest", asOFFSET(Info, arest));
  ScriptEngine->RegisterObjectProperty("Info", "CharArray vrests", asOFFSET(Info, vrests));
  ScriptEngine->RegisterObjectProperty("Info", "int vrest", asOFFSET(Info, vrest));
  ScriptEngine->RegisterObjectProperty("Info", "vector3 velocity", asOFFSET(Info, velocity));
  ScriptEngine->RegisterObjectProperty("Info", "double x_velocity", asOFFSET(Info, x_velocity));
  ScriptEngine->RegisterObjectProperty("Info", "double y_velocity", asOFFSET(Info, y_velocity));
  ScriptEngine->RegisterObjectProperty("Info", "double z_velocity", asOFFSET(Info, z_velocity));
  ScriptEngine->RegisterObjectProperty("Info", "int clone", asOFFSET(Info, clone));
  ScriptEngine->RegisterObjectProperty("Info", "int type", asOFFSET(Info, type));
  ScriptEngine->RegisterObjectProperty("Info", "int reserve", asOFFSET(Info, reserve));
  ScriptEngine->RegisterObjectProperty("Info", "DataFile @data", asOFFSET(Info, data));

  ScriptEngine->RegisterGlobalFunction("void printAddr(const IntArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const BoolArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const CharArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const CharArrayArray30 @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const CharArrayArray40 @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const DataFileArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const ObjectArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const FrameArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const BackgroundArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const SpawnArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const PhaseArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const StageArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const ItrArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const BdyArray @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Game @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const FileManager @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Stage @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Phase @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Spawn @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Background @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Object @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const DataFile @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Frame @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Bdy @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Itr @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Wpoint @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Cpoint @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Bpoint @addr)", asFUNCTION(printAddr), asCALL_CDECL);
  ScriptEngine->RegisterGlobalFunction("void printAddr(const Opoint @addr)", asFUNCTION(printAddr), asCALL_CDECL);

  ScriptEngine->RegisterGlobalProperty("const Info self", &self);
  ScriptEngine->RegisterGlobalProperty("Info target", &target);
  ScriptEngine->RegisterGlobalProperty("const Game game", game);
 }
 //-//

 //System Functions
 void startup()
 {
  #ifdef DEBUG_VERSION
   AllocConsole();
   freopen("CONIN$", "rb", stdin);   // reopen stdin handle as console window input
   freopen("CONOUT$", "wb", stdout); // reopen stout handle as console window output
   freopen("CONOUT$", "wb", stderr); // reopen stderr handle as console window output
  #endif
  ScriptModule = NULL;
  ScriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
  if(!ScriptEngine){::ExitProcess(0);}
  #ifdef DEBUG_VERSION
   ScriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), NULL, asCALL_CDECL);
  #endif
  ScriptContext = ScriptEngine->CreateContext();
  RegisterScriptFunctions();
  AI_o = (int(__stdcall*)(int, int, int, int, int, int, int))DetourFunction((PBYTE)0x00403a40, (PBYTE)AI);
  AIa_o = (int(__stdcall*)(int, int))DetourFunction((PBYTE)0x004094b0, (PBYTE)AIa);
  HANDLE hFind;
  WIN32_FIND_DATA FindFileData;
  char DirPath[MAX_PATH] = "data\\23.as";
  char FileName[MAX_PATH] = "data\\23.as";
  hFind = FindFirstFile(DirPath, &FindFileData);
  if(hFind == INVALID_HANDLE_VALUE) return;
  do {
	  strcpy(FileName, DirPath);
	  strcat(FileName, FindFileData.cFileName);
	  if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
	   FileValidity Validity = isValidFileName(FileName);
	   if(Validity)
       {
		FILETIME Time;
		hasFileTimeChanged(FileName, &Time);
		ModuleTimes.insert(std::pair<int, FILETIME>(getIdFromFileName(FileName), Time));
        CScriptBuilder Builder;
        #ifdef DEBUG_VERSION
		 Builder.DefineWord("debug");
		 ScriptEngine->WriteMessage(FileName, 0, 0, asMSGTYPE_INFORMATION, "Building.");
        #else
		 Builder.DefineWord("release");
        #endif
        if(Validity == MODULE){Builder.DefineWord("module");} else {Builder.DefineWord("character");}
        Builder.StartNewModule(ScriptEngine, getModuleName(FileName).c_str());
        int r = Builder.AddSectionFromFile(FileName);
		if(r < 0)
        {
         #ifdef DEBUG_VERSION
		  ScriptEngine->WriteMessage(FileName, 0, 0, asMSGTYPE_ERROR, "Unable to load file.");
         #endif
		 ScriptEngine->DiscardModule(getModuleName(FileName).c_str());
		 continue;
	    }
        r = Builder.BuildModule();
		if(r < 0)
        {
         #ifdef DEBUG_VERSION
		  ScriptEngine->WriteMessage(FileName, 0, 0, asMSGTYPE_ERROR, "Unable to build module.");
         #endif
		 ScriptEngine->DiscardModule(getModuleName(FileName).c_str());
		 continue;
		}
	   }
	  }
	 } while (FindNextFile(hFind, &FindFileData));
  FindClose(hFind);
 }
 void cleanup(){if(ScriptContext) ScriptContext->Release(); if(ScriptEngine) ScriptEngine->Release();}

 BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
 {
  LPVOID lpDummy = lpReserved;
  lpDummy = NULL;
  switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    startup();
	LoadingImg(true);
    VersionControl();
    StartDataControl();
	LoadingImg(false);
    InitInstance(hModule);
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   break;
   case DLL_PROCESS_DETACH:
	cleanup();
	ExitInstance();
   break;
   case DLL_THREAD_ATTACH: break;
   case DLL_THREAD_DETACH: break;
  }
  return(true);
 }

 HRESULT WINAPI DirectDrawCreate(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter)
 {
  if(!gl_hOriginalDll) LoadOriginalDll(); // looking for the "right ddraw.dll"
                                          
                                          // Hooking DDRAW interface from Original Library IDirectDraw *pDD;

  typedef HRESULT(WINAPI* DirectDrawCreate_Type)(GUID FAR*, LPVOID*, IUnknown FAR*);
  DirectDrawCreate_Type DirectDrawCreate_fn = (DirectDrawCreate_Type)GetProcAddress(gl_hOriginalDll, _T("DirectDrawCreate"));
  if(!DirectDrawCreate_fn){::ExitProcess(0);}
  LPDIRECTDRAW7 FAR dummy;
  HRESULT h = DirectDrawCreate_fn(lpGUID, (LPVOID*)&dummy, pUnkOuter);
  *lplpDD = (LPDIRECTDRAW) new myIDDraw7(dummy);
  return (h);
 }

 void InitInstance(HANDLE hModule)
 {
	//Initialisation
	gl_hOriginalDll = NULL;
	gl_hThisInstance = NULL;

	//Storing Instance handle into global var
	gl_hThisInstance = (HINSTANCE)hModule;
 }
 void LoadOriginalDll(void)
 {
  char buffer[MAX_PATH];
  ::GetSystemDirectory(buffer, MAX_PATH);                       //Getting path to system dir and to d3d9.dll.
  strcat(buffer, "\\ddraw.dll");                                //Append dll name.
  if(!gl_hOriginalDll) gl_hOriginalDll = ::LoadLibrary(buffer);
  if(!gl_hOriginalDll){::ExitProcess(0);}                       //Debug - Exit the hard way.
 }
 void ExitInstance()
 {
  if(gl_hOriginalDll){::FreeLibrary(gl_hOriginalDll); gl_hOriginalDll = NULL;}
 }
//-//