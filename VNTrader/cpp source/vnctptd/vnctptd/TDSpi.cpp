/*
1.本文件为VNTrader 期货CTP交易库底层代码
2.VNTrader及本C++库开源协议MIT协议

对VNPY开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.vnpy.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的vnpy库里。

官方网站：http://www.vnpy.cn
*/
#include "stdafx.h"
#include "TDSpi.h"
using namespace std;
extern std::string gFrontAddr[3];
extern std::string gBrokerID;
extern std::string gInvestorID;
extern std::string gPassword;
extern std::string gAppID;
extern std::string gAuthCode;
extern std::string gUserProductInfo;


extern HANDLE ghTradedVolMutex;
extern std::map<int, int> gOrderRef2TradedVol;

//乘数
std::map<std::string, double> gUnderlyingMultiple;
char	InstrumentID_n[TYPE_NUM][10] = { 0 };


//保证金率
std::map<std::string, double> gMarginRate_long;
std::map<std::string, double> gMarginRate_short;
//手续费率
std::map<std::string, double> gCommissionRate;

//查询最大报单数量
std::map<std::string, int> gMaxOrderVolume;
  


//持仓map
std::map<std::string, int> gPosition_S;
std::map<std::string, int> gPosition_B;

std::map<std::string, int> gPosition_S_Today;
std::map<std::string, int> gPosition_B_Today;
std::map<std::string, int> gPosition_S_History;
std::map<std::string, int> gPosition_B_History;


std::map<std::string, int> gTypeCheckState_S_Today;
std::map<std::string, int> gTypeCheckState_B_Today;
std::map<std::string, int> gTypeCheckState_S_History;
std::map<std::string, int> gTypeCheckState_B_History;




//授权功能
CThostFtdcTraderApi *vntdapi;
//自己增加
CTDSpi *vntdspi = new CTDSpi;


CTDSpi::CTDSpi()
{
	bInitOK = false;
	iRequestID = 0;
	iOrderRef = 0;
	FRONT_ID = 0;
	SESSION_ID = 0;

	vntdapi = NULL;

	hSyncObj = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CTDSpi::~CTDSpi()
{
	std::cout << "-->" << __FUNCTION__ << std::endl;
	if (vntdapi)
	{
// 		vntdapi->Release();
// 		vntdapi = NULL;
	}
	::CloseHandle(hSyncObj);
	std::cout << "<--" << __FUNCTION__ << std::endl;
}

#include <ShellApi.h>
#include <iostream> 
#include <fstream> 
using namespace std;



bool GState = true;
DWORD WINAPI PositionThreadProc(void* p)	//更新排名
{
	while (true)
	{
		GState = !GState;
		if (GState)
		{
			if(vntdspi)
				vntdspi->ReqQryInvestorPosition();//查询仓位管理		
		}
		else
		{
			if (vntdspi)
				vntdspi->ReqQryTradingAccount(); //查询资金
		}
        //CTP有1S流控，太多的查询会导致查询失败
		Sleep(1016);
	}

}

DWORD WINAPI ReqQryInstrumentMarginRateThreadProc(void* p)	//更新排名
{
	if (vntdspi)
	 	vntdspi->ReqQryInstrumentMarginRate("rb1701");//仓位管理		
		return 1;
}  


//char *Brokeid, char *Investor, char * Password, char * AppID, char *AuthCode, char * ProductInfo, char * Adder1, char * Adder2, char * Adder3

    /*
bool CTDSpi::Init()
{

	char dir[256] = {0};
	//::ZeroMemory(dir, 256);
	::GetCurrentDirectory(255, dir);
	std::string tempDir = std::string(dir).append(".\\CTP\\");
	::CreateDirectory(tempDir.c_str(), NULL);

	vntdapi = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\CTP\\");

	vntdspi = this;//自己增加
	vntdapi->RegisterSpi(this);
	vntdapi->SubscribePublicTopic(THOST_TERT_QUICK);
	vntdapi->SubscribePrivateTopic(THOST_TERT_QUICK);
	vntdapi->RegisterFront((char *)gFrontAddr[0].c_str());
	vntdapi->RegisterFront((char *)gFrontAddr[1].c_str());
	vntdapi->RegisterFront((char *)gFrontAddr[2].c_str());

	//std::cout << "QuickLib TD  CTP Init..." << std::endl;
	vntdapi->Init();
	DWORD err = ::WaitForSingleObject(hSyncObj, 10000);

	if (err == WAIT_OBJECT_0)
	{
		bInitOK = true;
	}
	//查询持仓线程
	HANDLE hThread3 = ::CreateThread(NULL, 0, PositionThreadProc, NULL, 0, NULL);
	HANDLE hThread4 = ::CreateThread(NULL, 0, ReqQryInstrumentMarginRateThreadProc, NULL, 0, NULL);

	return bInitOK;


	return true;
}    */
int CTDSpi::ReqUserLogin()
{
	std::cout << __FUNCTION__ << std::endl;
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(CThostFtdcReqUserLoginField));
	strcpy_s(req.BrokerID, sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), gInvestorID.c_str());
	strcpy_s(req.Password, sizeof(TThostFtdcPasswordType), gPassword.c_str());
	if (vntdapi)
	{
		return vntdapi->ReqUserLogin(&req, ++iRequestID);
	}
	else
	{
		return 1;
	}
}
void CTDSpi::OnFrontConnected()
{
	std::cout << __FUNCTION__ << std::endl;
	SetEvent(hEvent[EID_OnFrontConnected]);
	//认证请求
	ReqAuthenticate();
}
int CTDSpi::ReqAuthenticate()
{
	std::cout << __FUNCTION__ << std::endl;
	if (vntdapi == NULL) 
	{
		return 1;
	}
	CThostFtdcReqAuthenticateField  req;
	memset(&req, 0, sizeof(CThostFtdcReqAuthenticateField));
	_snprintf_s(req.BrokerID, sizeof(req.BrokerID), sizeof(req.BrokerID) - 1, "%s", gBrokerID.c_str());
	_snprintf_s(req.UserID, sizeof(req.UserID), sizeof(req.UserID) - 1, "%s", gInvestorID.c_str());
	_snprintf_s(req.AppID, sizeof(req.AppID), sizeof(req.AppID) - 1, "%s", gAppID.c_str());
	_snprintf_s(req.AuthCode, sizeof(req.AuthCode), sizeof(req.AuthCode) - 1, "%s", gAuthCode.c_str());
	_snprintf_s(req.UserProductInfo, sizeof(req.UserProductInfo), sizeof(req.UserProductInfo) - 1, "%s", gUserProductInfo.c_str());
	printf("%s,%s,%s,%s,%s\n", req.BrokerID, req.UserID, req.AppID, req.AuthCode, req.UserProductInfo);
	return vntdapi->ReqAuthenticate(&req, ++iRequestID);
}

void CTDSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{	
	std::cout << __FUNCTION__ << std::endl;
	if (pRspAuthenticateField == NULL)
	{
		ReqAuthenticate();
		return;
	}
	if (IsErrorRspInfo(pRspInfo))
	{
		std::cout << "OnRspAuthenticate错误\n" << std::endl;
	}
	hEvent[EID_OnRspAuthenticate] = CreateEvent(NULL, FALSE, FALSE, "EID_OnRspAuthenticate");
	SetEvent(hEvent[EID_OnRspAuthenticate]);
	ReqUserLogin();
}
 



