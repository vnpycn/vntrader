/*
1.本文件为VNTrader 期货CTP交易库底层代码
2.VNTrader及本C++库开源协议MIT协议

对VNPY开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.vnpy.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的vnpy库里。

官方网站：http://www.vnpy.cn
*/
#pragma once
#include "stdafx.h"
#include "IniFile.h"

#include "Interface.h"
#include "MdSpi.h"
#include "IniFile.h"
#include "mapdef.h"
#include <hash_map>
#include <map>
#include <string>
#include <algorithm>
#include "iostream"
//#include "VNCtpMD.h"
#include <iostream> 
#include <windows.h>
#include <thread>
using namespace std;
typedef map<string, PERIODTYPE> QS_Strategy_Map2;
QS_Strategy_Map2 * mapInstrument = new QS_Strategy_Map2;



/*
list <cmdcontent> cmdlist;
list <cmdcontent>::iterator cmd_Iter;

list <TThostFtdcInstrumentIDTypeStruct> ticknamelist;
list <TThostFtdcInstrumentIDTypeStruct>::iterator tickname_Iter;

list <CThostFtdcRspInfoField> errorlist;
list <CThostFtdcRspInfoField>::iterator error_Iter;

list <CThostFtdcRspUserLoginField> loginlist;
list <CThostFtdcRspUserLoginField>::iterator login_Iter;

list <CThostFtdcRspUserLoginField> loginfailelist;
list <CThostFtdcRspUserLoginField>::iterator loginfaile_Iter;

list <CThostFtdcUserLogoutField> loginoutlist;
list <CThostFtdcUserLogoutField>::iterator loginout_Iter;

list <int> connectlist;
list <int>::iterator connect_Iter;

list <CThostFtdcSpecificInstrumentField> subMarketlist;
list <CThostFtdcSpecificInstrumentField>::iterator subMarket_Iter;

list <CThostFtdcSpecificInstrumentField> unsubMarketlist;
list <CThostFtdcSpecificInstrumentField>::iterator unsubMarket_Iter;

list <CThostFtdcForQuoteRspField> forquotelist;
list <CThostFtdcForQuoteRspField>::iterator forquote_Iter;
*/


//#define LOG_LENGTH    100
//struct LOGSTRUCT
//{
//	char log[LOG_LENGTH];
//};
//list <LOGSTRUCT> loglist;
//list <LOGSTRUCT>::iterator log_Iter;




 
void LOG(char * msg)
{
	/*
	LOGSTRUCT tn;

	memset(&tn, 0, sizeof(LOGSTRUCT));
	_snprintf_s(tn.log, sizeof(tn.log), sizeof(tn.log), "%s", msg);
	EnterCriticalSection(&g_csdata);
	loglist.push_back(tn);
	LeaveCriticalSection(&g_csdata);
	SetEvent(hEvent[EID_OnLog]);

	*/
}





#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>
#include <iosfwd>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
using std::vector;
using std::string;
using std::wstring;
using std::iterator;
#ifdef UNICODE 
typedef wstring tstring;
#define lsprintf_s swprintf_s 
#else
typedef string tstring;
#define lsprintf_s sprintf_s 
#endif

#include<Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#include<Winsock2.h>
#pragma comment(lib,"Ws2_32.lib")


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#include <atlbase.h>
#include <atlstr.h>
#include "ResetMacClass\public\cwindowsexception.h"
#include "ResetMacClass\dev\CSetupComDi.h"
#include "ResetMacClass\public\mydebug.h"
#include "ResetMacClass\public\CConvertString.h"
#include "ResetMacClass\CGetPath.h"
#include "ResetMacClass\CChangeMac.h"
using std::cout;
using std::wcout;
using std::endl;
using std::cin;
using std::wcin;
#ifdef UNICODE
#define tcout wcout
#define tcin wcin
#else
#define tcout cout
#define tcin cin
#endif

strategys * its;
Instructment * it;
bool FindId2(const char *Instrument, const char * str)
{
	char * pdest1 = strstr((char*)Instrument, (char*)str);
	__int64  result1 = pdest1 - Instrument + 1;

	if (pdest1 != NULL)
	{	//printf("在%s发现%s\n", InstrumentID_n[id],str );
		return true;
	}
	else
	{
		//printf("%s 没有在%s发现\n", str, InstrumentID_n[id]);
		return false;
	}
}

int GetType(const char * InstrumentID)
{

	int TYPEID = -1;

	if (FindId2(InstrumentID, "ni"))
	{
		TYPEID = TYPE_NI;
	}
	else if (FindId2(InstrumentID, "zn"))
	{
		TYPEID = TYPE_ZN;
	}
	else if (FindId2(InstrumentID, "al"))
	{
		TYPEID = TYPE_AL;
	}
	else if (FindId2(InstrumentID, "cu"))
	{
		TYPEID = TYPE_CU;
	}
	else if (FindId2(InstrumentID, "au"))
	{
		TYPEID = TYPE_AU;
	}
	else if (FindId2(InstrumentID, "ag"))
	{
		TYPEID = TYPE_AG;
	}
	else if (FindId2(InstrumentID, "i"))
	{
		TYPEID = TYPE_I;
	}
	else if (FindId2(InstrumentID, "ru"))
	{
		TYPEID = TYPE_RU;
	}
	else if (FindId2(InstrumentID, "ta"))
	{
		TYPEID = TYPE_TA;
	}
	else if (FindId2(InstrumentID, "a"))
	{
		TYPEID = TYPE_A;
	}
	else if (FindId2(InstrumentID, "m"))
	{
		TYPEID = TYPE_M;
	}
	else if (FindId2(InstrumentID, "y"))
	{
		TYPEID = TYPE_Y;
	}
	else if (FindId2(InstrumentID, "p"))
	{
		TYPEID = TYPE_P;
	}
	else if (FindId2(InstrumentID, "rb"))
	{
		TYPEID = TYPE_RB;
	}
	else if (FindId2(InstrumentID, "ma"))
	{
		TYPEID = TYPE_MA;
	}
	else if (FindId2(InstrumentID, "pp"))
	{
		TYPEID = TYPE_PP;
	}
	else if (FindId2(InstrumentID, "cs"))
	{
		TYPEID = TYPE_CS;
	}
	else if (FindId2(InstrumentID, "jd"))
	{
		TYPEID = TYPE_JD;
	}
	else if (FindId2(InstrumentID, "bu"))
	{
		TYPEID = TYPE_BU;
	}
	else if (FindId2(InstrumentID, "fg"))
	{
		TYPEID = TYPE_FG;
	}
	else if (FindId2(InstrumentID, "l"))
	{
		TYPEID = TYPE_L;
	}
	else if (FindId2(InstrumentID, "v"))
	{
		TYPEID = TYPE_V;
	}
	else if (FindId2(InstrumentID, "sr"))
	{
		TYPEID = TYPE_SR;
	}
	else if (FindId2(InstrumentID, "rm"))
	{
		TYPEID = TYPE_RM;
	}
	else if (FindId2(InstrumentID, "cf"))
	{
		TYPEID = TYPE_CF;
	}
	else if (FindId2(InstrumentID, "c"))
	{
		TYPEID = TYPE_C;
	}
	else if (FindId2(InstrumentID, "wh"))
	{
		TYPEID = TYPE_WH;
	}
	else if (FindId2(InstrumentID, "sm"))
	{
		TYPEID = TYPE_SM;
	}
	else if (FindId2(InstrumentID, "sf"))
	{
		TYPEID = TYPE_SF;
	}
	else if (FindId2(InstrumentID, "ic"))
	{
		TYPEID = TYPE_IC;
	}
	else if (FindId2(InstrumentID, "if"))
	{
		TYPEID = TYPE_IF;
	}
	else if (FindId2(InstrumentID, "ih"))
	{
		TYPEID = TYPE_IH;
	}
	else if (FindId2(InstrumentID, "t"))
	{
		TYPEID = TYPE_T;
	}
	else if (FindId2(InstrumentID, "tf"))
	{
		TYPEID = TYPE_TF;
	}

	else if (FindId2(InstrumentID, "new1"))
	{
		TYPEID = TYPE_NEW1;
	}
	else if (FindId2(InstrumentID, "new2"))
	{
		TYPEID = TYPE_NEW2;
	}
	else if (FindId2(InstrumentID, "new3"))
	{
		TYPEID = TYPE_NEW3;
	}
	else if (FindId2(InstrumentID, "new4"))
	{
		TYPEID = TYPE_NEW4;
	}
	else if (FindId2(InstrumentID, "new5"))
	{
		TYPEID = TYPE_NEW5;
	}
	else if (FindId2(InstrumentID, "new6"))
	{
		TYPEID = TYPE_NEW6;
	}
	else if (FindId2(InstrumentID, "new7"))
	{
		TYPEID = TYPE_NEW7;
	}
	else if (FindId2(InstrumentID, "new8"))
	{
		TYPEID = TYPE_NEW8;
	}
	else if (FindId2(InstrumentID, "new9"))
	{
		TYPEID = TYPE_NEW9;
	}
	else if (FindId2(InstrumentID, "new10"))
	{
		TYPEID = TYPE_NEW10;
	}

	else if (FindId2(InstrumentID, "new11"))
	{
		TYPEID = TYPE_NEW11;
	}
	else if (FindId2(InstrumentID, "new12"))
	{
		TYPEID = TYPE_NEW12;
	}
	else if (FindId2(InstrumentID, "new13"))
	{
		TYPEID = TYPE_NEW13;
	}
	else if (FindId2(InstrumentID, "new14"))
	{
		TYPEID = TYPE_NEW14;
	}
	else if (FindId2(InstrumentID, "new15"))
	{
		TYPEID = TYPE_NEW15;
	}
	else if (FindId2(InstrumentID, "new16"))
	{
		TYPEID = TYPE_NEW16;
	}
	else if (FindId2(InstrumentID, "new17"))
	{
		TYPEID = TYPE_NEW17;
	}
	else if (FindId2(InstrumentID, "new18"))
	{
		TYPEID = TYPE_NEW18;
	}
	else if (FindId2(InstrumentID, "new19"))
	{
		TYPEID = TYPE_NEW19;
	}
	else if (FindId2(InstrumentID, "new20"))
	{
		TYPEID = TYPE_NEW20;
	}

	return TYPEID;

}




typedef map<string, InstrumentInfo> QS_Strategy_Map;
typedef hash_map<string, GuestOnlineHash> QS_Data_Map;

QS_Data_Map     mapData;
QS_Strategy_Map mapStrategy;




