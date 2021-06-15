/*
1.本文件为VNTrader 期货CTP交易库底层代码
2.VNTrader及本C++库开源协议MIT协议

对VNPY开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.vnpy.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的vnpy库里。

官方网站：http://www.vnpy.cn
*/
#pragma once
extern HANDLE hEvent[MAX_EVENTNUM];

extern CThostFtdcTraderApi *mpUserApi;
class CTDSpi: public CThostFtdcTraderSpi
{
public:

	virtual void OnFrontConnected();

	int ReqAuthenticate();

	virtual void OnFrontDisconnected(int nReason); //添加

	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//客户端认证响应 //need

	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//登出请求响应 
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); //need

	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, 
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);  //ADD 2016.12.28

	//合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);//ADD

	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);	//自己增加 

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
 

	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///请求查询合约手续费率响应
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应					
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInstrument2(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///查询最大报单数量响应
	virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询银期签约关系响应
	virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///期货发起银行资金转期货通知
	virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer);

	///期货发起期货资金转银行通知
	virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer);


 public: //更改为public
		///请求查询资金账户
	int ReqQryTradingAccount();
	///请求查询投资者持仓
	void ReqQryInvestorPosition();
 	///请求查询合约(乘数)
	void ReqQryInstrument(char *Instrument);

	///查询最大报单数量请求
	int ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID);
 
	//查询保证金率，新增
	void  ReqQryInvestorProductGroupMargin(char *Instrument);
	void  ReqQryInstrumentMarginRate(char *Instrument);

	///请求查询合约
	int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	///请求查询转帐流水
	//virtual int ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID);

	///请求查询银期签约关系
	//virtual int ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID);

	///请求查询签约银行
	virtual int ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID);

	///期货发起银行资金转期货请求
	int ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID);

	///期货发起期货资金转银行请求
	int ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID);

	HANDLE hSyncObj;
	int iRequestID;
	int iOrderRef;
	bool bInitOK;

	int FRONT_ID;
	int SESSION_ID;

public:
	int InsertOrder(char *InstrumentID, TThostFtdcDirectionType dir,char offsetFlag, char priceType, double price, int num);

	int DeleteOrder(char *InstrumentID, DWORD orderRef);

	void ReqUserLogin();

	int ReqSettlementInfoConfirm();

	CTDSpi();
	~CTDSpi();

	//bool Init();
	bool IsInitOK(){return bInitOK;}
};

