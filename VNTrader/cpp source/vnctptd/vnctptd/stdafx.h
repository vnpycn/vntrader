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
#define MAX_EVENTNUM  64
#define  EID_OnFrontConnected      0
#define  EID_OnFrontDisconnected   1
#define  EID_OnRspUserLogin        2
//#define  EID_OnRspUserLogin_Scuess 3
//#define  EID_OnRspUserLogin_Failer 2
//#define  EID_OnRspUserLogin_Scuess 3
#define  EID_OnRtnDepthMarketData  4
#define  EID_IsErrorRspInfo        5
#define  EID_OnRspSubMarketData    6
#define  EID_OnRspUnSubMarketData  7
#define  EID_OnRspUserLogout       8
#define  EID_OnRspForQuote         9