//暂时屏蔽2018
bool ReadInstrument()
{
	/*
	//printf("read Instrument.ini\n");
	LOG("read Instrument.ini");
	CIniFile	cfgfile("./Instrument.ini");
	int num = cfgfile.ReadInteger("number:", "num", 10);
	char instr[100] = { 0 };
	it = new Instructment[TYPE_ARR_LEN];
	memset(it, 0, sizeof(Instructment)*TYPE_ARR_LEN);


	for (int j = 1; j <= num; j++)
	{

	//map


	memset(&instr, 0, sizeof(instr));  //此处更新
	//	char list_num[40]={0};
	//	_snprintf(list_num, sizeof(list_num), "%d",j);
	//	MessageBox(NULL,list_num,"j",MB_OK);
	//_snprintf(qsid_id, sizeof(qsid_id), "qsid_%d", j);
	_snprintf_s(instr, sizeof(instr), sizeof(instr), "instrument%d", j);

	//   MessageBox(NULL,qsid_id,"qsid_id",MB_OK);
	//int qsid_zx = ini.ReadInteger(qsid_id, "qsid_zx", 0);
	//int qsid_jy = ini.ReadInteger(qsid_id, "qsid_jy", 0)

	// cfgfile.ReadString("合约品种", instr, "", it[i].instructment, dim(it[i].instructment));



	char temp1[31] = { 0 };
	char temp2[31] = { 0 };
	char temp3[31] = { 0 };
	cfgfile.ReadString(instr, "instrument", "", temp1, dim(temp1));

	if (0)
	{
	cfgfile.ReadString(instr, "strategy", "", temp2, dim(temp2));

	cfgfile.ReadString(instr, "positionrate", "", temp3, dim(temp3));
	InstrumentInfo iif;

	iif.tradestate = true;
	if (1)
	{
	////////////////////////////////////读取字段strategy

	//1.自己实现,线程安全,支持字符串作为分隔符.缺点可能就是代码量多.
	string str = temp2;// "google||twitter||facebook||microsoft||apple||ibmwdg";
	//printf("*************%s\n", temp2);
	//如果最后一个字符不是, 则添加","
	//std::string temp;
	//temp = str.substr(str.length() - 1, 1);
	if (str.substr(str.length() - 1, 1) != ",")
	{
	str = str + ",";
	}
	const char* delim = ",";
	size_t len = strlen(delim);
	size_t index = 0;
	size_t pos = str.find(delim, index);

	int id = -1;

	while (pos != string::npos)
	{
	id++;
	string ss = str.substr(index, pos - index);
	//cout << ss << endl;
	//cout << temp1 <<"["<< id + 1 << "] 定制了ID=" << ss << "的策略,请在文件strategy" << ss<<".py（或编译成strategy"<<ss<<".exe）中编写策略"<<endl;
	cout << temp1 << "[" << id + 1 << "] 定制了" << ss.c_str() << "的策略,请在文件" << ss.c_str() << "中编写策略" << endl;
	iif.strategyfile[id] = ss;// .c_str();// atoi(ss.c_str());//id
	index = pos + len;
	pos = str.find(delim, index);
	}



	//添加了“，”，此段可以省略
	//cout << "is last?" << " index:" << index << " str.length():" << str.length() << endl;
	//if ((index + 1) < str.length())
	//{
	//	id++;
	//	string ss = str.substr(index, str.length() - index);
	//	cout <<"["<<id<<"]" <<temp1<< "策略" <<ss << endl;
	////	iif.strategy[id] = atoi(ss.c_str());//id

	//}

	////////////////////////////////////读取字段strategy
	}
	int id = -1;
	if (1)
	{
	////////////////////////////////////读取字段position
	//1.自己实现,线程安全,支持字符串作为分隔符.缺点可能就是代码量多.
	string positionratestr = temp3;// "google||twitter||facebook||microsoft||apple||ibmwdg";
	//printf("*************%s\n", positionratestr.c_str());
	//printf("B2\n");
	//如果最后一个字符不是, 则添加","
	//std::string temp;
	//temp = str.substr(str.length() - 1, 1);
	if (positionratestr.substr(positionratestr.length() - 1, 1) != ",")
	{
	positionratestr = positionratestr + ",";
	}
	const char* delim = ",";
	size_t len = strlen(delim);
	size_t index = 0;
	size_t pos = positionratestr.find(delim, index);
	while (pos != string::npos)
	{
	id++;
	string ss = positionratestr.substr(index, pos - index);
	//cout << ss << endl;
	cout << temp1 << "[" << id + 1 << "] 仓位=" << ss.c_str() << "%" << endl;
	iif.position[id] = atoi(ss.c_str());//id
	index = pos + len;
	pos = positionratestr.find(delim, index);


	}
	////////////////////////////////////读取字段position

	}


	iif.strategyfilenum = id + 1;
	//printf("A5\n");
	if (0)
	{//暂时屏蔽
	printf("共[%d]个策略\n", iif.strategyfilenum);
	}

	pair<string, InstrumentInfo>value(temp1, iif);
	mapStrategy.insert(value);

	}



	//map

	//int TYPEIDXB = j;// GetType(temp) - 100000;
	// printf("A2\n");
	//printf("%s", temp1);
	//strncpy_s(its[TYPEIDXB].instructment, sizeof(its[TYPEIDXB].instructment), temp1, strlen(temp1));
	//printf("A3\n");

	//memset(its[TYPEIDXB].instructment,0,20);
	//_snprintf_s(its[TYPEIDXB].instructment,20,19, temp1, strlen(temp1));
	//printf("A4\n");
	if (strcmp(temp1, "") != 0)
	{
	Subscribe(temp1);
	}
	//printf("订阅[%s]\n", temp1);
	//strncpy_s(its[TYPEIDXB].instructment, sizeof(its[TYPEIDXB].instructment), temp1, strlen(temp1));
	//printf("A7\n");
	//its[TYPEIDXB].strategy[0] = 1;
	//Sleep(5000);
	//printf("A8\n");
	//printf("***************\n");



	///char instrument_MD[30] = { 0 };
	/char instrument_TRADE[30] = { 0 };
	//_snprintf_s(instrument_MD, sizeof(instrument_MD), sizeof(instrument_MD), "%s行情", it[TYPEIDXB].instructment);
	//_snprintf_s(instrument_TRADE, sizeof(instrument_TRADE), sizeof(instrument_TRADE), "%s交易", it[TYPEIDXB].instructment);



	//	printf("%s\n", instrument_MD);

	//cfgfile.ReadString("ag", "begin1", NULL, begintime1, dim(begintime1));
	//cfgfile.ReadString("ag", "end1", NULL, endtime1, dim(endtime1));
	//cfgfile.ReadString(instrument_MD, "begin1", "", begintime1, dim(begintime1));
	//printf("A30\n");
	//cfgfile.ReadString(instrument_MD, "end1", "", endtime1, dim(endtime1));


	//its[TYPEIDXB].MD_begin1 = cfgfile.ReadInt(instrument_MD, "begin1", 0);
	//its[TYPEIDXB].MD_end1 = cfgfile.ReadInt(instrument_MD, "end1", 0);




	}
	*/
	return true;
}