void CTDSpi::OnFrontDisconnected(int nReason)
{
	std::cout << __FUNCTION__ << std::endl;
	SYSTEMTIME t;
	::GetLocalTime(&t);
	std::cout << t.wHour << ":" << t.wMinute << ":" << t.wSecond << std::endl;
	std::cout << "--->>> " << __FUNCTION__ << std::endl;
	std::cout << "--->>> Reason = " << nReason << std::endl;
	::Beep(450, 10000);
}




void CTDSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspUserLogin==NULL)
	{
		return;
	}
	SetEvent(hEvent[EID_OnFrontConnected]);
	ReqSettlementInfoConfirm();
	std::cout << __FUNCTION__ << std::endl;
	FRONT_ID = pRspUserLogin->FrontID;
	SESSION_ID = pRspUserLogin->SessionID;
 
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		if (pRspInfo && pRspInfo->ErrorID != 0)
		{
			printf("Failer:ErrorID=0x%04x, ErrMsg=%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		}
		else
		{
			printf("Scuess:\n");
			Sleep(3000);
		}
	}
}
 
void CTDSpi::OnRspQryInstrument2(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
	if (pInstrument==NULL)
	{
		return;
	}
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		if (pRspInfo && pRspInfo->ErrorID != 0)
		{
			printf("failer:登录行情服务器失败\n");
		}
	}
}

void CTDSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
	if (pUserLogout==NULL)
	{
		return;
	}
	//FRONT_ID = pUserLogout->FrontID;
	//SESSION_ID = pUserLogout->SessionID;
	//if (bIsLast)
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		if (pRspInfo && pRspInfo->ErrorID != 0)
		{
			printf("登出失败,ErrorID=0x%04x, ErrMsg=%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		}
		else
		{
			printf("登出成功\n");
		}
	}
}

int CTDSpi::ReqSettlementInfoConfirm()
{
	//投资者结算结果确认
	std::cout << __FUNCTION__ << std::endl;
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(CThostFtdcSettlementInfoConfirmField));
	strcpy_s(req.BrokerID,sizeof(req.BrokerID), gBrokerID.c_str());
	strcpy_s(req.InvestorID,sizeof(req.InvestorID), gInvestorID.c_str());
	return vntdapi->ReqSettlementInfoConfirm(&req, ++iRequestID);
}

void CTDSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
 	std::cout << __FUNCTION__ << std::endl;
	if (bIsLast)
	{
		::SetEvent(hSyncObj);
		//确认结算单成功
	}
}

bool CTDSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		SYSTEMTIME t;
		::GetLocalTime(&t);
		std::cout << t.wHour << ":" << t.wMinute << ":" << t.wSecond << std::endl;
		std::cout << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
		::Beep(800, 10000);
	}
	return bResult;
}


void CTDSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
	std::cerr << pRspInfo->ErrorID << "\t" << pRspInfo->ErrorMsg << std::endl;
	IsErrorRspInfo(pRspInfo);
}

void CTDSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	std::cout << __FUNCTION__ << std::endl;
	if (!pOrder)
	{
		return;
	}
	std::cout << __FUNCTION__ << std::endl;
		int orderRef = ::atoi(pOrder->OrderRef);
		::WaitForSingleObject(ghTradedVolMutex, INFINITE);
		gOrderRef2TradedVol[orderRef] = pOrder->VolumeTraded;
		::ReleaseMutex(ghTradedVolMutex);
}

void CTDSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	std::cout << __FUNCTION__ << std::endl;
	if (!pTrade)
	{
		return;
	}
	std::cout << __FUNCTION__ << std::endl;
	int orderRef = ::atoi(pTrade->OrderRef);
	printf("订单成交[%d]\n", orderRef);
	//
	/*
	::WaitForSingleObject(ghTradedVolMutex, INFINITE);
	gOrderRef2TradedVol[orderRef] = pTrade->VolumeTraded;
	::ReleaseMutex(ghTradedVolMutex);
	*/
	///成交


}

//合约交易状态通知
 
void  CTDSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	std::cout << __FUNCTION__ << std::endl;
	if (!pInstrumentStatus)
	{
		return;
	}
 
}





int CTDSpi::DeleteOrder(char *InstrumentID, DWORD orderRef)
{
	//错误返回-1是和正常冲突的吗？
	if (!InstrumentID)
	{
		return -1;
	}
	//std::cout << __FUNCTION__ << std::endl;
	CThostFtdcInputOrderActionField ReqDel;
	::ZeroMemory(&ReqDel, sizeof(ReqDel));
	strcpy_s(ReqDel.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(ReqDel.InvestorID,sizeof(TThostFtdcInvestorIDType),gInvestorID.c_str());
	strcpy_s(ReqDel.InstrumentID,sizeof(TThostFtdcInstrumentIDType), InstrumentID);
	_snprintf_s(ReqDel.OrderRef,sizeof(TThostFtdcOrderRefType), sizeof(TThostFtdcOrderRefType)-1, "%012d", orderRef);
	ReqDel.FrontID = FRONT_ID;
	ReqDel.SessionID = SESSION_ID;
	ReqDel.ActionFlag = THOST_FTDC_AF_Delete;
	int iResult = vntdapi->ReqOrderAction(&ReqDel, ++(iRequestID));

	if (iResult != 0)
		cerr << "Failer: 撤单 : " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	else
		cerr << "Scuess: 撤单 : 成功" << endl;

	return iResult;
}

VOID MakeOrder(CThostFtdcInputOrderField *pOrder)
{
	memset(pOrder, 0, sizeof(*pOrder));
	///经纪公司代码
	strcpy_s(pOrder->BrokerID, sizeof(TThostFtdcBrokerIDType),gBrokerID.c_str());
	///投资者代码
	strcpy_s(pOrder->InvestorID, sizeof(TThostFtdcInvestorIDType),gInvestorID.c_str());
	///用户代码
	//TThostFtdcUserIDType	UserID;
	///组合投机套保标志
	pOrder->CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	
	///限价单
	pOrder->OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///触发条件: 立即
	pOrder->ContingentCondition = THOST_FTDC_CC_Immediately;
	///有效期类型: 当日有效
	pOrder->TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
	//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	pOrder->VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	pOrder->MinVolume = 1;
	///止损价
	//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	pOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	pOrder->IsAutoSuspend = 0;
	///业务单元
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	pOrder->UserForceClose = 0;
}








int CTDSpi:: InsertOrder(char *InstrumentID, char dir,char offsetFlag, char priceType, double price, int num)
{
	CThostFtdcInputOrderField req;
	MakeOrder(&req);
	::strcpy_s(req.InstrumentID,sizeof(TThostFtdcInstrumentIDType), InstrumentID);

	req.CombOffsetFlag[0] = offsetFlag;
	req.Direction = dir;
	req.VolumeTotalOriginal = num;
	req.LimitPrice = price;
	req.OrderPriceType = priceType;

	++(iOrderRef);
	_snprintf_s(req.OrderRef,sizeof(TThostFtdcOrderRefType), sizeof(TThostFtdcOrderRefType)-1, "%012d", iOrderRef);
	int iResult = vntdapi->ReqOrderInsert(&req, ++(iRequestID));
	if (iResult != 0)
		cerr << "Failer: 下单" << InstrumentID << price << num << ": " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	else
		cerr << "Scuess: 下单"<< InstrumentID  << price << num <<": 成功" << endl;

	return iOrderRef;
}


///////////////////////////////////////////////////////////////////////////////自己增加



int CTDSpi::ReqQryInvestorPosition()
{
	std::cout << __FUNCTION__ << std::endl;
	if (vntdapi == NULL)
	{
		return 1;
	}
	// WirteTradeRecordToFileMainThread(0, "ReqQryInvestorPosition");
	//CThostFtdcQryInvestorPositionField req = { 0 };
	//strcpy(req.BrokerID, m_BrokerID);
	//strcpy(req.InvestorID, m_InvestorInfos[reqInfo.lAccIdx].InvestorID);
	//req.InstrumentID; //指定合约的话，就是查询特定合约的持仓信息，不填就是查询所有持仓  
	//ReqQryInvestorPosition(&req, reqInfo.nRequestID);
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(CThostFtdcQryInvestorPositionField));
	//strcpy(req.BrokerID, BROKER_ID);
	//strcpy(req.InvestorID, INVESTOR_ID);

	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(req.InvestorID,sizeof(TThostFtdcInvestorIDType),gInvestorID.c_str());
	//strcpy(req.InstrumentID, INSTRUMENT_ID);
	//printf("指定持仓%s", INSTRUMENT_ID);
	//int iResult = vntdapi->ReqQryInvestorPosition(&req, ++iRequestID);

	return vntdapi->ReqQryInvestorPosition(&req, ++iRequestID);
	//cerr << "请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << endl;
	//if (iResult != 0)
	//	cerr << "Failer(ReqQryInvestorPosition): 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	//else
	//	cerr << "Scuess(ReqQryInvestorPosition): 请求查询投资者持仓: 成功" << endl;

}

