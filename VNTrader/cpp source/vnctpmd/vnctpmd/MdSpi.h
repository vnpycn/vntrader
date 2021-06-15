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
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///ѯ��֪ͨ
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	// class CthostMdApi::Init()
	BOOL Init();

	BOOL IsInitOK(){return mInitOK;}

    //ѯ��
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