//暂时屏蔽
void ReadTradeTime()
{
	/*
	//char	filename[1024];
	//sprintf(filename, "%s\\qlserver.ini", m_path);
	//	CProcRes*	m_ProcRes;
	//m_ProcRes->SetCfgFile("qlserver.ini");

	//	char	dllfile[512];
	//char	titlefile[512];
	CIniFile	cfgfile("./TradeTime.ini");
	//char TCP[256];
	//CHAR IPX[256];
	//cfgfile.ReadString("本机设置", "TCP端口", "24929", TCP, dim(TCP));
	//cfgfile.ReadString("本机设置", "SPX端口", "", IPX, dim(IPX));

	//long m_CheckSelectStatusTime = cfgfile.ReadInteger("本机设置", "CheckSelectStatusTime", 100);

	int showstate = cfgfile.ReadInteger("时间段配置文件读取显示", "是否显示读取过程", 1);

	int num=cfgfile.ReadInteger("合约数量", "num",10);
	if (showstate == 1)
	{
	//printf("读取合约配置数量[%d]\n", num);
	char str[100] = { 0 };
	_snprintf_s(str,sizeof(str),sizeof(str),"读取合约配置数量[%d]", num);
	LOG(str);
	}


	it = new Instructment[TYPE_ARR_LEN];
	memset(it, 0, sizeof(Instructment)*TYPE_ARR_LEN);


	for (int j = 0; j < num; j++)
	{
	char instr[16] = {0};
	if (showstate == 1)
	{
	_snprintf_s(instr, sizeof(instr), sizeof(instr), "合约%d", j + 1);
	}
	// cfgfile.ReadString("合约品种", instr, "", it[i].instructment, dim(it[i].instructment));
	char temp[20] = {0};
	cfgfile.ReadString("合约品种", instr, "", temp, dim(temp));

	int TYPEIDXB = j;// GetType(temp) - 100000;
	strncpy_s(it[TYPEIDXB].instructment,sizeof(it[TYPEIDXB].instructment),temp,strlen(temp));


	//printf("A2\n");
	char instrument_MD[30] = { 0 };
	char instrument_TRADE[30] = { 0 };
	_snprintf_s(instrument_MD, sizeof(instrument_MD), sizeof(instrument_MD), "%s行情", it[TYPEIDXB].instructment);
	_snprintf_s(instrument_TRADE, sizeof(instrument_TRADE), sizeof(instrument_TRADE), "%s交易", it[TYPEIDXB].instructment);
	char	begintime1[40] = { 0 };
	char	endtime1[40] = { 0 };
	char	begintime2[40] = { 0 };
	char	endtime2[40] = { 0 };
	char	begintime3[40] = { 0 };
	char	endtime3[40] = { 0 };
	char	begintime4[40] = { 0 };
	char	endtime4[40] = { 0 };



	cfgfile.ReadString(instrument_MD, "begin1", "", begintime1, dim(begintime1));
	//printf("A30\n");
	cfgfile.ReadString(instrument_MD, "end1", "", endtime1, dim(endtime1));
	//printf("A31\n");
	cfgfile.ReadString(instrument_MD, "begin2", "", begintime2, dim(begintime2));
	cfgfile.ReadString(instrument_MD, "end2", "", endtime2, dim(endtime2));
	//printf("A32\n");
	cfgfile.ReadString(instrument_MD, "begin3", "", begintime3, dim(begintime3));
	cfgfile.ReadString(instrument_MD, "end3", "", endtime3, dim(endtime3));
	//printf("A33\n");
	cfgfile.ReadString(instrument_MD, "begin4", "", begintime4, dim(begintime4));
	cfgfile.ReadString(instrument_MD, "end4", "", endtime4, dim(endtime4));

	//printf("A4\n");

	it[TYPEIDXB].MD_begin1 = cfgfile.ReadInt(instrument_MD, "begin1", 0);
	it[TYPEIDXB].MD_end1 = cfgfile.ReadInt(instrument_MD, "end1", 0);

	it[TYPEIDXB].MD_begin2 = cfgfile.ReadInt(instrument_MD, "begin2", 0);
	it[TYPEIDXB].MD_end2 = cfgfile.ReadInt(instrument_MD, "end2", 0);

	it[TYPEIDXB].MD_begin3 = cfgfile.ReadInt(instrument_MD, "begin3", 0);
	it[TYPEIDXB].MD_end3 = cfgfile.ReadInt(instrument_MD, "end3", 0);

	it[TYPEIDXB].MD_begin4 = cfgfile.ReadInt(instrument_MD, "begin4", 0);
	it[TYPEIDXB].MD_end4 = cfgfile.ReadInt(instrument_MD, "end4", 0);



	if (showstate == 1)
	{ //行情
	printf("--------%s--------\n", instrument_MD);
	//it[TYPEIDXB].instructment
	// printf("[%s ~ %s]\n",  begintime1, endtime1);
	// printf("[%s ~ %s]\n",  begintime2, endtime2);
	// printf("[%s ~ %s]\n",  begintime3, endtime3);
	// printf("[%s ~ %s]\n",  begintime4, endtime4);

	char str[100] = { 0 };
	_snprintf_s(str, sizeof(str), sizeof(str), "[%s ~ %s]",  begintime1, endtime1);
	LOG(str);
	memset(str,0,sizeof(str));
	_snprintf_s(str, sizeof(str), sizeof(str), "[%s ~ %s]", begintime2, endtime2);
	LOG(str);
	memset(str, 0, sizeof(str));
	_snprintf_s(str, sizeof(str), sizeof(str), "[%s ~ %s]", begintime3, endtime3);
	LOG(str);
	memset(str, 0, sizeof(str));
	_snprintf_s(str, sizeof(str), sizeof(str), "[%s ~ %s]", begintime4, endtime4);
	LOG(str);
	}

	memset(begintime1, 0, sizeof(begintime1));
	memset(begintime2, 0, sizeof(begintime2));
	memset(begintime3, 0, sizeof(begintime3));
	memset(begintime4, 0, sizeof(begintime4));
	memset(endtime1, 0, sizeof(endtime1));
	memset(endtime2, 0, sizeof(endtime2));
	memset(endtime3, 0, sizeof(endtime3));
	memset(endtime4, 0, sizeof(endtime4));

	// printf("A6\n");

	cfgfile.ReadString(instrument_TRADE, "begin1", "", begintime1, dim(begintime1));
	cfgfile.ReadString(instrument_TRADE, "end1", "", endtime1, dim(endtime1));

	cfgfile.ReadString(instrument_TRADE, "begin2", "", begintime2, dim(begintime2));
	cfgfile.ReadString(instrument_TRADE, "end2", "", endtime2, dim(endtime2));

	cfgfile.ReadString(instrument_TRADE, "begin3", "", begintime3, dim(begintime3));
	cfgfile.ReadString(instrument_TRADE, "end3", "", endtime3, dim(endtime3));

	cfgfile.ReadString(instrument_TRADE, "begin4", "", begintime4, dim(begintime4));
	cfgfile.ReadString(instrument_TRADE, "end4", "", endtime4, dim(endtime4));


	it[TYPEIDXB].TD_begin1 = cfgfile.ReadInt(instrument_TRADE, "begin1", 0);
	it[TYPEIDXB].TD_end1 = cfgfile.ReadInt(instrument_TRADE, "end1", 0);

	it[TYPEIDXB].TD_begin2 = cfgfile.ReadInt(instrument_TRADE, "begin2", 0);
	it[TYPEIDXB].TD_end2 = cfgfile.ReadInt(instrument_TRADE, "end2", 0);

	it[TYPEIDXB].TD_begin3 = cfgfile.ReadInt(instrument_TRADE, "begin3", 0);
	it[TYPEIDXB].TD_end3 = cfgfile.ReadInt(instrument_TRADE, "end3", 0);

	it[TYPEIDXB].TD_begin4 = cfgfile.ReadInt(instrument_TRADE, "begin4", 0);
	it[TYPEIDXB].TD_end4 = cfgfile.ReadInt(instrument_TRADE, "end4", 0);


	if (showstate == 1)
	{//交易
	printf("--------%s--------\n", instrument_TRADE);
	printf("[%s ~ %s]\n",  begintime1, endtime1);
	printf("[%s ~ %s]\n",  begintime2, endtime2);
	printf("[%s ~ %s]\n",  begintime3, endtime3);
	printf("[%s ~ %s]\n",  begintime4, endtime4);



	}


	}
	if (showstate == 1)
	{
	//printf("**********TradeTime.ini配置文件读取结束************\n");

	LOG("TradeTime.ini配置文件读取结束");
	}


	*/
}


 
//TradeTime
struct TradeTimesInterval
{
	double begin1;
	double end1;
	double begin2;
	double end2;
	double begin3;
	double end3;
};
TradeTimesInterval TradeTime[100];
 

std::string gFrontAddr[3];
std::string gBrokerID;
std::string gInvestorID;
std::string gPassword;
std::string sn;

char* ppInstrumentID[200] = {0};	

CThostFtdcDepthMarketDataField *depthdata[200] = {0};
int size = sizeof(CThostFtdcDepthMarketDataField);
int amount = 0;
std::map<std::string, int> gMarket;

std::string gListFile("List.ini");
//std::string gMDAccount("QuickLibMD.ini");

std::string gLic("Lic.ini");

std::string gTradeTime("TradeTime.ini");

int gStatus = 0;
CMdSpi gMDSpi;

 
 


#include <time.h>
#include <windows.h>
 

struct Mdata
{
 int  processid;        //进程ID
 char InstrumentID[10]; //品种
};


unsigned nThreadID_OnFrontConnected;
unsigned nThreadID_OnFrontDisconnected;
unsigned nThreadID_OnRspUserLogin;
unsigned nThreadID_OnRspUserLogout;
unsigned nThreadID_OnRspQryInvestorPosition;
unsigned nThreadID_OnRspQryTradingAccount;
unsigned nThreadID_OnRtnOrder;
unsigned nThreadID_OnRtnTrade;

unsigned nThreadID_OnRspSubMarketData;
unsigned nThreadID_OnRspUnSubMarketData;
unsigned nThreadID_OnRtnDepthMarketData;


HANDLE hStartEvent_OnFrontConnected;
HANDLE hStartEvent_OnFrontDisconnected;
HANDLE hStartEvent_OnRspUserLogin;
HANDLE hStartEvent_OnRspUserLogout;
HANDLE hStartEvent_OnRspQryInvestorPosition;
HANDLE hStartEvent_OnRspQryTradingAccount;
HANDLE hStartEvent_OnRtnOrder;
HANDLE hStartEvent_OnRtnTrade;

HANDLE hStartEvent_OnRspSubMarketData;
HANDLE hStartEvent_OnRspUnSubMarketData;
HANDLE hStartEvent_OnRtnDepthMarketData;
bool Start()
{


	ReadTradeTime();


	//BOOL ret = gMDSpi.Init();



	//if (ret) 
	//{
	//	LOG("Init scuess.");
	 	return TRUE;
	//}else
	//{
	//	LOG("Init failer.");
	//	return FALSE;
	//}
}




int UnSubscribeMarketData( char *InstrumentID)
{
	return 	gMDSpi.UnSubscribeMarketData();

	//char * crcvalue = NULL;
	if (mapInstrument)
	{
	
		delete mapInstrument;
		mapInstrument = NULL;
	}

	ppInstrumentID[amount] = new TThostFtdcInstrumentIDType;
	memset(ppInstrumentID[amount],0,sizeof(TThostFtdcInstrumentIDType));

	::strcpy_s(ppInstrumentID[amount],sizeof(TThostFtdcInstrumentIDType), InstrumentID);
	//::strcpy_s(ppInstrumentID[amount],sizeof(ppInstrumentID[amount]), contract);
	gMarket[InstrumentID] = amount;
	depthdata[amount] = new CThostFtdcDepthMarketDataField;
	memset(depthdata[amount],0,sizeof(CThostFtdcDepthMarketDataField));

	++amount;
	gMDSpi.UnSubscribeMarketData();

}

