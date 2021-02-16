#pragma once
#ifndef __MAIN_H__
#define __MAIN_H__
#endif

#include<bits/stdc++.h>
#include<random>
#include <atlconv.h>
using namespace std;

#include "extern/ExcelFormat.h"
using namespace ExcelFormat;

#include"utils.h"
#include"MCMF.h"
#include"ExcelReadWrite.h"
struct LECTURE {
	string name;
	string prof;
};

#ifndef __GLOBAL_VARIABLES__
#define __GLOBAL_VARIABLES__
extern char globalMsg[1010];
extern map<pair<int, int>, LECTURE> decomp;
extern map<string, int> prof2Idx;
extern map<string, int> lecPerProf;

extern vector<string> profList;
extern vector<vector<int>> preference;
extern int dayNodeNumber;
extern int totalLecCount;

extern const int labCount;
extern const int source;
extern const int sink;
extern const int dayCount;
extern const int profNodeBegin;
extern const int dayNodeBegin;
extern const int labNodeBegin;
extern vector<string> dayName;
extern vector<string> timeName;
extern vector<vector<int>> timetable;

extern MCMF mcmf;

extern random_device rd;
extern mt19937 gen;
#endif

typedef uniform_int_distribution<int> ud;