///请求查询投资者品种/跨品种保证金
void CTDSpi::ReqQryInvestorProductGroupMargin(char *Instrument)
{
	std::cout << __FUNCTION__ << std::endl;

	if (vntdapi == NULL)
	{
		return;
	}


	//CThostFtdcQryInvestorPositionField req = { 0 };
	//strcpy(req.BrokerID, m_BrokerID);
	//strcpy(req.InvestorID, m_InvestorInfos[reqInfo.lAccIdx].InvestorID);
	//req.InstrumentID; //指定合约的话，就是查询特定合约的持仓信息，不填就是查询所有持仓  
	//ReqQryInvestorPosition(&req, reqInfo.nRequestID);
	//CThostFtdcQryInvestorProductGroupMarginField
	/*
	///查询投资者品种/跨品种保证金
	struct CThostFtdcQryInvestorProductGroupMarginField
	{
		///经纪公司代码
		TThostFtdcBrokerIDType	BrokerID;
		///投资者代码
		TThostFtdcInvestorIDType	InvestorID;
		///品种/跨品种标示
		TThostFtdcInstrumentIDType	ProductGroupID;
		///投机套保标志
		TThostFtdcHedgeFlagType	HedgeFlag;
	};
	*/
	CThostFtdcQryInvestorProductGroupMarginField req;
	//CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(CThostFtdcQryInvestorProductGroupMarginField));

	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(req.InvestorID,sizeof(TThostFtdcInvestorIDType), gInvestorID.c_str());
	req.HedgeFlag = '1';
 


 	//printf("指定持仓%s", INSTRUMENT_ID);
	int iResult = vntdapi->ReqQryInvestorProductGroupMargin(&req, ++iRequestID);
	//cerr << "--->>> 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << endl;

	if (iResult != 0)
		cerr << "Failer(ReqQryInvestorProductGroupMargin): 请求查询投资者品种/跨品种保证金: " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqQryInvestorProductGroupMargin): 请求查询投资者品种/跨品种保证金: 成功" << endl;
}


int CTDSpi::ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID)
{
	std::cout << __FUNCTION__ << std::endl;

	int iResult = vntdapi->ReqQueryMaxOrderVolume(pQueryMaxOrderVolume, ++iRequestID);
	if (iResult != 0)
		cerr << "Failer(ReqQueryMaxOrderVolume): 查询下单最大值: " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqQueryMaxOrderVolume): 查询下单最大值: 成功" << endl;

	return iResult;
}

///期货发起银行资金转期货请求
int CTDSpi::ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID)
{
	std::cout << __FUNCTION__ << std::endl;

	int iResult = vntdapi->ReqFromBankToFutureByFuture(pReqTransfer, ++iRequestID);

	if (iResult != 0)
		cerr << "Failer(ReqFromBankToFutureByFuture): 期货发起银行资金转期货请求: " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqFromBankToFutureByFuture): 期货发起银行资金转期货请求: 成功" << endl;

	return iResult;
}

///期货发起期货资金转银行请求
int CTDSpi::ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID)
{
	std::cout << __FUNCTION__ << std::endl;

	int iResult = vntdapi->ReqFromFutureToBankByFuture(pReqTransfer, ++iRequestID);

	if (iResult != 0)
		cerr << "Failer(ReqFromFutureToBankByFuture): 期货发起期货资金转银行请求: " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqFromFutureToBankByFuture): 期货发起期货资金转银行请求: 成功" << endl;

	return iResult;
}


void CTDSpi::ReqQryInstrument(char *Instrument)
{
	std::cout << __FUNCTION__ << std::endl;

	if (vntdapi == NULL)
	{
		return;
	}

	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(CThostFtdcQryInstrumentField));
	strcpy_s(req.ExchangeID,sizeof(TThostFtdcExchangeIDType), "DCE");
	strcpy_s(req.ExchangeInstID,sizeof(TThostFtdcExchangeInstIDType), "");
	strcpy_s(req.ProductID,sizeof(TThostFtdcInstrumentIDType), "");
	//strcpy(req.BrokerID, gBrokerID.c_str());
	//strcpy(req.InvestorID, gInvestorID.c_str());
	strcpy_s(req.InstrumentID,sizeof(TThostFtdcInstrumentIDType), Instrument);

	//printf("查询乘数[%s]   [%s]  [%s]", req.ExchangeID, req.ProductID, req.InstrumentID);

	int iResult = vntdapi->ReqQryInstrument(&req, ++iRequestID);
	//cerr << "--->>> 请求查询合约: " << ((iResult == 0) ? "成功" : "失败") << endl;

	if (iResult != 0)
		cerr << "Failer(ReqQryInstrument): 请求查询合约: " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqQryInstrument): 请求查询合约: 成功" << endl;
}