//询价
void SubscribeForQuoteRsp(  char *InstrumentID)
{
	/*
	char * crcvalue = NULL;

	ppInstrumentID[amount] = new TThostFtdcInstrumentIDType;
	::strcpy(ppInstrumentID[amount], InstrumentID);
	//::strcpy_s(ppInstrumentID[amount],sizeof(ppInstrumentID[amount]), contract);
	gMarket[InstrumentID] = amount;
	//data[amount] = new CThostFtdcDepthMarketDataField;
	depthdata[amount] = new CThostFtdcDepthMarketDataField;
	++amount;
	*/
	gMDSpi.SubscribeForQuoteRsp(InstrumentID);

}
//添加记录, 
bool FindId(const char *Instrument, const char * str)
{
	char * pdest1 = strstr((char*)Instrument, (char*)str);
	__int64  result1 = pdest1 - Instrument + 1;

	if (pdest1 != NULL)
	{		//printf("在%s发现%s\n", InstrumentID_n[id],str );
		return true;
	}
	else
	{
		//printf("%s 没有在%s发现\n", str, InstrumentID_n[id]);
		return false;
	}
}
extern void InitGuestOnlineHash(GuestOnlineHash * q);
bool Add(const char *InstrumentID, const PERIODTYPE * pt)
{
	std::hash_map<string, GuestOnlineHash>::iterator it;
	it = mapData.find(InstrumentID);
	if (it == mapData.end())
	{
		//printf("*************没找到该合约的%s\n", InstrumentID);
		//GuestOnlineHash value;
		//InitGuestOnlineHash(&value);
		//mapData.insert(std::make_pair(InstrumentID, value));
		//ClearGuestOnlineHash(&value);
		//it = mapData.find(InstrumentID);


		GuestOnlineHash pi;
		InitGuestOnlineHash(&pi);
		pair<string, GuestOnlineHash>value2(InstrumentID, pi);
		mapData.insert(value2);



	}
	it = mapData.find(InstrumentID);
	if (it != mapData.end())
	{
		GuestOnlineHash * q = &(it->second);
			if (pt)
			{

				for (int i = 0; i < PERIOD_NUM; i++)
				{


					if (pt->periodtype[i])
					{
						switch (pt->periodtype[i])
						{
						case QL_ALL:
							q->period_M1 = true;
							q->period_M3 = true;
							q->period_M5 = true;
							q->period_M10 = true;
							q->period_M15 = true;
							q->period_M30 = true;
							q->period_M60 = true;
							q->period_D1 = true;
							printf("%s加入M1,M3,M5,M10,M15,M30,M60,D1周期\n", InstrumentID);
							break;
						case QL_M1:
							q->period_M1 = true;
							printf("%s加入M1周期\n", InstrumentID);
							break;
						case QL_M3:
							q->period_M3 = true;
							printf("%s加入M3周期\n", InstrumentID);
							break;
						case QL_M5:
							q->period_M5 = true;
							printf("%s加入M5周期\n", InstrumentID);
							break;

						case QL_M10:
							q->period_M10 = true;
							printf("%s加入M10周期\n", InstrumentID);
							break;

						case QL_M15:
							q->period_M15 = true;
							printf("%s加入M15周期\n", InstrumentID);
							break;

						case QL_M30:
							q->period_M30 = true;
							printf("%s加入M30周期\n", InstrumentID);
							break;

						case QL_M60:
							q->period_M60 = true;
							printf("%s加入M60周期\n", InstrumentID);
							break;

						case QL_M120:
							q->period_M120 = true;
							printf("%s加入M120周期\n", InstrumentID);
							break;

						case QL_D1:
							q->period_D1 = true;
							printf("%s加入D1周期\n", InstrumentID);
							break;
						}

					}

				}
			}


			if (FindId(InstrumentID, "ni"))
			{
				q->TYPEID = TYPE_NI;
			}
			else if (FindId(InstrumentID, "zn"))
			{
				q->TYPEID = TYPE_ZN;
			}
			else if (FindId(InstrumentID, "al"))
			{
				q->TYPEID = TYPE_AL;
			}
			else if (FindId(InstrumentID, "cu"))
			{
				q->TYPEID = TYPE_CU;
			}
			else if (FindId(InstrumentID, "au"))
			{
				q->TYPEID = TYPE_AU;
			}
			else if (FindId(InstrumentID, "ag"))
			{
				q->TYPEID = TYPE_AG;
			}
			else if (FindId(InstrumentID, "i"))
			{
				q->TYPEID = TYPE_I;
			}
			else if (FindId(InstrumentID, "ru"))
			{
				q->TYPEID = TYPE_RU;
			}
			else if (FindId(InstrumentID, "ta"))
			{
				q->TYPEID = TYPE_TA;
			}
			else if (FindId(InstrumentID, "a"))
			{
				q->TYPEID = TYPE_A;
			}
			else if (FindId(InstrumentID, "m"))
			{
				q->TYPEID = TYPE_M;
			}
			else if (FindId(InstrumentID, "y"))
			{
				q->TYPEID = TYPE_Y;
			}
			else if (FindId(InstrumentID, "p"))
			{
				q->TYPEID = TYPE_P;
			}
			else if (FindId(InstrumentID, "rb"))
			{
				q->TYPEID = TYPE_RB;
			}
			else if (FindId(InstrumentID, "ma"))
			{
				q->TYPEID = TYPE_MA;
			}
			else if (FindId(InstrumentID, "pp"))
			{
				q->TYPEID = TYPE_PP;
			}
			else if (FindId(InstrumentID, "cs"))
			{
				q->TYPEID = TYPE_CS;
			}
			else if (FindId(InstrumentID, "jd"))
			{
				q->TYPEID = TYPE_JD;
			}
			else if (FindId(InstrumentID, "bu"))
			{
				q->TYPEID = TYPE_BU;
			}
			else if (FindId(InstrumentID, "fg"))
			{
				q->TYPEID = TYPE_FG;
			}
			else if (FindId(InstrumentID, "l"))
			{
				q->TYPEID = TYPE_L;
			}
			else if (FindId(InstrumentID, "v"))
			{
				q->TYPEID = TYPE_V;
			}
			else if (FindId(InstrumentID, "sr"))
			{
				q->TYPEID = TYPE_SR;
			}
			else if (FindId(InstrumentID, "rm"))
			{
				q->TYPEID = TYPE_RM;
			}
			else if (FindId(InstrumentID, "cf"))
			{
				q->TYPEID = TYPE_CF;
			}
			else if (FindId(InstrumentID, "c"))
			{
				q->TYPEID = TYPE_C;
			}
			else if (FindId(InstrumentID, "wh"))
			{
				q->TYPEID = TYPE_WH;
			}
			else if (FindId(InstrumentID, "sm"))
			{
				q->TYPEID = TYPE_SM;
			}
			else if (FindId(InstrumentID, "sf"))
			{
				q->TYPEID = TYPE_SF;
			}
			else if (FindId(InstrumentID, "ic"))
			{
				q->TYPEID = TYPE_IC;
			}
			else if (FindId(InstrumentID, "if"))
			{
				q->TYPEID = TYPE_IF;
			}
			else if (FindId(InstrumentID, "ih"))
			{
				q->TYPEID = TYPE_IH;
			}
			else if (FindId(InstrumentID, "t"))
			{
				q->TYPEID = TYPE_T;
			}
			else if (FindId(InstrumentID, "tf"))
			{
				q->TYPEID = TYPE_TF;
			}

			else if (FindId(InstrumentID, "new1"))
			{
				q->TYPEID = TYPE_NEW1;
			}
			else if (FindId(InstrumentID, "new2"))
			{
				q->TYPEID = TYPE_NEW2;
			}
			else if (FindId(InstrumentID, "new3"))
			{
				q->TYPEID = TYPE_NEW3;
			}
			else if (FindId(InstrumentID, "new4"))
			{
				q->TYPEID = TYPE_NEW4;
			}
			else if (FindId(InstrumentID, "new5"))
			{
				q->TYPEID = TYPE_NEW5;
			}
			else if (FindId(InstrumentID, "new6"))
			{
				q->TYPEID = TYPE_NEW6;
			}
			else if (FindId(InstrumentID, "new7"))
			{
				q->TYPEID = TYPE_NEW7;
			}
			else if (FindId(InstrumentID, "new8"))
			{
				q->TYPEID = TYPE_NEW8;
			}
			else if (FindId(InstrumentID, "new9"))
			{
				q->TYPEID = TYPE_NEW9;
			}
			else if (FindId(InstrumentID, "new10"))
			{
				q->TYPEID = TYPE_NEW10;
			}

			else if (FindId(InstrumentID, "new11"))
			{
				q->TYPEID = TYPE_NEW11;
			}
			else if (FindId(InstrumentID, "new12"))
			{
				q->TYPEID = TYPE_NEW12;
			}
			else if (FindId(InstrumentID, "new13"))
			{
				q->TYPEID = TYPE_NEW13;
			}
			else if (FindId(InstrumentID, "new14"))
			{
				q->TYPEID = TYPE_NEW14;
			}
			else if (FindId(InstrumentID, "new15"))
			{
				q->TYPEID = TYPE_NEW15;
			}
			else if (FindId(InstrumentID, "new16"))
			{
				q->TYPEID = TYPE_NEW16;
			}
			else if (FindId(InstrumentID, "new17"))
			{
				q->TYPEID = TYPE_NEW17;
			}
			else if (FindId(InstrumentID, "new18"))
			{
				q->TYPEID = TYPE_NEW18;
			}
			else if (FindId(InstrumentID, "new19"))
			{
				q->TYPEID = TYPE_NEW19;
			}
			else if (FindId(InstrumentID, "new20"))
			{
				q->TYPEID = TYPE_NEW20;
			}

			/*
			else
			{
				//WirteSingleRecordToFile8(id, "K16");
				//cout << "无此记录，添加记录\n" << endl;
				//printf("(A)添加记录 [%d]%s  keystr:[%s] key[%d]\n", id, InstrumentID_n[id], keystr, key);
				GuestOnlineHash *temp = new GuestOnlineHash;
				//DWORD dwTick = ::GetTickCount();
				//WirteSingleRecordToFile8(id, "K17");
				//temp->LoginErrorTime=NULL;
				//temp->LoginErrorNum=0;
				//temp->dwLastActiveTime= dwTick;			 // 记录此用户的活动时间（服务器使用）

				//temp->next = HashGuestOnlineList[key]->next;
				//HashGuestOnlineList[key]->next = temp;

				if (pt)
				{

					for (int i = 0; i < PERIOD_NUM; i++)
					{
						//printf("yyyyyyyyyyyyyyyyyyyyyyy3\n");
						//printf("(%d)[%d][%d][%d][%d]\n",i, pt->periodtype[i], QL_M1, QL_M3, QL_M5);

						if (pt->periodtype[i])
						{
							switch (pt->periodtype[i])
							{
							case QL_ALL:
								temp->period_M1 = true;
								temp->period_M3 = true;
								temp->period_M5 = true;
								temp->period_M10 = true;
								temp->period_M15 = true;
								temp->period_M30 = true;
								temp->period_M60 = true;
								temp->period_D1 = true;
								printf("%s加入M1,M3,M5,M10,M15,M30,M60,D1周期\n", InstrumentID);
								break;
							case QL_M1:
								temp->period_M1 = true;
								printf("%s加入M1周期\n", InstrumentID);
								break;
							case QL_M3:
								temp->period_M3 = true;
								printf("%s加入M3周期\n", InstrumentID);
								break;
							case QL_M5:
								temp->period_M5 = true;
								printf("%s加入M5周期\n", InstrumentID);
								break;

							case QL_M10:
								temp->period_M10 = true;
								printf("%s加入M10周期\n", InstrumentID);
								break;

							case QL_M15:
								temp->period_M15 = true;
								printf("%s加入M15周期\n", InstrumentID);
								break;

							case QL_M30:
								temp->period_M30 = true;
								printf("%s加入M30周期\n", InstrumentID);
								break;

							case QL_M60:
								temp->period_M60 = true;
								printf("%s加入M60周期\n", InstrumentID);
								break;

							case QL_M120:
								temp->period_M120 = true;
								printf("%s加入M120周期\n", InstrumentID);
								break;

							case QL_D1:
								temp->period_D1 = true;
								printf("%s加入D1周期\n", InstrumentID);
								break;
							}

						}

					}
				}



				//添加的
				//temp->CROSS_SINGLE_TURN[id][0] = turn;
				//temp->CROSS_SINGLE_PRICE[id][0] = NewPrice;
				//temp->CROSS_SINGLE_SPACE_VALUE[id][0] = fabs(q->CROSS_SINGLE_PRICE[id][1] - q->CROSS_SINGLE_PRICE[id][0]);
				//temp->periodtype1 = periodtype;
				//temp->CROSS_SINGLE_SPACE_PER_M1[id][0] = (q->CROSS_SINGLE_SPACE_VALUE[id][1] - q->CROSS_SINGLE_SPACE_VALUE[id][0]) / (10000 * (q->CROSS_SINGLE_TIME[id][1] - q->CROSS_SINGLE_TIME[id][0]));
				//添加的
				strncpy_s(temp->InstrumentID, sizeof(temp->InstrumentID), InstrumentID, strlen(InstrumentID));
				temp->InstrumentID[strlen(InstrumentID)] = '\0';



				if (FindId(InstrumentID, "ni"))
				{
					temp->TYPEID = TYPE_NI;
				}
				else if (FindId(InstrumentID, "zn"))
				{
					temp->TYPEID = TYPE_ZN;
				}
				else if (FindId(InstrumentID, "al"))
				{
					temp->TYPEID = TYPE_AL;
				}
				else if (FindId(InstrumentID, "cu"))
				{
					temp->TYPEID = TYPE_CU;
				}
				else if (FindId(InstrumentID, "au"))
				{
					temp->TYPEID = TYPE_AU;
				}
				else if (FindId(InstrumentID, "ag"))
				{
					temp->TYPEID = TYPE_AG;
				}
				else if (FindId(InstrumentID, "i"))
				{
					temp->TYPEID = TYPE_I;
				}
				else if (FindId(InstrumentID, "ru"))
				{
					temp->TYPEID = TYPE_RU;
				}
				else if (FindId(InstrumentID, "ta"))
				{
					temp->TYPEID = TYPE_TA;
				}
				else if (FindId(InstrumentID, "a"))
				{
					temp->TYPEID = TYPE_A;
				}
				else if (FindId(InstrumentID, "m"))
				{
					temp->TYPEID = TYPE_M;
				}
				else if (FindId(InstrumentID, "y"))
				{
					temp->TYPEID = TYPE_Y;
				}
				else if (FindId(InstrumentID, "p"))
				{
					temp->TYPEID = TYPE_P;
				}
				else if (FindId(InstrumentID, "rb"))
				{
					temp->TYPEID = TYPE_RB;
				}
				else if (FindId(InstrumentID, "ma"))
				{
					temp->TYPEID = TYPE_MA;
				}
				else if (FindId(InstrumentID, "pp"))
				{
					temp->TYPEID = TYPE_PP;
				}
				else if (FindId(InstrumentID, "cs"))
				{
					temp->TYPEID = TYPE_CS;
				}
				else if (FindId(InstrumentID, "jd"))
				{
					temp->TYPEID = TYPE_JD;
				}
				else if (FindId(InstrumentID, "bu"))
				{
					temp->TYPEID = TYPE_BU;
				}
				else if (FindId(InstrumentID, "fg"))
				{
					temp->TYPEID = TYPE_FG;
				}
				else if (FindId(InstrumentID, "l"))
				{
					temp->TYPEID = TYPE_L;
				}
				else if (FindId(InstrumentID, "v"))
				{
					temp->TYPEID = TYPE_V;
				}
				else if (FindId(InstrumentID, "sr"))
				{
					temp->TYPEID = TYPE_SR;
				}
				else if (FindId(InstrumentID, "rm"))
				{
					temp->TYPEID = TYPE_RM;
				}
				else if (FindId(InstrumentID, "cf"))
				{
					temp->TYPEID = TYPE_CF;
				}
				else if (FindId(InstrumentID, "c"))
				{
					temp->TYPEID = TYPE_C;
				}
				else if (FindId(InstrumentID, "wh"))
				{
					temp->TYPEID = TYPE_WH;
				}
				else if (FindId(InstrumentID, "sm"))
				{
					temp->TYPEID = TYPE_SM;
				}
				else if (FindId(InstrumentID, "sf"))
				{
					temp->TYPEID = TYPE_SF;
				}
				else if (FindId(InstrumentID, "ic"))
				{
					temp->TYPEID = TYPE_IC;
				}
				else if (FindId(InstrumentID, "if"))
				{
					temp->TYPEID = TYPE_IF;
				}
				else if (FindId(InstrumentID, "ih"))
				{
					temp->TYPEID = TYPE_IH;
				}
				else if (FindId(InstrumentID, "t"))
				{
					temp->TYPEID = TYPE_T;
				}
				else if (FindId(InstrumentID, "tf"))
				{
					temp->TYPEID = TYPE_TF;
				}

				else if (FindId(InstrumentID, "new1"))
				{
					temp->TYPEID = TYPE_NEW1;
				}
				else if (FindId(InstrumentID, "new2"))
				{
					temp->TYPEID = TYPE_NEW2;
				}
				else if (FindId(InstrumentID, "new3"))
				{
					temp->TYPEID = TYPE_NEW3;
				}
				else if (FindId(InstrumentID, "new4"))
				{
					temp->TYPEID = TYPE_NEW4;
				}
				else if (FindId(InstrumentID, "new5"))
				{
					temp->TYPEID = TYPE_NEW5;
				}
				else if (FindId(InstrumentID, "new6"))
				{
					temp->TYPEID = TYPE_NEW6;
				}
				else if (FindId(InstrumentID, "new7"))
				{
					temp->TYPEID = TYPE_NEW7;
				}
				else if (FindId(InstrumentID, "new8"))
				{
					temp->TYPEID = TYPE_NEW8;
				}
				else if (FindId(InstrumentID, "new9"))
				{
					temp->TYPEID = TYPE_NEW9;
				}
				else if (FindId(InstrumentID, "new10"))
				{
					temp->TYPEID = TYPE_NEW10;
				}

				else if (FindId(InstrumentID, "new11"))
				{
					temp->TYPEID = TYPE_NEW11;
				}
				else if (FindId(InstrumentID, "new12"))
				{
					temp->TYPEID = TYPE_NEW12;
				}
				else if (FindId(InstrumentID, "new13"))
				{
					temp->TYPEID = TYPE_NEW13;
				}
				else if (FindId(InstrumentID, "new14"))
				{
					temp->TYPEID = TYPE_NEW14;
				}
				else if (FindId(InstrumentID, "new15"))
				{
					temp->TYPEID = TYPE_NEW15;
				}
				else if (FindId(InstrumentID, "new16"))
				{
					temp->TYPEID = TYPE_NEW16;
				}
				else if (FindId(InstrumentID, "new17"))
				{
					temp->TYPEID = TYPE_NEW17;
				}
				else if (FindId(InstrumentID, "new18"))
				{
					temp->TYPEID = TYPE_NEW18;
				}
				else if (FindId(InstrumentID, "new19"))
				{
					temp->TYPEID = TYPE_NEW19;
				}
				else if (FindId(InstrumentID, "new20"))
				{
					temp->TYPEID = TYPE_NEW20;
				}


				//strncpy_s(temp->keystr, sizeof(temp->keystr), keystr, strlen(keystr));
				//temp->keystr[strlen(keystr)] = '\0';
				//strncpy_s(temp->checkcode, sizeof(temp->checkcode), checkcode, strlen(checkcode));
				//temp->checkcode[strlen(checkcode)] = '\0';
				//WirteSingleRecordToFile8(id, "K20");
				//printf("A7\n");
			}
			*/
			return true;
		//}
	}

	return false;
	//strncpy_s(temp->InstrumentID, sizeof(temp->InstrumentID), InstrumentID, strlen(InstrumentID));
	//temp->InstrumentID[strlen(InstrumentID)] = '\0';




}
char * wchar2char(const wchar_t* wchar)
{
	char * m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
int SubscribeMarketData(char *InstrumentID)
{
	return gMDSpi.SubscribeMarketData( (InstrumentID));

	//return gMDSpi.SubscribeMarketData(wchar2char(InstrumentID));
 /*
	//char * crcvalue = NULL;
	Add(InstrumentID, NULL);
	ppInstrumentID[amount] = new TThostFtdcInstrumentIDType;
	::strcpy_s(ppInstrumentID[amount],sizeof(TThostFtdcInstrumentIDType), InstrumentID);
	//::strcpy_s(ppInstrumentID[amount],sizeof(ppInstrumentID[amount]), contract);
	gMarket[InstrumentID] = amount;
	//data[amount] = new CThostFtdcDepthMarketDataField;
	depthdata[amount] = new CThostFtdcDepthMarketDataField;
	++amount;
	gMDSpi.SubscribeMarketData();
	*/
}

void Subscribe1(const char *InstrumentID, int periodtype1)
{
	//char * crcvalue = NULL;
	PERIODTYPE pt;
	pt.periodtype[0] = periodtype1;
	pt.periodtype[1] = NULL;
	pt.periodtype[2] = NULL;
	pt.periodtype[3] = NULL;
	pt.periodtype[4] = NULL;
	pt.periodtype[5] = NULL;
	pt.periodtype[6] = NULL;
	pt.periodtype[7] = NULL;
	Add(InstrumentID, &pt);
	ppInstrumentID[amount] = new TThostFtdcInstrumentIDType;
	::strcpy_s(ppInstrumentID[amount], sizeof(TThostFtdcInstrumentIDType), InstrumentID);
	//::strcpy_s(ppInstrumentID[amount],sizeof(ppInstrumentID[amount]), contract);
	gMarket[InstrumentID] = amount;
	//data[amount] = new CThostFtdcDepthMarketDataField;
	depthdata[amount] = new CThostFtdcDepthMarketDataField;
	++amount;
	gMDSpi.SubscribeMarketData();
	printf("(并设置了1个周期)\n");
}
void Subscribe2(const char *InstrumentID, int periodtype1, int periodtype2)
{
	//char * crcvalue = NULL;
	PERIODTYPE pt;
	pt.periodtype[0] = periodtype1;
	pt.periodtype[1] = periodtype2;
	pt.periodtype[2] = NULL;
	pt.periodtype[3] = NULL;
	pt.periodtype[4] = NULL;
	pt.periodtype[5] = NULL;
	pt.periodtype[6] = NULL;
	pt.periodtype[7] = NULL;
	Add(InstrumentID, &pt);
	//printf("pass %s", contract);
	ppInstrumentID[amount] = new TThostFtdcInstrumentIDType;
	::strcpy_s(ppInstrumentID[amount], sizeof(TThostFtdcInstrumentIDType),InstrumentID);
	//::strcpy_s(ppInstrumentID[amount],sizeof(ppInstrumentID[amount]), contract);
	gMarket[InstrumentID] = amount;
	//data[amount] = new CThostFtdcDepthMarketDataField;
	depthdata[amount] = new CThostFtdcDepthMarketDataField;
	++amount;
	gMDSpi.SubscribeMarketData();


	printf("(并设置了2个周期)\n");
}

/*
int Test_MA()
{
	int retCode = TA_Initialize();
	if (retCode != TA_SUCCESS)
	{
		printf("\n初始化指标库失败\n");
		return -1;
	}

	printf("MD这是指标测试\n");


	TA_Real    closePrice[400] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
		41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
		81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56,34,34,32,34,34,
		34,34,56,21,33,45,567,78,8,9,12,34,55,66,89,111,34,56,78,22,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56,34,34,32,34,34,
		34,34,56,21,33,45,567,78,8,9,12,34,55,66,89,111,34,56,78,22,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56,34,34,32,34,34,
		34,34,56,21,33,45,567,78,8,9,12,34,55,66,89,111,34,56,78,22,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56,34,34,32,34,34,
		34,34,56,21,33,45,567,78,8,9,12,34,55,66,89,111,34,56,78,22,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56,34,34,32,34,34,
		34,34,56,21,33,45,567,78,8,9,12,34,55,66,89,111,34,56,78,22,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56,34,34,32,34,34,
		34,34,56,21,33,45,567,78,8,9,12,34,55,66,89,111,34,56,78,22,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56,34,34,32,34,34,
		34,34,56,21,33,45,567,78,8,9,12,34,55,66,89,111,34,56,78,22,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56,34,34,32,34,34};

	TA_Real    out[400];

	TA_Integer outBeg;

	//TA_IntegeroutNbElement;

	// ... initializeyour closing price here... 
	//double outNbElement[] = {34,34,56,21,33,45,567,78,8,9,12,34,55,66,89,111,34,56,78,22,0,34,35,36,37,78,34,34,33,22,345,64,3,34,56};
	int outNbElement = 0;
	retCode = TA_MA(0, 50,

		&closePrice[0],

		30, TA_MAType_SMA,

		&outBeg, &outNbElement, &out[0]);

	// The output isdisplayed here 

	for (int i = 0; i <outNbElement; i++)

		printf("第 %d 天 MA值 = %f\n", outBeg + i, out[i]);


	//int retValue = genCode(1, 1);

	retCode = TA_Shutdown();
	if (retCode != TA_SUCCESS)
	{
		printf("Shutdown failed (%d)\n", retCode);
	}
	return 1;
}
*/


bool CROSSUP(const char *InstrumentID, int indicators, int periodtype, int PriceType, int period1, int period2,bool printstate)
{
	return 0;// 屏蔽 2017.1.1 CxHashList->GetDataCross(InstrumentID, indicators, PriceType, periodtype, period1, period2, true, printstate);
}

bool CROSSDOWN(const char *InstrumentID, int indicators, int periodtype, int PriceType, int period1, int period2,bool printstate)
{
	return 0;//屏蔽2017.1.1 CxHashList->GetDataCross(InstrumentID, indicators, PriceType, periodtype, period1, period2, false, printstate);
 
}

bool CROSSUP_S(int strategyid, int periodtype, int PriceType, int period1, int period2)
{



	return 0;
}

bool CROSSDOWN_S(int strategyid, int periodtype, int PriceType, int period1, int period2)
{



	return 0;

}


 


bool End()
{

	//临界区
	//DeleteCriticalSection(&g_csdata);
	for (int i = 0; i < amount; ++i)
	{
		delete [](ppInstrumentID[i]);
		//delete data[i];
		delete depthdata[i];
	}
	return true;
}


void *GetData(int i)
{
	if (gStatus)
	{
		return NULL;
	}
	if (i < 0 || (i >= (int)gMarket.size()))
	{
		return NULL;
	}else
	{
		//return data[i];
		return depthdata[i];
	}
}

int GetInstrumentNum()
{
	return (int)gMarket.size();
}

int IsInitOK()
{
	return gMDSpi.IsInitOK();
}



char *  GetApiVersion()
{

	return gMDSpi.GetApiVersion();

}




 
int InitMD()

// void InitMD(char *Brokeid, char *Investor, char * Password, char * AppID, char *AuthCode, char * ProductInfo, char * Adder1, char * Adder2, char * Adder3)
{
	IniFile file;
	if (!file.Init("vnctpmd.ini"))
	{
		//MessageBox(NULL, _T("读取vnctpmd.ini失败!"), _T("错误提示"), MB_OK | MB_ICONWARNING);
		//gStatus = 1;
		return 1;
	}

	gBrokerID = file.GetValueFromSection("setting", "brokeid");
	gInvestorID = file.GetValueFromSection("setting", "investor");
	gPassword = file.GetValueFromSection("setting", "password");
	gFrontAddr[0] = file.GetValueFromSection("setting", "address1");
	gFrontAddr[1] = file.GetValueFromSection("setting", "address2");
	gFrontAddr[2] = file.GetValueFromSection("setting", "address3");

	//printf("CPP  %s , %s  , %s\n", gBrokerID.c_str(), gInvestorID.c_str(), gFrontAddr[0].c_str());

	/*
	if (gBrokerID == "")
	{
		//MessageBox(NULL, _T("vnctpmd.ini中BrokerID字段未设置"), _T("错误提示"), MB_OK | MB_ICONWARNING);
		return 1;
	}
	if (gInvestorID == "")
	{
		//MessageBox(NULL, _T("vnctpmd.ini中InvestorID字段未设置"), _T("错误提示"), MB_OK | MB_ICONWARNING);
		return 1;
	}
	*/
	if (gFrontAddr[0] == "" &&  gFrontAddr[1] == "" && gFrontAddr[2] == "")
	{
		//MessageBox(NULL, _T("vnctpmd.ini中FrontAddr字段至少要设置一个"), _T("错误提示"), MB_OK | MB_ICONWARNING);
		return 1;
	}

 
		char dir[256] = { 0 };
		::GetCurrentDirectory(255, dir);
		std::string tempDir = std::string(dir).append("\\MdTemp\\");
		::CreateDirectory(tempDir.c_str(), NULL);
		mpUserApi = CThostFtdcMdApi::CreateFtdcMdApi(tempDir.c_str());
		mpUserApi->RegisterSpi((CThostFtdcMdSpi*)&gMDSpi);
		mpUserApi->RegisterFront((char *)gFrontAddr[0].c_str());
		mpUserApi->RegisterFront((char *)gFrontAddr[1].c_str());
		mpUserApi->RegisterFront((char *)gFrontAddr[2].c_str());
		cerr << "--->>> " << (char *)gFrontAddr[0].c_str() << std::endl;
		cerr << "--->>> " << (char *)gFrontAddr[1].c_str() << std::endl;
		cerr << "--->>> " << (char *)gFrontAddr[2].c_str() << std::endl;
		mpUserApi->Init();

		return 0;
}


 

char *  GetTradingDay()
{
	return gMDSpi.GetTradingDay();
}
void   RegisterFront(char *pszFrontAddress)
{
	return gMDSpi.RegisterFront(pszFrontAddress);
}

void VN_EXPORT RegisterNameServer(char *pszNsAddress)
{
	return gMDSpi.RegisterNameServer(pszNsAddress);
}

int ReqUserLogin()
{
	
	return gMDSpi.ReqUserLogin();
}

int ReqUserLogout()
{
	return gMDSpi.ReqUserLogout();
}

bool  AddPeriodType2(const char *InstrumentID, int  periodtype)
{
		char str[100] = {0};
	std::hash_map<string, GuestOnlineHash>::iterator it;
	it = mapData.find(InstrumentID);
	if (it == mapData.end())
	{
	 
		_snprintf_s(str,100,100,"%s", InstrumentID);
		LOG(str);
		//printf("*************没找到该合约的%s\n", InstrumentID);
		//GuestOnlineHash value;
		//InitGuestOnlineHash(&value);
		//mapData.insert(std::make_pair(InstrumentID, value));
		//ClearGuestOnlineHash(&value);
		//it = mapData.find(InstrumentID);
		return 0;
	}
	else
	{

		GuestOnlineHash * q = &(it->second);
		switch (periodtype)
		{
		case QL_ALL:
			q->period_M1 = true;
			q->period_M3 = true;
			q->period_M5 = true;
			q->period_M10 = true;
			q->period_M15 = true;
			q->period_M30 = true;
			q->period_M60 = true;
			q->period_D1 = true;
			//printf("%s加入M1,M3,M5,M10,M15,M30,M60,D1周期\n", InstrumentID);

			_snprintf_s(str, 100, 100, "%s加入M1,M3,M5,M10,M15,M30,M60,D1周期", InstrumentID);
			LOG(str);
			break;
		case QL_M1:
			q->period_M1 = true;
			//printf("%s加入M1周期\n", InstrumentID);
			
			_snprintf_s(str, 100, 100, "%s加入M1周期", InstrumentID);
			LOG(str);
			break;
		case QL_M3:
			q->period_M3 = true;
			//printf("%s加入M3周期\n", InstrumentID);
			//char str[100];
			_snprintf_s(str, 100, 100, "%s加入M3周期", InstrumentID);
			LOG(str);
			break;
		case QL_M5:
			q->period_M5 = true;
			//printf("%s加入M5周期\n", InstrumentID);
			//char str[100];
			_snprintf_s(str, 100, 100, "%s加入M5周期", InstrumentID);
			LOG(str);
			break;

		case QL_M10:
			q->period_M10 = true;
			//printf("%s加入M10周期\n", InstrumentID);
			//char str[100];
			_snprintf_s(str, 100, 100, "%s加入M10周期", InstrumentID);
			LOG(str);
			break;

		case QL_M15:
			q->period_M15 = true;
			//printf("%s加入M15周期\n", InstrumentID);
			//char str[100];
			_snprintf_s(str, 100, 100, "%s加入M15周期", InstrumentID);
			LOG(str);
			break;

		case QL_M30:
			q->period_M30 = true;
			//printf("%s加入M30周期\n", InstrumentID);
			//char str[100];
			_snprintf_s(str, 100, 100, "%s加入M30周期", InstrumentID);
			LOG(str);
			break;

		case QL_M60:
			q->period_M60 = true;
			//printf("%s加入M60周期\n", InstrumentID);
			//char str[100];
			_snprintf_s(str, 100, 100, "%s加入M60周期", InstrumentID);
			LOG(str);
			break;

		case QL_M120:
			q->period_M120 = true;
			//printf("%s加入M120周期\n", InstrumentID);
			//char str[100];
			_snprintf_s(str, 100, 100, "%s加入M120周期", InstrumentID);
			LOG(str);
			break;

		case QL_D1:
			q->period_D1 = true;
			//printf("%s加入D1周期\n", InstrumentID);
			//char str[100];
			_snprintf_s(str, 100, 100, "%s加入D1周期", InstrumentID);
			LOG(str);
			break;
		}
	}
	return true;
}

void AddPeriod(const char *InstrumentID, int periodtype,bool printfdata)
{

	if (printfdata)
	{
		//printf("添加[%s]周期(由Tick生成周期数据供指标调用)：", InstrumentID);
		char str[100];
		_snprintf_s(str, 100, 100, "添加[%s]周期(由Tick生成周期数据供指标调用)", InstrumentID);
		LOG(str);
	   switch (periodtype)
	  {
	  case QL_ALL:
		  printf("M1、M3、M5、M10、M15、M30、M60、M120、D1 ");
		  break;
	  case QL_M1:
		  printf("M1 ");
		  break;
	  case QL_M3:
		  printf("M3 ");
		  //gM3data = new double[500];
		  //memset(gM3data, 0, 500 * sizeof(double));
		  //std::map<std::string, int> gM3;
		  break;
	  case QL_M5:
		  printf("M5 ");
		  //gM5data = new double[500];
		  //memset(gM5data, 0, 500 * sizeof(double));
		  //std::map<std::string, int> gM5;
		  break;
	  case QL_M10:
		  printf("M10 ");
		  //gM10data = new double[500];
		  //memset(gM10data, 0, 500 * sizeof(double));
		  //std::map<std::string, int> gM10;
		  break;
	  case QL_M15:
		  printf("M15 ");
		  //gM15data = new double[500];
		  //memset(gM15data, 0, 500 * sizeof(double));
		  //std::map<std::string, int> gM15;
		  break;
	  case QL_M30:
		  printf("M30 ");
		  //gM30data = new double[500];
		  //memset(gM30data, 0, 500 * sizeof(double));
		  //std::map<std::string, int> gM30;
		  break;
	  case QL_M60:
		  printf("M60 ");
		  //gM60data = new double[500];
		  //memset(gM60data, 0, 500 * sizeof(double));
		  //std::map<std::string, int> gM60;
		  break;
	  case QL_M120:
		  printf("M120 ");
		  //gM120data = new double[500];
		  //memset(gM120data, 0, 500 * sizeof(double));
		  //std::map<std::string, int> gM120;
		  break;
	  case QL_D1:
		  printf("周期：D1 ");
		  //gD1data = new double[500];
		  //memset(gD1data, 0, 500 * sizeof(double));
		  //std::map<std::string, int> gD1;
		  break;
	  default:
		  return;
		  break;
	  }
	}
	AddPeriodType2(InstrumentID, periodtype);
}


void   AddStopMonitor(const char *InstrumentID, int OrderRef, int mode, double percent)
{
	printf("设置了止损监控(本函数未演示，正在封装)参数： %s 单号 %d 模式 %d 止损阈值百分比%0.02f\n", InstrumentID, OrderRef, mode, percent);
}


void   DeleteStopMonitor(const char *InstrumentID, int OrderRef, int mode, double percent)
{
	printf("删除止损监控(本函数未演示，正在封装)参数： %s 单号 %d 模式 %d 止损阈值百分比%0.02f\n", InstrumentID, OrderRef, mode, percent);
}

double GetPeriodData(const char *InstrumentID, int periodtype, int PriceType, int ref)
{
	ref ++;
	std::hash_map<string, GuestOnlineHash>::iterator it;
	it = mapData.find(InstrumentID);
	if (it == mapData.end())
	{	    printf("*************没找到该合约的%s\n", InstrumentID);
		//GuestOnlineHash value;
		//InitGuestOnlineHash(&value);
		//mapData.insert(std::make_pair(InstrumentID, value));
		//ClearGuestOnlineHash(&value);
		//it = mapData.find(InstrumentID);
	        return -1;
	}
	else
	{

		GuestOnlineHash * q = &(it->second);

		double dataarr = -1;
		 printf("找到合约数据%s %d %d %d\n",InstrumentID,   periodtype,   PriceType,   ref);
		{
			//printf("MA找到数据\n");
			//if (pt)
			//{
			//for (int i = 0; i < PERIOD_NUM; i++)
			//{
			switch (periodtype)
				//switch (q->periodtype)
			{
			case QL_M1:
				//pstate = true;
				dataarr = (q->M1_close[ref]);
				break;
			case QL_M3:
				//pstate = true;
				dataarr = (q->M3_close[ref]);
				break;
			case QL_M5:
				//pstate = true;
				dataarr = (q->M5_close[ref]);
				break;

			case QL_M10:
				//pstate = true;
				dataarr = (q->M10_close[ref]);
				break;

			case QL_M15:
				//pstate = true;
				dataarr = (q->M15_close[ref]);
				break;

			case QL_M30:
				//pstate = true;
				dataarr = (q->M30_close[ref]);
				break;

			case QL_M60:
				//pstate = true;
				dataarr = (q->M60_close[ref]);
				break;

				//case QL_M120:
				//	pstate = true;
				//	dataarr = q->M120_close;
				//break;

				//case QL_D1:
				///	pstate = true;
				//	dataarr = q->D1_close;
				//	break;
			}
		}
		return dataarr;
	}
}

/*
void Log(const char *filename, const char *content)
{


	char str[256] = { 0 };
	strcat_s(str, 256, filename);

	//检查文件是否存在，是否需要新建文本文件
	ifstream inf;
	ofstream ouf;
	//inf.open(TickFileWritepaths[i], ios::out);
	inf.open(str, ios::out);
	//}

	//记录TICK数据
	ofstream o_file(content, ios::app);
	//if ( RunMode && ( check0 || check1 || check2) )

	//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

	o_file << content << "\t" << endl;

	//else
	//{
	//o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //将内容写入到文本文件中
	//}
	o_file.close();						//关闭文件
}
*/

//extern int savetickstate;// = 0;
/*
void SaveTick(int index)
{
	if (index == 2)
	{
		CreateDirectoryA("TickData_Detail", NULL); //创建文件夹
		//gMDSpi.
			savetickstate = 2;
	}
	else if (index == 1)
	{
		CreateDirectoryA("TickData_Simple", NULL); //创建文件夹
		//gMDSpi.
			savetickstate = 1;
	}
}
*/

/*
char * Read_Ini(const char * filename, const char * option, const char * key)
{

	char * temp = new char[50];
	//char temp[50] = {0};
	_snprintf_s(temp, sizeof(*temp), sizeof(*temp),"%s","zn1609");

	return  temp;
}

*/






void Log(const char * filename, const char * content)
{




	//检查文件是否存在，是否需要新建文本文件
	ifstream inf;
	ofstream ouf;
	//inf.open(TickFileWritepaths[i], ios::out);
	inf.open(filename, ios::out);
	//}

	//记录TICK数据
	ofstream o_file(filename, ios::app);
	//if ( RunMode && ( check0 || check1 || check2) )

	//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

	o_file << content <<  endl;

	//else
	//{
	//o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //将内容写入到文本文件中
	//}
	o_file.close();						//关闭文件
}





void TestArr(char** pIpAddList)
{

	for (int i = 0; i < 10; i++)
	{
		char temp[20] = {0};
		sprintf_s(temp,20,"%d",i);
		strcpy_s(pIpAddList[i],20, temp);
	    //ipAddress为IP地址
	
	}

}



void  SetRejectdataTime(double  begintime1, double endtime1, double begintime2, double endtime2, double begintime3, double endtime3, double begintime4, double endtime4)
{
	if (begintime1 < 0 || endtime1 < 0 || begintime2 < 0 || endtime2 < 0 || begintime3 < 0 || endtime3 < 0 || begintime4 < 0 || endtime4 < 0)
	{
		printf("设置拒收行情的时间段必须大于等于0\n");
	}

	if (begintime1 != 100 && endtime1 != 100)
	{
		printf("[%0.06f ~ %0.06f]拒收数据的时间段\n", begintime1, endtime1);
		gMDSpi.begintime1 = begintime1;
		gMDSpi.endtime1 = endtime1;
	}

	if (begintime2 != 100 && endtime2 != 100)
	{
		printf("[%0.06f ~ %0.06f]拒收数据的时间段\n", begintime2, endtime2);
	    gMDSpi.begintime2 = begintime2;
	    gMDSpi.endtime2 = endtime2;
	}

	if (begintime3 != 100 && endtime3 != 100)
	{
		printf("[%0.06f ~ %0.06f]拒收数据的时间段\n", begintime3, endtime3);
	  gMDSpi.begintime3 = begintime3;
	  gMDSpi.endtime3 = endtime3;

	}

	if (begintime3 != 100 && endtime3 != 100)
	{
		printf("[%0.06f ~ %0.06f]拒收数据的时间段\n", begintime4, endtime4);
	  gMDSpi.begintime4 = begintime4;
	  gMDSpi.endtime4 = endtime4;
	}

}


 
bool allprintfstate = false; //是否打印 tick数据

//bool allprintfstate = false; //是否打印 tick数据
void VN_EXPORT SetPrintState(bool printfstate)
{
	allprintfstate = printfstate;

}



unsigned __stdcall MsgThreadOnFrontConnected(void *param)
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	if (!SetEvent(hStartEvent_OnFrontConnected))  // set thread start event 
	{
		printf("set start event failed,errno:%d\n", ::GetLastError());
		return 1;
	}
	while (true)
	{
		if (GetMessage(&msg, 0, 0, 0))  // get msg from message queue
		{
			switch (msg.message)
			{
			case MY_OnFrontConnected:
				((void(__cdecl *)(void))param)();
				printf("recv MY_OnFrontConnected\n");
				break;
			}
		}
	};
}
unsigned __stdcall MsgThreadOnFrontDisconnected(void *param)
{
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	if (!::SetEvent(hStartEvent_OnFrontDisconnected)) //set thread start event
	{
		printf("set start event failed,errno:%d\n", ::GetLastError());
		return 1;
	}
	while (true)
	{
		if (::GetMessage(&msg, 0, 0, 0)) //get msg from message queue 
		{
			switch (msg.message)
			{
			case MY_OnFrontDisconnected:
			{
			    int reason = (int)msg.wParam;
				((void(__cdecl *)(int a))param)(reason);
				
				printf("Recv MsgOnFrontDisconnected %d\n", reason);
				 
				break;
			}
			}
		}
	}
	return 0;
}
unsigned __stdcall MsgThreadOnRspUserLogin(void *param)
{
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	if (!::SetEvent(hStartEvent_OnRspUserLogin)) //set thread start event
	{
		printf("set start event failed,errno:%d\n", ::GetLastError());
		return 1;
	}
	while (true)
	{
		if (::GetMessage(&msg, 0, 0, 0)) //get msg from message queue 
		{
			switch (msg.message)
			{
			case MY_OnRspUserLogin:
			{
				CThostFtdcRspUserLoginField *pRspUserLogin = (CThostFtdcRspUserLoginField *)msg.wParam;
				printf("Recv MsgOnRspUserLogin %s %s\n", pRspUserLogin->BrokerID, pRspUserLogin->UserID);
				((void(__cdecl *)(const CThostFtdcRspUserLoginField * a))param)(pRspUserLogin);
				delete[] pRspUserLogin; 
				break;
			}
			}
		}
	}
	return 0;
}
unsigned __stdcall MsgThreadOnRspUserLogout(void *param)
{
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	if (!::SetEvent(hStartEvent_OnRspUserLogout)) //set thread start event
	{
		printf("set start event failed,errno:%d\n", ::GetLastError());
		return 1;
	}
	while (true)
	{
		if (::GetMessage(&msg, 0, 0, 0)) //get msg from message queue 
		{
			switch (msg.message)
			{
			case MY_OnRspUserLogout:
			{
				CThostFtdcUserLogoutField * pUserLogout = (CThostFtdcUserLogoutField *)msg.wParam;
				printf("Recv MsgOnRspUserLogout %s %s\n", pUserLogout->BrokerID, pUserLogout->UserID);
				((void(__cdecl *)(void))param)();
				delete[] pUserLogout;
				pUserLogout = NULL;
				break;
			}
			}
			Sleep(1);
		}
	}
	return 0;
}

