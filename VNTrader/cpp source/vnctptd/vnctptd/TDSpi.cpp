/*
1.���ļ�ΪVNTrader �ڻ�CTP���׿�ײ����
2.VNTrader����C++�⿪ԴЭ��MITЭ��

��VNPY��Դ���������׵ģ����õ�ԭʼ���߿϶��ģ���������http://www.vnpy.cn��վ�ϣ�
������ڡ���Դ˵���͸�л.txt�����������ļ����ϸ��·���ÿһ���°汾��vnpy���

�ٷ���վ��http://www.vnpy.cn
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

//����
std::map<std::string, double> gUnderlyingMultiple;
char	InstrumentID_n[TYPE_NUM][10] = { 0 };


//��֤����
std::map<std::string, double> gMarginRate_long;
std::map<std::string, double> gMarginRate_short;
//��������
std::map<std::string, double> gCommissionRate;

//��ѯ��󱨵�����
std::map<std::string, int> gMaxOrderVolume;
  


//�ֲ�map
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




//��Ȩ����
CThostFtdcTraderApi *vntdapi;
//�Լ�����
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
DWORD WINAPI PositionThreadProc(void* p)	//��������
{
	while (true)
	{
		GState = !GState;
		if (GState)
		{
			if(vntdspi)
				vntdspi->ReqQryInvestorPosition();//��ѯ��λ����		
		}
		else
		{
			if (vntdspi)
				vntdspi->ReqQryTradingAccount(); //��ѯ�ʽ�
		}
        //CTP��1S���أ�̫��Ĳ�ѯ�ᵼ�²�ѯʧ��
		Sleep(1016);
	}

}

DWORD WINAPI ReqQryInstrumentMarginRateThreadProc(void* p)	//��������
{
	if (vntdspi)
	 	vntdspi->ReqQryInstrumentMarginRate("rb1701");//��λ����		
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

	vntdspi = this;//�Լ�����
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
	//��ѯ�ֲ��߳�
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
	//��֤����
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
		std::cout << "OnRspAuthenticate����\n" << std::endl;
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
			printf("failer:��¼���������ʧ��\n");
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
			printf("�ǳ�ʧ��,ErrorID=0x%04x, ErrMsg=%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		}
		else
		{
			printf("�ǳ��ɹ�\n");
		}
	}
}

int CTDSpi::ReqSettlementInfoConfirm()
{
	//Ͷ���߽�����ȷ��
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
		//ȷ�Ͻ��㵥�ɹ�
	}
}

bool CTDSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
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
	printf("�����ɽ�[%d]\n", orderRef);
	//
	/*
	::WaitForSingleObject(ghTradedVolMutex, INFINITE);
	gOrderRef2TradedVol[orderRef] = pTrade->VolumeTraded;
	::ReleaseMutex(ghTradedVolMutex);
	*/
	///�ɽ�


}

//��Լ����״̬֪ͨ
 
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
	//���󷵻�-1�Ǻ�������ͻ����
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
		cerr << "Failer: ���� : " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	else
		cerr << "Scuess: ���� : �ɹ�" << endl;

	return iResult;
}

VOID MakeOrder(CThostFtdcInputOrderField *pOrder)
{
	memset(pOrder, 0, sizeof(*pOrder));
	///���͹�˾����
	strcpy_s(pOrder->BrokerID, sizeof(TThostFtdcBrokerIDType),gBrokerID.c_str());
	///Ͷ���ߴ���
	strcpy_s(pOrder->InvestorID, sizeof(TThostFtdcInvestorIDType),gInvestorID.c_str());
	///�û�����
	//TThostFtdcUserIDType	UserID;
	///���Ͷ���ױ���־
	pOrder->CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	
	///�޼۵�
	pOrder->OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///��������: ����
	pOrder->ContingentCondition = THOST_FTDC_CC_Immediately;
	///��Ч������: ������Ч
	pOrder->TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
	//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	pOrder->VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	pOrder->MinVolume = 1;
	///ֹ���
	//	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��: ��ǿƽ
	pOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	pOrder->IsAutoSuspend = 0;
	///ҵ��Ԫ
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
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
		cerr << "Failer: �µ�" << InstrumentID << price << num << ": " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	else
		cerr << "Scuess: �µ�"<< InstrumentID  << price << num <<": �ɹ�" << endl;

	return iOrderRef;
}


