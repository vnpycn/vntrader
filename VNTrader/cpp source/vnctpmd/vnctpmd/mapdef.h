#pragma once

#include <iostream>
using  namespace std;
#include <string.h>
#define NULL 0
#include <math.h>
#include "def.h"

 

struct strategys
{
	char instructment[20];
	int type;
	int strategy[100];
};
extern strategys * its;

struct Instructment
{

	char instructment[20];
	int type;
	int MD_begin1;
	int MD_end1;
	int MD_begin2;
	int MD_end2;
	int MD_begin3;
	int MD_end3;
	int MD_begin4;
	int MD_end4;
	int MD_begin5;
	int MD_end5;

	int TD_begin1;
	int TD_end1;
	int TD_begin2;
	int TD_end2;
	int TD_begin3;
	int TD_end3;
	int TD_begin4;
	int TD_end4;
	int TD_begin5;
	int TD_end5;

};

extern Instructment * it;


struct hashinfo
{
	double usedrate;
	double max_len;
	double ave_len;
	int maxlen_num;
};

//排序用保存HASH的结构体
struct HashData
{
	int cycle;
	int key;
	int M;
	int N;
	int sort;
	int emaspace;
};
#include <time.h>
 
#define PERIOD_NUM   8
struct PERIODTYPE
{
  int periodtype[PERIOD_NUM];
 
};