unsigned __stdcall MsgThreadOnRspSubMarketData(void *param)
{
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	if (!::SetEvent(hStartEvent_OnRspUserLogout)) //set thread start event
	{
		printf("set start event failed,errno:%d\n", ::GetLastError());
		return 1;
	}
	while (true)
	{
		if (::GetMessage(&msg, 0, 0, 0)) //get msg from message queue 
		{
			switch (msg.message)
			{
			case MY_OnRspSubMarketData:
			{
				CThostFtdcUserLogoutField * pUserLogout = (CThostFtdcUserLogoutField *)msg.wParam;
				printf("Recv MsgThreadOnRspSubMarketData %s %s\n", pUserLogout->BrokerID, pUserLogout->UserID);
				((void(__cdecl *)(void))param)();
				delete[] pUserLogout;
				pUserLogout = NULL;
				break;
			}
			}
			Sleep(1);
		}
	}
	return 0;
}

unsigned __stdcall MsgThreadOnRspUnSubMarketData(void *param)
{
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	if (!::SetEvent(hStartEvent_OnRspUserLogout)) //set thread start event
	{
		printf("set start event failed,errno:%d\n", ::GetLastError());
		return 1;
	}
	while (true)
	{
		if (::GetMessage(&msg, 0, 0, 0)) //get msg from message queue 
		{
			switch (msg.message)
			{
			case MY_OnRspUnSubMarketData:
			{
				CThostFtdcUserLogoutField * pUserLogout = (CThostFtdcUserLogoutField *)msg.wParam;
				printf("Recv MsgThreadOnRspUnSubMarketData %s %s\n", pUserLogout->BrokerID, pUserLogout->UserID);
				((void(__cdecl *)(void))param)();
				delete[] pUserLogout;
				pUserLogout = NULL;
				break;
			}
			}
			Sleep(1);
		}
	}
	return 0;
}