///////////////////////////////////////////////////////////////////////////////�Լ�����



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
	//req.InstrumentID; //ָ����Լ�Ļ������ǲ�ѯ�ض���Լ�ĳֲ���Ϣ��������ǲ�ѯ���гֲ�  
	//ReqQryInvestorPosition(&req, reqInfo.nRequestID);
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(CThostFtdcQryInvestorPositionField));
	//strcpy(req.BrokerID, BROKER_ID);
	//strcpy(req.InvestorID, INVESTOR_ID);

	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(req.InvestorID,sizeof(TThostFtdcInvestorIDType),gInvestorID.c_str());
	//strcpy(req.InstrumentID, INSTRUMENT_ID);
	//printf("ָ���ֲ�%s", INSTRUMENT_ID);
	//int iResult = vntdapi->ReqQryInvestorPosition(&req, ++iRequestID);

	return vntdapi->ReqQryInvestorPosition(&req, ++iRequestID);
	//cerr << "�����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
	//if (iResult != 0)
	//	cerr << "Failer(ReqQryInvestorPosition): �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	//else
	//	cerr << "Scuess(ReqQryInvestorPosition): �����ѯͶ���ֲ߳�: �ɹ�" << endl;

}

///�����ѯͶ����Ʒ��/��Ʒ�ֱ�֤��
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
	//req.InstrumentID; //ָ����Լ�Ļ������ǲ�ѯ�ض���Լ�ĳֲ���Ϣ��������ǲ�ѯ���гֲ�  
	//ReqQryInvestorPosition(&req, reqInfo.nRequestID);
	//CThostFtdcQryInvestorProductGroupMarginField
	/*
	///��ѯͶ����Ʒ��/��Ʒ�ֱ�֤��
	struct CThostFtdcQryInvestorProductGroupMarginField
	{
		///���͹�˾����
		TThostFtdcBrokerIDType	BrokerID;
		///Ͷ���ߴ���
		TThostFtdcInvestorIDType	InvestorID;
		///Ʒ��/��Ʒ�ֱ�ʾ
		TThostFtdcInstrumentIDType	ProductGroupID;
		///Ͷ���ױ���־
		TThostFtdcHedgeFlagType	HedgeFlag;
	};
	*/
	CThostFtdcQryInvestorProductGroupMarginField req;
	//CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(CThostFtdcQryInvestorProductGroupMarginField));

	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(req.InvestorID,sizeof(TThostFtdcInvestorIDType), gInvestorID.c_str());
	req.HedgeFlag = '1';
 


 	//printf("ָ���ֲ�%s", INSTRUMENT_ID);
	int iResult = vntdapi->ReqQryInvestorProductGroupMargin(&req, ++iRequestID);
	//cerr << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;

	if (iResult != 0)
		cerr << "Failer(ReqQryInvestorProductGroupMargin): �����ѯͶ����Ʒ��/��Ʒ�ֱ�֤��: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqQryInvestorProductGroupMargin): �����ѯͶ����Ʒ��/��Ʒ�ֱ�֤��: �ɹ�" << endl;
}


int CTDSpi::ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID)
{
	std::cout << __FUNCTION__ << std::endl;

	int iResult = vntdapi->ReqQueryMaxOrderVolume(pQueryMaxOrderVolume, ++iRequestID);
	if (iResult != 0)
		cerr << "Failer(ReqQueryMaxOrderVolume): ��ѯ�µ����ֵ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqQueryMaxOrderVolume): ��ѯ�µ����ֵ: �ɹ�" << endl;

	return iResult;
}

///�ڻ����������ʽ�ת�ڻ�����
int CTDSpi::ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID)
{
	std::cout << __FUNCTION__ << std::endl;

	int iResult = vntdapi->ReqFromBankToFutureByFuture(pReqTransfer, ++iRequestID);

	if (iResult != 0)
		cerr << "Failer(ReqFromBankToFutureByFuture): �ڻ����������ʽ�ת�ڻ�����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqFromBankToFutureByFuture): �ڻ����������ʽ�ת�ڻ�����: �ɹ�" << endl;

	return iResult;
}