void CTDSpi::ReqQryInstrumentMarginRate(char *Instrument)
{
	std::cout << __FUNCTION__ << std::endl;

	if (vntdapi == NULL)
	{
		return;
	}

	//CThostFtdcQryInvestorPositionField req = { 0 };
	//strcpy(req.BrokerID, m_BrokerID);
	//strcpy(req.InvestorID, m_InvestorInfos[reqInfo.lAccIdx].InvestorID);
	//req.InstrumentID; //指定合约的话，就是查询特定合约的持仓信息，不填就是查询所有持仓  
	//ReqQryInvestorPosition(&req, reqInfo.nRequestID);

	CThostFtdcQryInstrumentMarginRateField req;
	memset(&req, 0, sizeof(CThostFtdcQryInstrumentMarginRateField));

	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(req.InvestorID,sizeof(TThostFtdcInvestorIDType), gInvestorID.c_str());
	strcpy_s(req.InstrumentID,sizeof(TThostFtdcInstrumentIDType), Instrument);
	req.HedgeFlag = '1';

	printf("查询保证金[%s] 投资者账户[%s]合约[%s]\n",req.BrokerID, req.InvestorID, req.InstrumentID);
	//1>CTPTraderSpi.cpp(654) : error C2664 : “int CThostFtdcTraderApi::ReqQryInstrumentMarginRate(
		//CThostFtdcQryInstrumentMarginRateField *, int)” : 无法将参数 1 从“
		//CThostFtdcInstrumentMarginRateField *”转换为“CThostFtdcQryInstrumentMarginRateField *”
	//printf("\n----------------------------------------\n");

	//strcpy(req.InstrumentID, INSTRUMENT_ID);
	//printf("指定持仓%s", INSTRUMENT_ID);
	int iResult = vntdapi->ReqQryInstrumentMarginRate(&req, ++iRequestID);
	cerr << "--->>> 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << endl;


 
}



///请求查询合约
int CTDSpi::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	std::cout << __FUNCTION__ << std::endl;


	//if (pQryInstrument == NULL)
	//{
	//	return 0;
	//}
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(CThostFtdcQryInstrumentField));

	/*
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///产品代码
	TThostFtdcInstrumentIDType	ProductID;
	*/
	//strcpy(req.BrokerID, gBrokerID.c_str());
	//strcpy(req.InvestorID, gInvestorID.c_str());
	//strcpy(req.InstrumentID, Instrument);
	int iResult = vntdapi->ReqQryInstrument(&req, ++iRequestID);
	if (iResult != 0)
		cerr << "Failer(ReqQryInstrument): 请求查询合约: " << ((iResult == 0) ? "成功" : "失败(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqQryInstrument): 请求查询合约: 成功" << endl;
	return iResult;
}

int CTDSpi::ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID)
{
	std::cout << __FUNCTION__ << std::endl;

	if (pQryContractBank == NULL)
	{
		return -1;
	}
	CThostFtdcQryContractBankField req;
	memset(&req, 0, sizeof(CThostFtdcQryContractBankField));

 
	//strcpy(req.BrokerID, gBrokerID.c_str());
	//strcpy(req.InvestorID, gInvestorID.c_str());
	//strcpy(req.InstrumentID, Instrument);
	int iResult = vntdapi->ReqQryContractBank(&req, ++iRequestID);
	// cerr << "Failer: 请求查询银行: " << ((iResult == 0) ? "成功" : "失败(") << iResult<<")"<<endl;
	if(iResult!=0)
	    cerr << "Failer(ReqQryContractBank): 请求查询银行: " << ((iResult == 0) ? "成功" : "失败(") << iResult<<")"<<endl;
	else
		cerr << "Scuess(ReqQryContractBank): 请求查询银行: 成功" <<endl;
	return iResult;
}



int CTDSpi::ReqQryTradingAccount()
{
	std::cout << __FUNCTION__ << std::endl;

	if (vntdapi == NULL)
	{
		return 1;
	}
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(CThostFtdcQryTradingAccountField));
	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(req.InvestorID,sizeof(TThostFtdcInvestorIDType), gInvestorID.c_str());
	return vntdapi->ReqQryTradingAccount(&req, ++iRequestID); 
}



bool FindStr(int id, char * str)
{
	std::cout << __FUNCTION__ << std::endl;

	//char * pdest1 = strstr(InstrumentID_n[id], str);
	//int  result1 = pdest1 - InstrumentID_n[id] + 1;
	//printf("%s  %s\n", InstrumentID_n[id], str);

	if (_stricmp(InstrumentID_n[id], str) == 0)
		//if (pdest1 != NULL)
	{	//printf("在%s发现%s\n", InstrumentID_n[id],str );
		return true;
	}
	else
	{
		//printf("%s 没有在%s发现\n", str, InstrumentID_n[id]);
		return false;
	}
}

int SaveInstrumentID = { 0 };
bool  checkstate = false;
bool  TypeCheckState_B_Today[TYPE_NUM] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool  TypeCheckState_S_Today[TYPE_NUM] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

bool  TypeCheckState_B_History[TYPE_NUM] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool  TypeCheckState_S_History[TYPE_NUM] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
///请求查询投资者持仓响应

int errnum = 0;

extern bool showpositionstate;
void CTDSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;

	if (pInvestorPosition == NULL)
	{
		errnum++;
		if (errnum > 10)
		{
			printf("多次查询持仓指针错误,请在配置文件中选择较快的交易服务器\n");
		}
		return;
	}
	errnum = 0;



	//WirteTradeRecordToFileMainThread(0, "请求查询投资者持仓响应OnRspQryInvestorPosition");

	//cerr << "--->>> " << "投资者持仓数量:" << endl;
	// if (bIsLast && !IsErrorRspInfo(pRspInfo))
	//if (bIsLast)
	//{


	/*
	cout << "OnRspQryInvestorPosition  ID: " << nRequestID << endl;
	cout << "合约代码  ID: " << pInvestorPosition->InstrumentID << endl;
	//cout << "错误代码：" << pRspInfo->ErrorID << "错误信息:" << pRspInfo->ErrorMsg;
	cout << "持仓多空方向:" << pInvestorPosition->PosiDirection << endl;  //2多单，3空单
	cout << "仓位:" << pInvestorPosition->Position << endl;
	*/