unsigned __stdcall MsgThreadOnRtnDepthMarketData(void *param)
{
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	if (!::SetEvent(hStartEvent_OnRspUserLogout)) //set thread start event
	{
		printf("set start event failed,errno:%d\n", ::GetLastError());
		return 1;
	}
	while (true)
	{
		if (::GetMessage(&msg, 0, 0, 0)) //get msg from message queue 
		{
			switch (msg.message)
			{
			case MY_OnRtnDepthMarketData:
			{
				CThostFtdcUserLogoutField * pUserLogout = (CThostFtdcUserLogoutField *)msg.wParam;
				printf("Recv MsgThreadOnRtnDepthMarketData %s %s\n", pUserLogout->BrokerID, pUserLogout->UserID);
				((void(__cdecl *)(void))param)();
				delete[] pUserLogout;
				pUserLogout = NULL;
				break;
			}
			}
			Sleep(1);
		}
	}
	return 0;
}


void  VNRegOnFrontConnected(void(*outputCallback)())
{
	HANDLE hThread;
	hStartEvent_OnFrontConnected = ::CreateEvent(0, FALSE, FALSE, 0);  // create thread start event
	if (hStartEvent_OnFrontConnected == 0)
	{
		printf("create start event failed,errno:%d\n", ::GetLastError());
		return;
	}
	hThread = (HANDLE)_beginthreadex(NULL, 0, &MsgThreadOnFrontConnected, outputCallback, 0, &nThreadID_OnFrontConnected);
	if (hThread == 0)
	{
		printf("start thread failed,errno:%d\n", ::GetLastError());
		CloseHandle(hStartEvent_OnFrontConnected);
		return;
	}
	::WaitForSingleObject(hStartEvent_OnFrontConnected, INFINITE);
	CloseHandle(hStartEvent_OnFrontConnected);
	::WaitForSingleObject(hThread, INFINITE);
}