///�ڻ������ڻ��ʽ�ת��������
int CTDSpi::ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID)
{
	std::cout << __FUNCTION__ << std::endl;

	int iResult = vntdapi->ReqFromFutureToBankByFuture(pReqTransfer, ++iRequestID);

	if (iResult != 0)
		cerr << "Failer(ReqFromFutureToBankByFuture): �ڻ������ڻ��ʽ�ת��������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqFromFutureToBankByFuture): �ڻ������ڻ��ʽ�ת��������: �ɹ�" << endl;

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

	//printf("��ѯ����[%s]   [%s]  [%s]", req.ExchangeID, req.ProductID, req.InstrumentID);

	int iResult = vntdapi->ReqQryInstrument(&req, ++iRequestID);
	//cerr << "--->>> �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;

	if (iResult != 0)
		cerr << "Failer(ReqQryInstrument): �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqQryInstrument): �����ѯ��Լ: �ɹ�" << endl;
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
	//req.InstrumentID; //ָ����Լ�Ļ������ǲ�ѯ�ض���Լ�ĳֲ���Ϣ��������ǲ�ѯ���гֲ�  
	//ReqQryInvestorPosition(&req, reqInfo.nRequestID);

	CThostFtdcQryInstrumentMarginRateField req;
	memset(&req, 0, sizeof(CThostFtdcQryInstrumentMarginRateField));

	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	strcpy_s(req.InvestorID,sizeof(TThostFtdcInvestorIDType), gInvestorID.c_str());
	strcpy_s(req.InstrumentID,sizeof(TThostFtdcInstrumentIDType), Instrument);
	req.HedgeFlag = '1';

	printf("��ѯ��֤��[%s] Ͷ�����˻�[%s]��Լ[%s]\n",req.BrokerID, req.InvestorID, req.InstrumentID);
	//1>CTPTraderSpi.cpp(654) : error C2664 : ��int CThostFtdcTraderApi::ReqQryInstrumentMarginRate(
		//CThostFtdcQryInstrumentMarginRateField *, int)�� : �޷������� 1 �ӡ�
		//CThostFtdcInstrumentMarginRateField *��ת��Ϊ��CThostFtdcQryInstrumentMarginRateField *��
	//printf("\n----------------------------------------\n");

	//strcpy(req.InstrumentID, INSTRUMENT_ID);
	//printf("ָ���ֲ�%s", INSTRUMENT_ID);
	int iResult = vntdapi->ReqQryInstrumentMarginRate(&req, ++iRequestID);
	cerr << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;


 
}



///�����ѯ��Լ
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
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
	*/
	//strcpy(req.BrokerID, gBrokerID.c_str());
	//strcpy(req.InvestorID, gInvestorID.c_str());
	//strcpy(req.InstrumentID, Instrument);
	int iResult = vntdapi->ReqQryInstrument(&req, ++iRequestID);
	if (iResult != 0)
		cerr << "Failer(ReqQryInstrument): �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult << ")" << endl;
	else
		cerr << "Scuess(ReqQryInstrument): �����ѯ��Լ: �ɹ�" << endl;
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
	// cerr << "Failer: �����ѯ����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult<<")"<<endl;
	if(iResult!=0)
	    cerr << "Failer(ReqQryContractBank): �����ѯ����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��(") << iResult<<")"<<endl;
	else
		cerr << "Scuess(ReqQryContractBank): �����ѯ����: �ɹ�" <<endl;
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
	{	//printf("��%s����%s\n", InstrumentID_n[id],str );
		return true;
	}
	else
	{
		//printf("%s û����%s����\n", str, InstrumentID_n[id]);
		return false;
	}
}

int SaveInstrumentID = { 0 };
bool  checkstate = false;
bool  TypeCheckState_B_Today[TYPE_NUM] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool  TypeCheckState_S_Today[TYPE_NUM] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

