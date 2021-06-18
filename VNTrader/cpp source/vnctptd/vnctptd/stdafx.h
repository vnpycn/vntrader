#pragma once
#undef UNICODE
#undef _UNICODE
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             
#include <windows.h>
#include "CTP/ThostFtdcUserApiDataType.h"
#include "CTP/ThostFtdcUserApiStruct.h"
#include "CTP/ThostFtdcTraderApi.h"
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#define PYCTPTRADER_EXPORTS
#define TYPE_NUM 20  //品种数量应该于MD 订阅一致



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
 

#include "stdafx.h"
#include <windows.h>
#include <process.h>

//#define MY_MSG WM_USER+100 
const int MAX_INFO_SIZE = 20;

//extern HANDLE hThread;

 
extern HANDLE hStartEvent_OnFrontConnected;
extern HANDLE hStartEvent_OnFrontDisconnected;
extern HANDLE hStartEvent_OnRspUserLogin;
extern HANDLE hStartEvent_OnRspUserLogout;
extern HANDLE hStartEvent_OnRspQryInvestorPosition;
extern HANDLE hStartEvent_OnRspQryTradingAccount;
extern HANDLE hStartEvent_OnRtnOrder;
extern HANDLE hStartEvent_OnRtnTrade;

extern unsigned nThreadID_OnFrontConnected;
extern unsigned nThreadID_OnFrontDisconnected;
extern unsigned nThreadID_OnRspUserLogin;
extern unsigned nThreadID_OnRspUserLogout;
extern unsigned nThreadID_OnRspQryInvestorPosition;
extern unsigned nThreadID_OnRspQryTradingAccount;
extern unsigned nThreadID_OnRtnOrder;
extern unsigned nThreadID_OnRtnTrade;



struct VNDEFTradeAcount
{
	// 静态权益
	double prebalance;
	//动态权益
	double current;
	//可用权益
	double available;
	//今日盈亏
	double rate;
	//仓位
	double positionrate;
};