void   VNRegOnFrontDisconnected(void(*outputCallback)(int a))
{
	hStartEvent_OnFrontDisconnected = ::CreateEvent(0, FALSE, FALSE, 0); //create thread start event
	if (hStartEvent_OnFrontDisconnected == 0)
	{
		printf("create start event failed,errno:%d\n", ::GetLastError());
		return;
	}
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &MsgThreadOnFrontDisconnected, outputCallback, 0, &nThreadID_OnFrontDisconnected);
	if (hThread == 0)
	{
		printf("start thread failed,errno:%d\n", ::GetLastError());
		::CloseHandle(hStartEvent_OnFrontDisconnected);
		return;
	}
	::WaitForSingleObject(hStartEvent_OnFrontDisconnected, INFINITE);
	::CloseHandle(hStartEvent_OnFrontDisconnected);
	::WaitForSingleObject(hThread, INFINITE);

}
 
void   VNRegOnRspUserLogin(void(*outputCallback)(const CThostFtdcRspUserLoginField* a))
{
	hStartEvent_OnRspUserLogin = ::CreateEvent(0, FALSE, FALSE, 0); //create thread start event
	if (hStartEvent_OnRspUserLogin == 0)
	{
		printf("create start event failed,errno:%d\n", ::GetLastError());
		return;
	}
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &MsgThreadOnRspUserLogin, outputCallback, 0, &nThreadID_OnRspUserLogin);
	if (hThread == 0)
	{
		printf("start thread failed,errno:%d\n", ::GetLastError());
		::CloseHandle(hStartEvent_OnRspUserLogin);
		return;
	}
	::WaitForSingleObject(hStartEvent_OnRspUserLogin, INFINITE);
	::CloseHandle(hStartEvent_OnRspUserLogin);
	::WaitForSingleObject(hThread, INFINITE);
}
void   VNRegOnRspUserLogout(void(*outputCallback)(const CThostFtdcUserLogoutField * a))
{
	hStartEvent_OnRspUserLogout = ::CreateEvent(0, FALSE, FALSE, 0); //create thread start event
	if (hStartEvent_OnRspUserLogout == 0)
	{
		printf("create start event failed,errno:%d\n", ::GetLastError());
		return;
	}
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &MsgThreadOnRspUserLogout, outputCallback, 0, &nThreadID_OnRspUserLogout);
	if (hThread == 0)
	{
		printf("start thread failed,errno:%d\n", ::GetLastError());
		::CloseHandle(hStartEvent_OnRspUserLogout);
		return;
	}
	::WaitForSingleObject(hStartEvent_OnRspUserLogout, INFINITE);
	::CloseHandle(hStartEvent_OnRspUserLogout);
	::WaitForSingleObject(hThread, INFINITE);
} 