bool  TypeCheckState_B_History[TYPE_NUM] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool  TypeCheckState_S_History[TYPE_NUM] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
///�����ѯͶ���ֲ߳���Ӧ

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
			printf("��β�ѯ�ֲ�ָ�����,���������ļ���ѡ��Ͽ�Ľ��׷�����\n");
		}
		return;
	}
	errnum = 0;



	//WirteTradeRecordToFileMainThread(0, "�����ѯͶ���ֲ߳���ӦOnRspQryInvestorPosition");

	//cerr << "--->>> " << "Ͷ���ֲ߳�����:" << endl;
	// if (bIsLast && !IsErrorRspInfo(pRspInfo))
	//if (bIsLast)
	//{


	/*
	cout << "OnRspQryInvestorPosition  ID: " << nRequestID << endl;
	cout << "��Լ����  ID: " << pInvestorPosition->InstrumentID << endl;
	//cout << "������룺" << pRspInfo->ErrorID << "������Ϣ:" << pRspInfo->ErrorMsg;
	cout << "�ֲֶ�շ���:" << pInvestorPosition->PosiDirection << endl;  //2�൥��3�յ�
	cout << "��λ:" << pInvestorPosition->Position << endl;
	*/

/*
	cout << "[ID]" << nRequestID << "��Լ:"
		<< pInvestorPosition->InstrumentID
		<< "�ֲַ���:" << pInvestorPosition->PosiDirection
		<< "��λ:" << pInvestorPosition->Position
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
	[ID]22��Լ:m1609�ֲַ���:2��λ : 0BrokerID : 0AbandonFrozen : 8016CashIn : 0
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
		//printf("�²�ѯ��ʼ��\n");
	}
 
	//for (int i = 0; i <TYPE_NUM; i++)
	//{
		//if (FindStr(i, pInvestorPosition->InstrumentID))
		//{
	
			if (pInvestorPosition->PosiDirection == '2' &&  pInvestorPosition->Position != 0) //ע���İ汾Ҫ�޸�
			{
				if (pInvestorPosition->TodayPosition == 0) //���
				{
					//TypeCheckState_B_History[i] = true;
					gTypeCheckState_B_History[pInvestorPosition->InstrumentID] = true;
					//Trade_dataA_Amount_B_History[i] = pInvestorPosition->Position;

					//�ֲ�map
					gPosition_B_History[pInvestorPosition->InstrumentID] = pInvestorPosition->Position;

					//printf("��ʷ�� [%d]\n", gPosition_B_History[pInvestorPosition->InstrumentID]);

				}
				else
				{
					//TypeCheckState_B_Today[i] = true;
					gTypeCheckState_B_Today[pInvestorPosition->InstrumentID] = true;
					//Trade_dataA_Amount_B_Today[i] = pInvestorPosition->Position;

					//�ֲ�map
					gPosition_B_Today[pInvestorPosition->InstrumentID] = pInvestorPosition->Position;

					//printf("����� [%d]\n", gPosition_B_Today[pInvestorPosition->InstrumentID]);
				}
			}
			else if (pInvestorPosition->PosiDirection == '3' &&  pInvestorPosition->Position != 0)  //ע���İ汾Ҫ�޸�
			{
				if (pInvestorPosition->TodayPosition == 0) //���
				{
					//TypeCheckState_S_History[i] = true;
					gTypeCheckState_S_History[pInvestorPosition->InstrumentID] = true;
					//Trade_dataA_Amount_S_History[i] = pInvestorPosition->Position;

					//�ֲ�map
					gPosition_S_History[pInvestorPosition->InstrumentID] = (pInvestorPosition->Position);

					//printf("��ʷ���� [%d]\n", gPosition_S_History[pInvestorPosition->InstrumentID]);
				}
				else
				{
					//TypeCheckState_S_Today[i] = true;
					gTypeCheckState_S_Today[pInvestorPosition->InstrumentID] = true;
					//Trade_dataA_Amount_S_Today[i] = pInvestorPosition->Position;

					//�ֲ�map
					gPosition_S_Today[pInvestorPosition->InstrumentID] = (pInvestorPosition->Position);

					//printf("������� [%d]\n", gPosition_S_Today[pInvestorPosition->InstrumentID]);
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
				//�ֲ�map
				gPosition_B_History[pInvestorPosition->InstrumentID] = 0;
				//printf("��ʷ���0 [%d]\n", gPosition_B_History[pInvestorPosition->InstrumentID]);
			}


			if(!gTypeCheckState_B_Today[pInvestorPosition->InstrumentID])
			//if (!TypeCheckState_B_Today[i])
			{
				//Trade_dataA_Amount_B_Today[i] = 0;
				//�ֲ�map
				gPosition_B_Today[pInvestorPosition->InstrumentID] = 0;

				//printf("�������0 [%d]\n", gPosition_B_Today[pInvestorPosition->InstrumentID]);
			}

			if(!gTypeCheckState_S_History[pInvestorPosition->InstrumentID])
			//if (!TypeCheckState_S_History[i])
			{
				//Trade_dataA_Amount_S_History[i] = 0;
				//�ֲ�map
				gPosition_S_History[pInvestorPosition->InstrumentID] = 0;

				//printf("��ʷ����0 [%d]\n", gPosition_S_History[pInvestorPosition->InstrumentID]);

			}

			if(!gTypeCheckState_S_Today[pInvestorPosition->InstrumentID])
			//if (!TypeCheckState_S_Today[i])
			{
				//Trade_dataA_Amount_S_Today[i] = 0;
				//�ֲ�map
				gPosition_S_Today[pInvestorPosition->InstrumentID] = 0;

				//printf("��������0 [%d]\n", gPosition_S_Today[pInvestorPosition->InstrumentID]);
			}
			if (1)
			{
			//	printf("Ʒ�֣�%s  BUY�ֲ�(���)[%d] BUY�ֲ�(��ʷ��)[%d] SELL�ֲ�(���)[%d] SELL�ֲ�(��ʷ)[%d]\n", InstrumentID_n[i], Trade_dataA_Amount_B_Today[i], Trade_dataA_Amount_B_History[i], Trade_dataA_Amount_S_Today[i], Trade_dataA_Amount_S_History[i]);
			}

		}



	}

	gPosition_S[pInvestorPosition->InstrumentID] = gPosition_S_Today[pInvestorPosition->InstrumentID] + gPosition_S_History[pInvestorPosition->InstrumentID];
	gPosition_B[pInvestorPosition->InstrumentID] = gPosition_B_Today[pInvestorPosition->InstrumentID] + gPosition_B_History[pInvestorPosition->InstrumentID];

	if (showpositionstate)
	{
		printf("%s ��������[%d] ������[%d] �ǽ�������[%d] ����[%d] ����[%d] �ǽ�����[%d]\n", pInvestorPosition->InstrumentID,
		gPosition_S[pInvestorPosition->InstrumentID], gPosition_S_Today[pInvestorPosition->InstrumentID], gPosition_S_History[pInvestorPosition->InstrumentID],
		gPosition_B[pInvestorPosition->InstrumentID], gPosition_B_Today[pInvestorPosition->InstrumentID] , gPosition_B_History[pInvestorPosition->InstrumentID]);
	}



	//TypeNumber[nRequestID] = pInvestorPosition->Position;
	//printf("��� ��%d\n",  pInvestorPosition->Position);

	//printf("Ʒ�֣�%s\n", InstrumentID_n[nRequestID]);

	//printf("*******************************************************************\n");
	///��Լ��֤���Լ������Ѳ�ѯ����
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
 
	cerr << "--->>> " << "IsErrorRspInfo\n" << "0" << endl;	  //ָ����
															  // ���ErrorID != 0, ˵���յ��˴������Ӧ
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
	// ��̬Ȩ��
	double prebalance;
	//��̬Ȩ��
	double current;
 	//����Ȩ��
	double available;
    //����ӯ��
	double rate;
	//��λ
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
		//cerr << "--->>> ������: " << pTradingAccount->TradingDay << "\n" << endl;
		//cerr << "--->>> \n�����ʽ�: " << (int)(pTradingAccount->Available / 10000) << "��\n" << endl;
		//cerr << "--->>> ��ȡ�ʽ�: " << pTradingAccount->WithdrawQuota  << endl;
		//��̬Ȩ��=���ս���-������+�����
		tn.prebalance = pTradingAccount->PreBalance - pTradingAccount->Withdraw + pTradingAccount->Deposit;
		//cerr << "--->>> ��̬Ȩ��: " << preBalance  << endl;
		//��̬Ȩ��=��̬Ȩ��+ ƽ��ӯ��+ �ֲ�ӯ��- ������
		tn.current = tn.prebalance + pTradingAccount->CloseProfit + pTradingAccount->PositionProfit - pTradingAccount->Commission;
		//cerr << "--->>> ��̬Ȩ��: " << current  << endl;

		YestayAllAmount = tn.prebalance; //��̬Ȩ��
		TodayAllAmount = tn.current; //��̬Ȩ��
		tn.available = pTradingAccount->Available;  //�����ʽ�
		Available = tn.available;
		printf("Available: %f\n", tn.available);
		/*
		//��齻����־�ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
		if (LogFilePaths[0] == '\0')
		{
			strcpy(LogFilePaths, "./AutoTrade/");
			strcat_s(LogFilePaths, pTradingAccount->TradingDay);
			strcat_s(LogFilePaths, ".txt");

			//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
			ifstream inf;
			ofstream ouf;
			inf.open(LogFilePaths, ios::out);
		}
		*/
		///�����ѯͶ���ֲ߳�
		//for (int i = 0; i < 20; i++)
		//{
		//strcpy(INSTRUMENT_ID, InstrumentID_n[3]);
		//ReqQryInvestorPosition();
		//}
	}
}


