#pragma once
#include <map>
#include <string>
#include <algorithm>
#include "iostream"
#include <iostream> 
#include <windows.h>
#include "interface.h"
using namespace std;
struct InstrumentInfo
{
	string strategyfile[300];
	//int strategy[100];
	int strategyfilenum;
	int position[100];
	bool tradestate;
};

#include "def.h"
/*
class CStrategy : public CWinThread
{
public:
	//CStrategy();
	void strategy();

};
 */
extern CThostFtdcMdApi *mpUserApi ;

class CMdSpi : public CThostFtdcMdSpi
{

public:
	char tradingday[50] = { 0 };
	char ver[30] = { 0 };
	double begintime1;
	double begintime2;
	double begintime3;
	double begintime4;

	double endtime1;
	double endtime2;
	double endtime3;
	double endtime4;
	//CStrategy *m_pCStrategyThread;

	BOOL mInitOK;
	HANDLE mhSyncObj;

	int RequestID;
	//void strategy();


	CMdSpi();
	~CMdSpi();
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	// class CthostMdApi::Init()
	BOOL Init();

	BOOL IsInitOK(){return mInitOK;}

    //询价
	int SubscribeForQuoteRsp(char * InstrumentID);
	int SubscribeMarketData(char * InstrumentID);
	int SubscribeMarketData();
	int UnSubscribeMarketData();

	int ReqUserLogin();

	int ReqUserLogout();

	char * GetApiVersion();

	char *  GetTradingDay();
	void   RegisterFront(char *pszFrontAddress);
	void   RegisterNameServer(char *pszNsAddress);
private:

	// 
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

};