/*
	cout << "[ID]" << nRequestID << "合约:"
		<< pInvestorPosition->InstrumentID
		<< "持仓方向:" << pInvestorPosition->PosiDirection
		<< "仓位:" << pInvestorPosition->Position
		<< "BrokerID:" << pInvestorPosition->AbandonFrozen
		<< "AbandonFrozen:" << pInvestorPosition->BrokerID
		<< "CashIn:" << pInvestorPosition->CashIn
		<< "a:" << pInvestorPosition->CloseAmount
		<< "b:" << pInvestorPosition->CloseProfit
		<< "c:" << pInvestorPosition->CloseProfitByDate
		<< "d:" << pInvestorPosition->CloseProfitByTrade
		<< "e:" << pInvestorPosition->CloseVolume
		<< "f:" << pInvestorPosition->CombLongFrozen
		<< "g:" << pInvestorPosition->CombPosition
		<< "h:" << pInvestorPosition->CombShortFrozen
		<< "i:" << pInvestorPosition->Commission
		<< "j:" << pInvestorPosition->ExchangeMargin
		<< "k:" << pInvestorPosition->FrozenCash
		<< "l:" << pInvestorPosition->FrozenCommission
		<< "cc:" << pInvestorPosition->FrozenMargin
		<< "m:" << pInvestorPosition->HedgeFlag
		<< "n:" << pInvestorPosition->InstrumentID
		<< "o:" << pInvestorPosition->InvestorID
		<< "p:" << pInvestorPosition->LongFrozen
		<< "q:" << pInvestorPosition->LongFrozenAmount
		<< "r:" << pInvestorPosition->MarginRateByMoney
		<< "s:" << pInvestorPosition->MarginRateByVolume
		<< "t:" << pInvestorPosition->OpenAmount
		<< "u:" << pInvestorPosition->OpenCost
		<< "v:" << pInvestorPosition->OpenVolume
		<< "w:" << pInvestorPosition->PosiDirection
		<< "x:" << pInvestorPosition->Position
		<< "y:" << pInvestorPosition->PositionCost
		<< "z:" << pInvestorPosition->PositionDate
		<< "1:" << pInvestorPosition->PositionProfit
		<< "2:" << pInvestorPosition->PreMargin
		<< "3:" << pInvestorPosition->PreSettlementPrice
		<< "4:" << pInvestorPosition->SettlementID
		<< "5:" << pInvestorPosition->SettlementPrice
		<< "6:" << pInvestorPosition->ShortFrozen
		<< "7:" << pInvestorPosition->ShortFrozenAmount
		<< "8:" << pInvestorPosition->StrikeFrozen
		<< "9:" << pInvestorPosition->StrikeFrozenAmount
		<< "10:" << pInvestorPosition->StrikeFrozen
		<< "11:" << pInvestorPosition->StrikeFrozenAmount
		<< "12:" << pInvestorPosition->TodayPosition
		<< "13:" << pInvestorPosition->TradingDay
		<< "14:" << pInvestorPosition->UseMargin
		<< "15:" << pInvestorPosition->YdPosition

		<< endl;
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	
	*/

	/*
	[ID]22合约:m1609持仓方向:2仓位 : 0BrokerID : 0AbandonFrozen : 8016CashIn : 0
	a : 162540
	b : 780
	c : 780
	d : 120
	e : 6
	f : 0
	g : 0
	h : 0
	i : 9.71378
	j : 0
	k : 0l : 0cc : 0
	m : 1
	n : m1609
	o : 13100293
	p : 0
	q : 0
	r : 0.1
	s : 0
	t : 0
	u : 0
	v : 0
	w : 2
	x : 0
	y : 0
	z :
	11 : 0
	2 : 0
	3 : 2696
	4 : 1
	5 : 2709
	6 : 0
	7 : 0
	8:0
	9:0
	10:0
	11 : 0
	12 : 0
	13 : 20160504
	14 : 0
	15 : 6
	*/


	while (nRequestID != SaveInstrumentID)
	{
		SaveInstrumentID = nRequestID;

		/*
		for (int i = 0; i < TYPE_NUM; i++)
		{
			TypeCheckState_B_Today[i] = false;
			TypeCheckState_S_Today[i] = false;
			TypeCheckState_B_History[i] = false;
			TypeCheckState_S_History[i] = false;
		}
		*/

		gTypeCheckState_B_History[pInvestorPosition->InstrumentID] = false;
		gTypeCheckState_B_Today[pInvestorPosition->InstrumentID] = false;
		gTypeCheckState_S_History[pInvestorPosition->InstrumentID] = false;
		gTypeCheckState_S_Today[pInvestorPosition->InstrumentID] = false;
		//printf("新查询初始化\n");
	}
 
	//for (int i = 0; i <TYPE_NUM; i++)
	//{
		//if (FindStr(i, pInvestorPosition->InstrumentID))
		//{
	
			if (pInvestorPosition->PosiDirection == '2' &&  pInvestorPosition->Position != 0) //注意别的版本要修复
			{
				if (pInvestorPosition->TodayPosition == 0) //今仓
				{
					//TypeCheckState_B_History[i] = true;
					gTypeCheckState_B_History[pInvestorPosition->InstrumentID] = true;
					//Trade_dataA_Amount_B_History[i] = pInvestorPosition->Position;

					//持仓map
					gPosition_B_History[pInvestorPosition->InstrumentID] = pInvestorPosition->Position;

					//printf("历史买单 [%d]\n", gPosition_B_History[pInvestorPosition->InstrumentID]);

				}
				else
				{
					//TypeCheckState_B_Today[i] = true;
					gTypeCheckState_B_Today[pInvestorPosition->InstrumentID] = true;
					//Trade_dataA_Amount_B_Today[i] = pInvestorPosition->Position;

					//持仓map
					gPosition_B_Today[pInvestorPosition->InstrumentID] = pInvestorPosition->Position;

					//printf("今仓买单 [%d]\n", gPosition_B_Today[pInvestorPosition->InstrumentID]);
				}
			}
			else if (pInvestorPosition->PosiDirection == '3' &&  pInvestorPosition->Position != 0)  //注意别的版本要修复
			{
				if (pInvestorPosition->TodayPosition == 0) //今仓
				{
					//TypeCheckState_S_History[i] = true;
					gTypeCheckState_S_History[pInvestorPosition->InstrumentID] = true;
					//Trade_dataA_Amount_S_History[i] = pInvestorPosition->Position;

					//持仓map
					gPosition_S_History[pInvestorPosition->InstrumentID] = (pInvestorPosition->Position);

					//printf("历史卖单 [%d]\n", gPosition_S_History[pInvestorPosition->InstrumentID]);
				}
				else
				{
					//TypeCheckState_S_Today[i] = true;
					gTypeCheckState_S_Today[pInvestorPosition->InstrumentID] = true;
					//Trade_dataA_Amount_S_Today[i] = pInvestorPosition->Position;

					//持仓map
					gPosition_S_Today[pInvestorPosition->InstrumentID] = (pInvestorPosition->Position);

					//printf("今仓卖单 [%d]\n", gPosition_S_Today[pInvestorPosition->InstrumentID]);
				}
			}

 


	if (bIsLast)
	{
		for (int i = 0; i < TYPE_NUM; i++)
		{
			if(!gTypeCheckState_B_History[pInvestorPosition->InstrumentID])
			//if (!TypeCheckState_B_History[i])
			{
				//Trade_dataA_Amount_B_History[i] = 0;
				//持仓map
				gPosition_B_History[pInvestorPosition->InstrumentID] = 0;
				//printf("历史买归0 [%d]\n", gPosition_B_History[pInvestorPosition->InstrumentID]);
			}


			if(!gTypeCheckState_B_Today[pInvestorPosition->InstrumentID])
			//if (!TypeCheckState_B_Today[i])
			{
				//Trade_dataA_Amount_B_Today[i] = 0;
				//持仓map
				gPosition_B_Today[pInvestorPosition->InstrumentID] = 0;

				//printf("今天买归0 [%d]\n", gPosition_B_Today[pInvestorPosition->InstrumentID]);
			}

			if(!gTypeCheckState_S_History[pInvestorPosition->InstrumentID])
			//if (!TypeCheckState_S_History[i])
			{
				//Trade_dataA_Amount_S_History[i] = 0;
				//持仓map
				gPosition_S_History[pInvestorPosition->InstrumentID] = 0;

				//printf("历史卖归0 [%d]\n", gPosition_S_History[pInvestorPosition->InstrumentID]);

			}

			if(!gTypeCheckState_S_Today[pInvestorPosition->InstrumentID])
			//if (!TypeCheckState_S_Today[i])
			{
				//Trade_dataA_Amount_S_Today[i] = 0;
				//持仓map
				gPosition_S_Today[pInvestorPosition->InstrumentID] = 0;

				//printf("今天卖归0 [%d]\n", gPosition_S_Today[pInvestorPosition->InstrumentID]);
			}
			if (1)
			{
			//	printf("品种：%s  BUY持仓(今仓)[%d] BUY持仓(历史仓)[%d] SELL持仓(今仓)[%d] SELL持仓(历史)[%d]\n", InstrumentID_n[i], Trade_dataA_Amount_B_Today[i], Trade_dataA_Amount_B_History[i], Trade_dataA_Amount_S_Today[i], Trade_dataA_Amount_S_History[i]);
			}

		}



	}

	gPosition_S[pInvestorPosition->InstrumentID] = gPosition_S_Today[pInvestorPosition->InstrumentID] + gPosition_S_History[pInvestorPosition->InstrumentID];
	gPosition_B[pInvestorPosition->InstrumentID] = gPosition_B_Today[pInvestorPosition->InstrumentID] + gPosition_B_History[pInvestorPosition->InstrumentID];

	if (showpositionstate)
	{
		printf("%s ：总卖单[%d] 今卖单[%d] 非今日卖单[%d] 总买单[%d] 今买单[%d] 非今日买单[%d]\n", pInvestorPosition->InstrumentID,
		gPosition_S[pInvestorPosition->InstrumentID], gPosition_S_Today[pInvestorPosition->InstrumentID], gPosition_S_History[pInvestorPosition->InstrumentID],
		gPosition_B[pInvestorPosition->InstrumentID], gPosition_B_Today[pInvestorPosition->InstrumentID] , gPosition_B_History[pInvestorPosition->InstrumentID]);
	}



	//TypeNumber[nRequestID] = pInvestorPosition->Position;
	//printf("输出 ：%d\n",  pInvestorPosition->Position);

	//printf("品种：%s\n", InstrumentID_n[nRequestID]);

	//printf("*******************************************************************\n");
	///合约保证金以及手续费查询请求
	//for (int i = 0; i < 20; i++)
	//{
	//strcpy(INSTRUMENT_ID,InstrumentID_n[3]);	
	//strcpy(INSTRUMENT_ID, InstrumentID_n[nRequestID]);
	//ReqQryInstrumentMarginRate();
	//ReqQryInstrumentCommissionRate();
	//	Sleep(0);
	//}

	//}

	//END

}