//����
///�����ѯ��Լ��Ӧ
void CTDSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrument == NULL)
	{
		return;
	}


	//cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///��Լ
		/*
		struct CThostFtdcInstrumentField
		{
			///��Լ����
			TThostFtdcInstrumentIDType	InstrumentID;
			///����������
			TThostFtdcExchangeIDType	ExchangeID;
			///��Լ����
			TThostFtdcInstrumentNameType	InstrumentName;
			///��Լ�ڽ������Ĵ���
			TThostFtdcExchangeInstIDType	ExchangeInstID;
			///��Ʒ����
			TThostFtdcInstrumentIDType	ProductID;
			///��Ʒ����
			TThostFtdcProductClassType	ProductClass;
			///�������
			TThostFtdcYearType	DeliveryYear;
			///������
			TThostFtdcMonthType	DeliveryMonth;
			///�м۵�����µ���
			TThostFtdcVolumeType	MaxMarketOrderVolume;
			///�м۵���С�µ���
			TThostFtdcVolumeType	MinMarketOrderVolume;
			///�޼۵�����µ���
			TThostFtdcVolumeType	MaxLimitOrderVolume;
			///�޼۵���С�µ���
			TThostFtdcVolumeType	MinLimitOrderVolume;
			///��Լ��������
			TThostFtdcVolumeMultipleType	VolumeMultiple;
			///��С�䶯��λ
			TThostFtdcPriceType	PriceTick;
			///������
			TThostFtdcDateType	CreateDate;
			///������
			TThostFtdcDateType	OpenDate;
			///������
			TThostFtdcDateType	ExpireDate;
			///��ʼ������
			TThostFtdcDateType	StartDelivDate;
			///����������
			TThostFtdcDateType	EndDelivDate;
			///��Լ��������״̬
			TThostFtdcInstLifePhaseType	InstLifePhase;
			///��ǰ�Ƿ���
			TThostFtdcBoolType	IsTrading;
			///�ֲ�����
			TThostFtdcPositionTypeType	PositionType;
			///�ֲ���������
			TThostFtdcPositionDateTypeType	PositionDateType;
			///��ͷ��֤����
			TThostFtdcRatioType	LongMarginRatio;
			///��ͷ��֤����
			TThostFtdcRatioType	ShortMarginRatio;
			///�Ƿ�ʹ�ô��߱�֤���㷨
			TThostFtdcMaxMarginSideAlgorithmType	MaxMarginSideAlgorithm;
			///������Ʒ����
			TThostFtdcInstrumentIDType	UnderlyingInstrID;
			///ִ�м�
			TThostFtdcPriceType	StrikePrice;
			///��Ȩ����
			TThostFtdcOptionsTypeType	OptionsType;
			///��Լ������Ʒ����
			TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;
			///�������
			TThostFtdcCombinationTypeType	CombinationType;
		}
		*/
 
 
	//	printf("VVV���� InstrumentID[%s] UnderlyingMultiple[%d]\n\n", pInstrument->InstrumentID, pInstrument->UnderlyingMultiple  );
 

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
 
