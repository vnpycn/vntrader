/*
1.���ļ�ΪVNTrader �ڻ�CTP���׿�ײ����
2.VNTrader����C++�⿪ԴЭ��MITЭ��

��VNPY��Դ���������׵ģ����õ�ԭʼ���߿϶��ģ���������http://www.vnpy.cn��վ�ϣ�
������ڡ���Դ˵���͸�л.txt�����������ļ����ϸ��·���ÿһ���°汾��vnpy���

�ٷ���վ��http://www.vnpy.cn
*/
#pragma once
#undef UNICODE
#undef _UNICODE
#define WM_STRATEGY WM_USER+1 
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN           
#include <windows.h>
#include "CTP/ThostFtdcUserApiDataType.h"
#include "CTP/ThostFtdcUserApiStruct.h"
#include "CTP/ThostFtdcMdApi.h"
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <hash_map>
#define PYCTPMARKET_EXPORTS

#include <list>
struct cmdcontent
{
	int cmd;
	char content[31];
};




struct TThostFtdcInstrumentIDTypeStruct
{
	TThostFtdcInstrumentIDType Instrument;
};


const int MAX_INFO_SIZE = 20;
#define MY_OnFrontConnected             WM_USER + 101
#define MY_OnFrontDisconnected          WM_USER + 102
#define MY_OnRspUserLogin               WM_USER + 103
#define MY_OnRspUserLogout              WM_USER + 104
#define MY_OnRspQryInvestorPosition     WM_USER + 105
#define MY_OnRspQryTradingAccount       WM_USER + 106
#define MY_OnRtnOrder                   WM_USER + 107
#define MY_OnRtnTrade                   WM_USER + 108
#define MY_OnRtnDepthMarketData         WM_USER + 109
#define MY_OnRspSubMarketData           WM_USER + 110
#define MY_OnRspUnSubMarketData         WM_USER + 111
#define MY_OnRspForQuote                WM_USER + 112
#define MY_OnRspAuthenticate            WM_USER + 113
#define MY_IsErrorRspInfo               WM_USER + 114



extern HANDLE hStartEvent_OnFrontConnected;
extern HANDLE hStartEvent_OnFrontDisconnected;
extern HANDLE hStartEvent_OnRspUserLogin;
extern HANDLE hStartEvent_OnRspUserLogout;
extern HANDLE hStartEvent_OnRspQryInvestorPosition;
extern HANDLE hStartEvent_OnRspQryTradingAccount;
extern HANDLE hStartEvent_OnRtnOrder;
extern HANDLE hStartEvent_OnRtnTrade;

extern HANDLE hStartEvent_OnRspSubMarketData;
extern HANDLE hStartEvent_OnRspUnSubMarketData;
extern HANDLE hStartEvent_OnRtnDepthMarketData;




extern unsigned nThreadID_OnFrontConnected;
extern unsigned nThreadID_OnFrontDisconnected;
extern unsigned nThreadID_OnRspUserLogin;
extern unsigned nThreadID_OnRspUserLogout;
extern unsigned nThreadID_OnRspQryInvestorPosition;
extern unsigned nThreadID_OnRspQryTradingAccount;
extern unsigned nThreadID_OnRtnOrder;
extern unsigned nThreadID_OnRtnTrade;

extern unsigned nThreadID_OnRspSubMarketData;
extern unsigned nThreadID_OnRspUnSubMarketData;
extern unsigned nThreadID_OnRtnDepthMarketData;
