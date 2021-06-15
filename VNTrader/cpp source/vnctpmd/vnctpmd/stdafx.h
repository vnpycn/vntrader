/*
1.本文件为VNTrader 期货CTP交易库底层代码
2.VNTrader及本C++库开源协议MIT协议

对VNPY开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.vnpy.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的vnpy库里。

官方网站：http://www.vnpy.cn
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
#define MAX_EVENTNUM  64


//回调类型
#define SYSTEM_EMPTY                  8000 //无
#define SYSTEM_LOGIN_SCUESS           8001 //登录成功
#define SYSTEM_LOGIN_DENIED           8002 //登录被拒绝
//#define SYSTEM_LOGIN_ERRORPASSWORD  8003 //密码错误 ??
#define SYSTEM_LOGINOUT_SCUESS        8004 //登出成功
#define SYSTEM_NETCONNECT_SCUESS      8005 //连接成功
#define SYSTEM_NETCONNECT_BREAK       8006 //断开连接
#define SYSTEM_NETCONNECT_FAILER      8007 //连接失败 ??
#define SYSTEM_SUBCRIBE_SCUESS        8008 //订阅成功
#define SYSTEM_UNSUBCRIBE_SCUESS      8009 //取消订阅成功
#define SYSTEM_NEWTICK                8010 //新Tick到来
#define SYSTEM_SYSTEM_ERROR           8011 //错误应答
#define SYSTEM_QRY_FORQUOTE           8012 //
//#define SYSTEM_LOG                    8013 //日志
struct TThostFtdcInstrumentIDTypeStruct
{
	TThostFtdcInstrumentIDType Instrument;
};

 

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
