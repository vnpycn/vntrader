/*
1.本文件为VNTrader 期货CTP交易库底层代码
2.VNTrader及本C++库开源协议MIT协议

对VNPY开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.vnpy.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的vnpy库里。

官方网站：http://www.vnpy.cn
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
double  VN_EXPORT QryExchangeMarginRate(char *Instrument, int positiontype);    //保证金率
double  VN_EXPORT QryUnderlyingMultiple(char *Instrument);    //乘数
int  VN_EXPORT  ReqFromBankToFutureByFuture(char * BankID,
	char *  BrokerBranchID,
	char *BankAccount,
	char * BankPassWord,
	char * AccountID,
	double  TradeAmount,
	int nRequestID);

///期货发起期货资金转银行请求
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
double VN_EXPORT QryBalance(bool BalanceType);     //1静态权益 0动态权益 自己增加
double VN_EXPORT QryAvailable();   //可用资金
void VN_EXPORT SetShowPosition(bool showstate);

#ifdef __cplusplus
}
#endif