void   VNRegOnRspSubMarketData(void(*outputCallback)(const int* a))
{
	hStartEvent_OnRspSubMarketData = ::CreateEvent(0, FALSE, FALSE, 0); //create thread start event
	if (hStartEvent_OnRspSubMarketData == 0)
	{
		printf("create start event failed,errno:%d\n", ::GetLastError());
		return;
	}
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &MsgThreadOnRspSubMarketData, outputCallback, 0, &nThreadID_OnRspSubMarketData);
	if (hThread == 0)
	{
		printf("start thread failed,errno:%d\n", ::GetLastError());
		::CloseHandle(hStartEvent_OnRspSubMarketData);
		return;
	}
	::WaitForSingleObject(hStartEvent_OnRspSubMarketData, INFINITE);
	::CloseHandle(hStartEvent_OnRspSubMarketData);
	::WaitForSingleObject(hThread, INFINITE);
}

void   VNRegOnRspUnSubMarketData(void(*outputCallback)(const int* a))
{
	hStartEvent_OnRspUnSubMarketData = ::CreateEvent(0, FALSE, FALSE, 0); //create thread start event
	if (hStartEvent_OnRspUnSubMarketData == 0)
	{
		printf("create start event failed,errno:%d\n", ::GetLastError());
		return;
	}
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &MsgThreadOnRspUnSubMarketData, outputCallback, 0, &nThreadID_OnRspUnSubMarketData);
	if (hThread == 0)
	{
		printf("start thread failed,errno:%d\n", ::GetLastError());
		::CloseHandle(hStartEvent_OnRspUnSubMarketData);
		return;
	}
	::WaitForSingleObject(hStartEvent_OnRspUnSubMarketData, INFINITE);
	::CloseHandle(hStartEvent_OnRspUnSubMarketData);
	::WaitForSingleObject(hThread, INFINITE);
}


void   VNRegOnRtnDepthMarketData(void(*outputCallback)(const CThostFtdcDepthMarketDataField* a))
{
	hStartEvent_OnRtnDepthMarketData = ::CreateEvent(0, FALSE, FALSE, 0); //create thread start event
	if (hStartEvent_OnRtnDepthMarketData == 0)
	{
		printf("create start event failed,errno:%d\n", ::GetLastError());
		return;
	}
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &MsgThreadOnRtnDepthMarketData, outputCallback, 0, &nThreadID_OnRtnDepthMarketData);
	if (hThread == 0)
	{
		printf("start thread failed,errno:%d\n", ::GetLastError());
		::CloseHandle(hStartEvent_OnRtnDepthMarketData);
		return;
	}
	::WaitForSingleObject(hStartEvent_OnRtnDepthMarketData, INFINITE);
	::CloseHandle(hStartEvent_OnRtnDepthMarketData);
	::WaitForSingleObject(hThread, INFINITE);
}


