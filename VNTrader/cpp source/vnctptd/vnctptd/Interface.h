/*
1.���ļ�ΪVNTrader �ڻ�CTP���׿�ײ����
2.VNTrader����C++�⿪ԴЭ��MITЭ��

��VNPY��Դ���������׵ģ����õ�ԭʼ���߿϶��ģ���������http://www.vnpy.cn��վ�ϣ�
������ڡ���Դ˵���͸�л.txt�����������ļ����ϸ��·���ÿһ���°汾��vnpy���

�ٷ���վ��http://www.vnpy.cn
*/
#pragma once

#ifdef PYCTPTRADER_EXPORTS
#	define VN_EXPORT __declspec(dllexport)
#else
#	define VN_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif


	void VN_EXPORT VNRegOnFrontConnected(void(*outputCallback)());

	void VN_EXPORT VNRegOnFrontDisconnected(void(*outputCallback)(int * a));

	void VN_EXPORT VNRegOnRtnDepthMarketData(void(*outputCallback)(const CThostFtdcDepthMarketDataField* a));

	void VN_EXPORT VNRegOnRspUserLogin(void(*outputCallback)(const CThostFtdcRspUserLoginField * a));

	void VN_EXPORT VNRegOnRspUserLogout(void(*outputCallback)(const int* a));

	void VN_EXPORT VNRegOnRspUnSubMarketData(void(*outputCallback)(const int* a));

	void VN_EXPORT VNRegOnRspSubMarketData(void(*outputCallback)(const int* a));

	void VN_EXPORT InitTD();




int VN_EXPORT QryQueryMaxOrderVolume(char *BrokerID, char * InvestorID, char * Instrument, char * Direction, char * OffsetFlag, char * HedgeFlag,int MaxVolume);
int VN_EXPORT Login();
int VN_EXPORT InsertOrderByRate(char *Instrument, char direction, char offsetFlag, char priceType, double price,double rate, bool BalanceType,int multiplier);
int VN_EXPORT InsertOrder(char *Instrument,char direction, char offsetFlag, char priceType, double price, int num);
int VN_EXPORT DeleteOrder(char *Instrument, int OrderRef);
int VN_EXPORT QryTradedVol(int OrderRef);
int VN_EXPORT QryPosition(char *Instrument,int positiontype);   
int VN_EXPORT ReqQueryMaxOrderVolume(char * BrokerID, char * InvestorID, char * InstrumentID, char Direction, char  OffsetFlag, char  HedgeFlag, int MaxVolume);
int  VN_EXPORT ReqQryContractBank();
double  VN_EXPORT QryExchangeMarginRate(char *Instrument, int positiontype);    //��֤����
double  VN_EXPORT QryUnderlyingMultiple(char *Instrument);    //����
int  VN_EXPORT  ReqFromBankToFutureByFuture(char * BankID,
	char *  BrokerBranchID,
	char *BankAccount,
	char * BankPassWord,
	char * AccountID,
	double  TradeAmount,
	int nRequestID);

///�ڻ������ڻ��ʽ�ת��������
int  VN_EXPORT  ReqFromFutureToBankByFuture(
	char * BankID,
	char *  BrokerBranchID,
	char *BankAccount,
	char * BankPassWord,
	char * AccountID,
	double  TradeAmount,
	int nRequestID);
int VN_EXPORT ReqQryInstrument();
void VN_EXPORT *QryPositionList(int i);
double VN_EXPORT QryBalance(bool BalanceType);     //1��̬Ȩ�� 0��̬Ȩ�� �Լ�����
double VN_EXPORT QryAvailable();   //�����ʽ�
void VN_EXPORT SetShowPosition(bool showstate);

#ifdef __cplusplus
}
#endif