bool  IsErrorRspInfo222(CThostFtdcRspInfoField *pRspInfo)
{
 
	cerr << "--->>> " << "IsErrorRspInfo\n" << "0" << endl;	  //指针检查
															  // 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
 
	if (bResult)
	{
		cerr << "--->>> \nErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
		char errmsg[200] = { 0 };
		_snprintf_s(errmsg, sizeof(errmsg), sizeof(errmsg), "OnRspError ErrorID:%d ErrorMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		//WirteTradeRecordToFileMainThread(0, errmsg);
	}
 

	//return bResult;
 

	return  ((pRspInfo) && (pRspInfo->ErrorID != 0));
}

double YestayAllAmount=0;
double TodayAllAmount=0;
double Available=0;


struct CTradeAcount
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

void CTDSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
	if (pTradingAccount == NULL)
	{
		return;
	}
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		CTradeAcount tn;
		//cerr << "--->>> 交易日: " << pTradingAccount->TradingDay << "\n" << endl;
		//cerr << "--->>> \n可用资金: " << (int)(pTradingAccount->Available / 10000) << "万\n" << endl;
		//cerr << "--->>> 可取资金: " << pTradingAccount->WithdrawQuota  << endl;
		//静态权益=上日结算-出金金额+入金金额
		tn.prebalance = pTradingAccount->PreBalance - pTradingAccount->Withdraw + pTradingAccount->Deposit;
		//cerr << "--->>> 静态权益: " << preBalance  << endl;
		//动态权益=静态权益+ 平仓盈亏+ 持仓盈亏- 手续费
		tn.current = tn.prebalance + pTradingAccount->CloseProfit + pTradingAccount->PositionProfit - pTradingAccount->Commission;
		//cerr << "--->>> 动态权益: " << current  << endl;

		YestayAllAmount = tn.prebalance; //静态权益
		TodayAllAmount = tn.current; //动态权益
		tn.available = pTradingAccount->Available;  //可用资金
		Available = tn.available;
		printf("Available: %f\n", tn.available);
		/*
		//检查交易日志文件是否存在，是否需要新建文本文件
		if (LogFilePaths[0] == '\0')
		{
			strcpy(LogFilePaths, "./AutoTrade/");
			strcat_s(LogFilePaths, pTradingAccount->TradingDay);
			strcat_s(LogFilePaths, ".txt");

			//检查文件是否存在，是否需要新建文本文件
			ifstream inf;
			ofstream ouf;
			inf.open(LogFilePaths, ios::out);
		}
		*/
		///请求查询投资者持仓
		//for (int i = 0; i < 20; i++)
		//{
		//strcpy(INSTRUMENT_ID, InstrumentID_n[3]);
		//ReqQryInvestorPosition();
		//}
	}
}


//增加
///请求查询合约响应
void CTDSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrument == NULL)
	{
		return;
	}


	//cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///合约
		/*
		struct CThostFtdcInstrumentField
		{
			///合约代码
			TThostFtdcInstrumentIDType	InstrumentID;
			///交易所代码
			TThostFtdcExchangeIDType	ExchangeID;
			///合约名称
			TThostFtdcInstrumentNameType	InstrumentName;
			///合约在交易所的代码
			TThostFtdcExchangeInstIDType	ExchangeInstID;
			///产品代码
			TThostFtdcInstrumentIDType	ProductID;
			///产品类型
			TThostFtdcProductClassType	ProductClass;
			///交割年份
			TThostFtdcYearType	DeliveryYear;
			///交割月
			TThostFtdcMonthType	DeliveryMonth;
			///市价单最大下单量
			TThostFtdcVolumeType	MaxMarketOrderVolume;
			///市价单最小下单量
			TThostFtdcVolumeType	MinMarketOrderVolume;
			///限价单最大下单量
			TThostFtdcVolumeType	MaxLimitOrderVolume;
			///限价单最小下单量
			TThostFtdcVolumeType	MinLimitOrderVolume;
			///合约数量乘数
			TThostFtdcVolumeMultipleType	VolumeMultiple;
			///最小变动价位
			TThostFtdcPriceType	PriceTick;
			///创建日
			TThostFtdcDateType	CreateDate;
			///上市日
			TThostFtdcDateType	OpenDate;
			///到期日
			TThostFtdcDateType	ExpireDate;
			///开始交割日
			TThostFtdcDateType	StartDelivDate;
			///结束交割日
			TThostFtdcDateType	EndDelivDate;
			///合约生命周期状态
			TThostFtdcInstLifePhaseType	InstLifePhase;
			///当前是否交易
			TThostFtdcBoolType	IsTrading;
			///持仓类型
			TThostFtdcPositionTypeType	PositionType;
			///持仓日期类型
			TThostFtdcPositionDateTypeType	PositionDateType;
			///多头保证金率
			TThostFtdcRatioType	LongMarginRatio;
			///空头保证金率
			TThostFtdcRatioType	ShortMarginRatio;
			///是否使用大额单边保证金算法
			TThostFtdcMaxMarginSideAlgorithmType	MaxMarginSideAlgorithm;
			///基础商品代码
			TThostFtdcInstrumentIDType	UnderlyingInstrID;
			///执行价
			TThostFtdcPriceType	StrikePrice;
			///期权类型
			TThostFtdcOptionsTypeType	OptionsType;
			///合约基础商品乘数
			TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;
			///组合类型
			TThostFtdcCombinationTypeType	CombinationType;
		}
		*/
 
 
	//	printf("VVV乘数 InstrumentID[%s] UnderlyingMultiple[%d]\n\n", pInstrument->InstrumentID, pInstrument->UnderlyingMultiple  );
 

       gUnderlyingMultiple[pInstrument->InstrumentID] =  pInstrument->UnderlyingMultiple;// LongMarginRatioByMoney;// gPosition_S_Today[pInstrumentMarginRate->InstrumentID] + gPosition_S_History[pInstrumentMarginRate->InstrumentID];
 
	    
	}
 
}