///Ͷ����Ʒ��/��Ʒ�ֱ�֤��
struct CThostFtdcInvestorProductGroupMarginField
{

	///Ʒ��/��Ʒ�ֱ�ʾ
	TThostFtdcInstrumentIDType	ProductGroupID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	TThostFtdcSettlementIDType	SettlementID;
	///����ı�֤��
	TThostFtdcMoneyType	FrozenMargin;
	///��ͷ����ı�֤��
	TThostFtdcMoneyType	LongFrozenMargin;
	///��ͷ����ı�֤��
	TThostFtdcMoneyType	ShortFrozenMargin;
	///ռ�õı�֤��
	TThostFtdcMoneyType	UseMargin;
	///��ͷ��֤��
	TThostFtdcMoneyType	LongUseMargin;
	///��ͷ��֤��
	TThostFtdcMoneyType	ShortUseMargin;
	///��������֤��
	TThostFtdcMoneyType	ExchMargin;
	///��������ͷ��֤��
	TThostFtdcMoneyType	LongExchMargin;
	///��������ͷ��֤��
	TThostFtdcMoneyType	ShortExchMargin;
	///ƽ��ӯ��
	TThostFtdcMoneyType	CloseProfit;
	///�����������
	TThostFtdcMoneyType	FrozenCommission;
	///������
	TThostFtdcMoneyType	Commission;
	///������ʽ�
	TThostFtdcMoneyType	FrozenCash;
	///�ʽ���
	TThostFtdcMoneyType	CashIn;
	///�ֲ�ӯ��
	TThostFtdcMoneyType	PositionProfit;
	///�۵��ܽ��
	TThostFtdcMoneyType	OffsetAmount;
	///��ͷ�۵��ܽ��
	TThostFtdcMoneyType	LongOffsetAmount;
	///��ͷ�۵��ܽ��
	TThostFtdcMoneyType	ShortOffsetAmount;
	///�������۵��ܽ��
	TThostFtdcMoneyType	ExchOffsetAmount;
	///��������ͷ�۵��ܽ��
	TThostFtdcMoneyType	LongExchOffsetAmount;
	///��������ͷ�۵��ܽ��
	TThostFtdcMoneyType	ShortExchOffsetAmount;
	///Ͷ���ױ���־
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
		//cout << "--->>>ָ�����OnRspQryTradingAccount" << endl;			  //ָ����
		//WirteTradeRecordToFileMainThread(0, "OnRspQryTradingAccountָ�����");
		return;
	}

	//WirteTradeRecordToFileMainThread(0, "OnRspQryTradingAccount");

	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
         /*
		///��Լ��֤����
         struct CThostFtdcInstrumentMarginRateField
       {
	    ///��Լ����
	    TThostFtdcInstrumentIDType	InstrumentID;
	    ///Ͷ���߷�Χ
	    TThostFtdcInvestorRangeType	InvestorRange;
	    ///���͹�˾����
	    TThostFtdcBrokerIDType	BrokerID;
	    ///Ͷ���ߴ���
	    TThostFtdcInvestorIDType	InvestorID;
	    ///Ͷ���ױ���־
	    TThostFtdcHedgeFlagType	HedgeFlag;
	    ///��ͷ��֤����
	    TThostFtdcRatioType	LongMarginRatioByMoney;
	   ///��ͷ��֤���
	     TThostFtdcMoneyType	LongMarginRatioByVolume;
	    ///��ͷ��֤����
	    TThostFtdcRatioType	ShortMarginRatioByMoney;
	   ///��ͷ��֤���
	    TThostFtdcMoneyType	ShortMarginRatioByVolume;
	    ///�Ƿ���Խ�������ȡ
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

		//cerr << "--->>> ������: " << pTradingAccount->TradingDay << "\n" << endl;
		//cerr << "--->>> \n�����ʽ�: " << (int)(pTradingAccount->Available / 10000) << "��\n" << endl;
		//cerr << "--->>> ��ȡ�ʽ�: " << pTradingAccount->WithdrawQuota  << endl;
		//��̬Ȩ��=���ս���-������+�����
		//double preBalance = pTradingAccount->PreBalance - pTradingAccount->Withdraw + pTradingAccount->Deposit;
		//cerr << "--->>> ��̬Ȩ��: " << preBalance  << endl;
		//��̬Ȩ��=��̬Ȩ��+ ƽ��ӯ��+ �ֲ�ӯ��- ������
		//double current = preBalance + pTradingAccount->CloseProfit + pTradingAccount->PositionProfit - pTradingAccount->Commission;
		//cerr << "--->>> ��̬Ȩ��: " << current  << endl;

		//YestayAllAmount = preBalance; //��̬Ȩ��
		//TodayAllAmount = current; //��̬Ȩ��
		//Available = pTradingAccount->Available;  //�����ʽ�
												  /*
												  //��齻����־�ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
												  if (LogFilePaths[0] == '\0')
												  {
												  strcpy(LogFilePaths, "./AutoTrade/");
												  strcat_s(LogFilePaths, pTradingAccount->TradingDay);
												  strcat_s(LogFilePaths, ".txt");

												  //����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
												  ifstream inf;
												  ofstream ouf;
												  inf.open(LogFilePaths, ios::out);
												  }
												  */
												  ///�����ѯͶ���ֲ߳�
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
		///��Լ��������
		struct CThostFtdcInstrumentCommissionRateField
		{
			///��Լ����
			TThostFtdcInstrumentIDType	InstrumentID;
			///Ͷ���߷�Χ
			TThostFtdcInvestorRangeType	InvestorRange;
			///���͹�˾����
			TThostFtdcBrokerIDType	BrokerID;
			///Ͷ���ߴ���
			TThostFtdcInvestorIDType	InvestorID;
			///������������
			TThostFtdcRatioType	OpenRatioByMoney;
			///����������
			TThostFtdcRatioType	OpenRatioByVolume;
			///ƽ����������
			TThostFtdcRatioType	CloseRatioByMoney;
			///ƽ��������
			TThostFtdcRatioType	CloseRatioByVolume;
			///ƽ����������
			TThostFtdcRatioType	CloseTodayRatioByMoney;
			///ƽ��������
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


///��ѯ��󱨵�������Ӧ
void CTDSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pQueryMaxOrderVolume == NULL)
	{
		//cout << "--->>>ָ�����OnRspQryTradingAccount" << endl;			  //ָ����
		//WirteTradeRecordToFileMainThread(0, "OnRspQryTradingAccountָ�����");
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
	///��������
	TThostFtdcTradeDateType	TradeDay;
	///���б���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///�ڻ���˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�ڻ���˾��֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///֤������
	TThostFtdcIdCardTypeType	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���������
	TThostFtdcOpenOrDestroyType	OpenOrDestroy;
	///ǩԼ����
	TThostFtdcTradeDateType	RegDate;
	///��Լ����
	TThostFtdcTradeDateType	OutDate;
	///����ID
	TThostFtdcTIDType	TID;
	///�ͻ�����
	TThostFtdcCustTypeType	CustType;
	///�����ʺ�����
	TThostFtdcBankAccTypeType	BankAccType;
	///���ͻ�����
	TThostFtdcLongIndividualNameType	LongCustomerName;
	*/


	printf("�������ڣ�%s\n", pAccountregister->TradeDay);
	printf("���б��룺%s\n", pAccountregister->BankID);
	printf("���з�֧�������룺%s\n", pAccountregister->BankBranchID);
	printf("�����ʺţ�%s\n", pAccountregister->BankAccount);
	printf("�ڻ���˾���룺%s\n", pAccountregister->BrokerID);
	printf("�ڻ���˾��֧�������룺%s\n", pAccountregister->BrokerBranchID);
	printf("Ͷ�����ʺţ�%s\n", pAccountregister->AccountID);
	printf("֤�����ͣ�%c\n", pAccountregister->IdCardType);
	printf("֤�����룺%s\n", pAccountregister->IdentifiedCardNo);

	printf("�ͻ�������%s\n", pAccountregister->CustomerName);
	printf("���ִ��룺%s\n", pAccountregister->CurrencyID);
	printf("���������%c\n", pAccountregister->OpenOrDestroy);
}






///�ڻ����������ʽ�ת�ڻ�֪ͨ
void CTDSpi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	if (pRspTransfer == NULL)
	{
		return;
	}
}


///�ڻ������ڻ��ʽ�ת����֪ͨ
void CTDSpi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	if (pRspTransfer == NULL)
	{
		return;
	}

}