void CTDSpi::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInvestorProductGroupMargin == NULL)
	{
		return;
	}

 	/*
 
///投资者品种/跨品种保证金
struct CThostFtdcInvestorProductGroupMarginField
{

	///品种/跨品种标示
	TThostFtdcInstrumentIDType	ProductGroupID;
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///交易日
	TThostFtdcDateType	TradingDay;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;
	///冻结的保证金
	TThostFtdcMoneyType	FrozenMargin;
	///多头冻结的保证金
	TThostFtdcMoneyType	LongFrozenMargin;
	///空头冻结的保证金
	TThostFtdcMoneyType	ShortFrozenMargin;
	///占用的保证金
	TThostFtdcMoneyType	UseMargin;
	///多头保证金
	TThostFtdcMoneyType	LongUseMargin;
	///空头保证金
	TThostFtdcMoneyType	ShortUseMargin;
	///交易所保证金
	TThostFtdcMoneyType	ExchMargin;
	///交易所多头保证金
	TThostFtdcMoneyType	LongExchMargin;
	///交易所空头保证金
	TThostFtdcMoneyType	ShortExchMargin;
	///平仓盈亏
	TThostFtdcMoneyType	CloseProfit;
	///冻结的手续费
	TThostFtdcMoneyType	FrozenCommission;
	///手续费
	TThostFtdcMoneyType	Commission;
	///冻结的资金
	TThostFtdcMoneyType	FrozenCash;
	///资金差额
	TThostFtdcMoneyType	CashIn;
	///持仓盈亏
	TThostFtdcMoneyType	PositionProfit;
	///折抵总金额
	TThostFtdcMoneyType	OffsetAmount;
	///多头折抵总金额
	TThostFtdcMoneyType	LongOffsetAmount;
	///空头折抵总金额
	TThostFtdcMoneyType	ShortOffsetAmount;
	///交易所折抵总金额
	TThostFtdcMoneyType	ExchOffsetAmount;
	///交易所多头折抵总金额
	TThostFtdcMoneyType	LongExchOffsetAmount;
	///交易所空头折抵总金额
	TThostFtdcMoneyType	ShortExchOffsetAmount;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
};
	*/

	printf("\n$$$BrokerID[%s]  UseMargin[%0.04f] LongUseMargin[%0.04f]  ShortUseMargin[%0.04f] ExchMargin[%0.08f] LongExchMargin[%0.08f] ShortExchMargin[%0.08f] Commission[%0.08f]\n",
		pInvestorProductGroupMargin->BrokerID,
		pInvestorProductGroupMargin->UseMargin,
		pInvestorProductGroupMargin->LongUseMargin,
		pInvestorProductGroupMargin->ShortUseMargin,
		pInvestorProductGroupMargin->ExchMargin,
		pInvestorProductGroupMargin->LongExchMargin,
		pInvestorProductGroupMargin->ShortExchMargin,
		pInvestorProductGroupMargin->Commission);

}

void CTDSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrumentMarginRate == NULL)
	{
		//cout << "--->>>指针错误OnRspQryTradingAccount" << endl;			  //指针检查
		//WirteTradeRecordToFileMainThread(0, "OnRspQryTradingAccount指针错误");
		return;
	}

	//WirteTradeRecordToFileMainThread(0, "OnRspQryTradingAccount");

	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
         /*
		///合约保证金率
         struct CThostFtdcInstrumentMarginRateField
       {
	    ///合约代码
	    TThostFtdcInstrumentIDType	InstrumentID;
	    ///投资者范围
	    TThostFtdcInvestorRangeType	InvestorRange;
	    ///经纪公司代码
	    TThostFtdcBrokerIDType	BrokerID;
	    ///投资者代码
	    TThostFtdcInvestorIDType	InvestorID;
	    ///投机套保标志
	    TThostFtdcHedgeFlagType	HedgeFlag;
	    ///多头保证金率
	    TThostFtdcRatioType	LongMarginRatioByMoney;
	   ///多头保证金费
	     TThostFtdcMoneyType	LongMarginRatioByVolume;
	    ///空头保证金率
	    TThostFtdcRatioType	ShortMarginRatioByMoney;
	   ///空头保证金费
	    TThostFtdcMoneyType	ShortMarginRatioByVolume;
	    ///是否相对交易所收取
	    TThostFtdcBoolType	IsRelative;
        };
         */
		////pInstrumentMarginRate->BrokerID;
		//pInstrumentMarginRate->HedgeFlag;
	//	pInstrumentMarginRate->InstrumentID;
		//pInstrumentMarginRate->InvestorID;
		//pInstrumentMarginRate->InvestorRange;
		//pInstrumentMarginRate->IsRelative;
		//pInstrumentMarginRate->LongMarginRatioByMoney;
		//pInstrumentMarginRate->LongMarginRatioByVolume;
		//pInstrumentMarginRate->ShortMarginRatioByMoney;
		//pInstrumentMarginRate->ShortMarginRatioByVolume;
		// if(printfstate)
		 /*
		printf("BrokerID[%s] HedgeFlag[%c]InstrumentID[%s] InvestorID[%s] InvestorRange[%s] IsRelative[%d] LongMarginRatioByMoney[%0.02f] LongMarginRatioByVolume[%0.02f] ShortMarginRatioByMoney[%0.02f] ShortMarginRatioByVolume[%0.02f]\n",pInstrumentMarginRate->BrokerID,
		pInstrumentMarginRate->HedgeFlag,
		pInstrumentMarginRate->InstrumentID,
		pInstrumentMarginRate->InvestorID,
		pInstrumentMarginRate->InvestorRange,
		pInstrumentMarginRate->IsRelative,

		pInstrumentMarginRate->LongMarginRatioByMoney,
		pInstrumentMarginRate->LongMarginRatioByVolume,
		pInstrumentMarginRate->ShortMarginRatioByMoney,
		pInstrumentMarginRate->ShortMarginRatioByVolume);

		 printf("BrokerID[%s]  instrumentID[%s] InvestorRange[%c]  HedgeFlag[%c] LongMarginRatioByMoney[%0.08f] LongMarginRatioByVolume[%0.08f] ShortMarginRatioByMoney[%0.08f] ShortMarginRatioByVolume[%0.08f]\n", 
			 pInstrumentMarginRate->BrokerID,
			 pInstrumentMarginRate->InstrumentID,
			 pInstrumentMarginRate->InvestorRange,
			 pInstrumentMarginRate->HedgeFlag,
			 pInstrumentMarginRate->LongMarginRatioByMoney,
			 pInstrumentMarginRate->LongMarginRatioByVolume,
			 pInstrumentMarginRate->ShortMarginRatioByMoney,
			 pInstrumentMarginRate->ShortMarginRatioByVolume);
		*/
		 gMarginRate_long[pInstrumentMarginRate->InstrumentID] = pInstrumentMarginRate->LongMarginRatioByMoney;// LongMarginRatioByMoney;// gPosition_S_Today[pInstrumentMarginRate->InstrumentID] + gPosition_S_History[pInstrumentMarginRate->InstrumentID];
		 gMarginRate_short[pInstrumentMarginRate->InstrumentID] = pInstrumentMarginRate->ShortMarginRatioByMoney;// gPosition_S_Today[pInstrumentMarginRate->InstrumentID] + gPosition_S_History[pInstrumentMarginRate->InstrumentID];

		//cerr << "--->>> 交易日: " << pTradingAccount->TradingDay << "\n" << endl;
		//cerr << "--->>> \n可用资金: " << (int)(pTradingAccount->Available / 10000) << "万\n" << endl;
		//cerr << "--->>> 可取资金: " << pTradingAccount->WithdrawQuota  << endl;
		//静态权益=上日结算-出金金额+入金金额
		//double preBalance = pTradingAccount->PreBalance - pTradingAccount->Withdraw + pTradingAccount->Deposit;
		//cerr << "--->>> 静态权益: " << preBalance  << endl;
		//动态权益=静态权益+ 平仓盈亏+ 持仓盈亏- 手续费
		//double current = preBalance + pTradingAccount->CloseProfit + pTradingAccount->PositionProfit - pTradingAccount->Commission;
		//cerr << "--->>> 动态权益: " << current  << endl;

		//YestayAllAmount = preBalance; //静态权益
		//TodayAllAmount = current; //动态权益
		//Available = pTradingAccount->Available;  //可用资金
												  /*
												  //检查交易日志文件是否存在，是否需要新建文本文件
												  if (LogFilePaths[0] == '\0')
												  {
												  strcpy(LogFilePaths, "./AutoTrade/");
												  strcat_s(LogFilePaths, pTradingAccount->TradingDay);
												  strcat_s(LogFilePaths, ".txt");

												  //检查文件是否存在，是否需要新建文本文件
												  ifstream inf;
												  ofstream ouf;
												  inf.open(LogFilePaths, ios::out);
												  }
												  */
												  ///请求查询投资者持仓
												  //for (int i = 0; i < 20; i++)
												  //{
												  //strcpy(INSTRUMENT_ID, InstrumentID_n[3]);
												  //ReqQryInvestorPosition();
												  //}
	}
}



void CTDSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrumentCommissionRate == NULL)
	{
		return;
	}

	 cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{

		/*
		///合约手续费率
		struct CThostFtdcInstrumentCommissionRateField
		{
			///合约代码
			TThostFtdcInstrumentIDType	InstrumentID;
			///投资者范围
			TThostFtdcInvestorRangeType	InvestorRange;
			///经纪公司代码
			TThostFtdcBrokerIDType	BrokerID;
			///投资者代码
			TThostFtdcInvestorIDType	InvestorID;
			///开仓手续费率
			TThostFtdcRatioType	OpenRatioByMoney;
			///开仓手续费
			TThostFtdcRatioType	OpenRatioByVolume;
			///平仓手续费率
			TThostFtdcRatioType	CloseRatioByMoney;
			///平仓手续费
			TThostFtdcRatioType	CloseRatioByVolume;
			///平今手续费率
			TThostFtdcRatioType	CloseTodayRatioByMoney;
			///平今手续费
			TThostFtdcRatioType	CloseTodayRatioByVolume;
		};
          */
		pInstrumentCommissionRate->BrokerID;
		pInstrumentCommissionRate->CloseRatioByMoney;
		pInstrumentCommissionRate->CloseRatioByVolume;
		pInstrumentCommissionRate->CloseTodayRatioByMoney;
		pInstrumentCommissionRate->CloseTodayRatioByVolume;
		pInstrumentCommissionRate->InstrumentID;
		pInstrumentCommissionRate->InvestorID;
		pInstrumentCommissionRate->InvestorRange;
		pInstrumentCommissionRate->OpenRatioByMoney;



	}




}


///查询最大报单数量响应
void CTDSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pQueryMaxOrderVolume == NULL)
	{
		//cout << "--->>>指针错误OnRspQryTradingAccount" << endl;			  //指针检查
		//WirteTradeRecordToFileMainThread(0, "OnRspQryTradingAccount指针错误");
		return;
	}

	//WirteTradeRecordToFileMainThread(0, "OnRspQryTradingAccount");

	 cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{



	}


}




void CTDSpi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pAccountregister == NULL)
	{
		return;
	}

	/*
	///交易日期
	TThostFtdcTradeDateType	TradeDay;
	///银行编码
	TThostFtdcBankIDType	BankID;
	///银行分支机构编码
	TThostFtdcBankBrchIDType	BankBranchID;
	///银行帐号
	TThostFtdcBankAccountType	BankAccount;
	///期货公司编码
	TThostFtdcBrokerIDType	BrokerID;
	///期货公司分支机构编码
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///证件类型
	TThostFtdcIdCardTypeType	IdCardType;
	///证件号码
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///客户姓名
	TThostFtdcIndividualNameType	CustomerName;
	///币种代码
	TThostFtdcCurrencyIDType	CurrencyID;
	///开销户类别
	TThostFtdcOpenOrDestroyType	OpenOrDestroy;
	///签约日期
	TThostFtdcTradeDateType	RegDate;
	///解约日期
	TThostFtdcTradeDateType	OutDate;
	///交易ID
	TThostFtdcTIDType	TID;
	///客户类型
	TThostFtdcCustTypeType	CustType;
	///银行帐号类型
	TThostFtdcBankAccTypeType	BankAccType;
	///长客户姓名
	TThostFtdcLongIndividualNameType	LongCustomerName;
	*/


	printf("交易日期：%s\n", pAccountregister->TradeDay);
	printf("银行编码：%s\n", pAccountregister->BankID);
	printf("银行分支机构编码：%s\n", pAccountregister->BankBranchID);
	printf("银行帐号：%s\n", pAccountregister->BankAccount);
	printf("期货公司编码：%s\n", pAccountregister->BrokerID);
	printf("期货公司分支机构编码：%s\n", pAccountregister->BrokerBranchID);
	printf("投资者帐号：%s\n", pAccountregister->AccountID);
	printf("证件类型：%c\n", pAccountregister->IdCardType);
	printf("证件号码：%s\n", pAccountregister->IdentifiedCardNo);

	printf("客户姓名：%s\n", pAccountregister->CustomerName);
	printf("币种代码：%s\n", pAccountregister->CurrencyID);
	printf("开销户类别：%c\n", pAccountregister->OpenOrDestroy);
}






///期货发起银行资金转期货通知
void CTDSpi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	if (pRspTransfer == NULL)
	{
		return;
	}
}


///期货发起期货资金转银行通知
void CTDSpi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	if (pRspTransfer == NULL)
	{
		return;
	}

}

