/*
1.本文件为VNTrader 期货CTP交易库底层代码
2.VNTrader及本C++库开源协议MIT协议

对VNPY开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.vnpy.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的vnpy库里。

官方网站：http://www.vnpy.cn
*/
#include "stdafx.h"
#include "IniFile.h"
#include "Interface.h"
#include "TDSpi.h"
using namespace std;

std::string gFrontAddr[3];
std::string gBrokerID;
std::string gInvestorID;
std::string gPassword;
std::string gAppID;
std::string gAuthCode;
std::string gUserProductInfo;


HANDLE ghTradedVolMutex = NULL;
std::map<int, int> gOrderRef2TradedVol;

bool showpositionstate=false;
extern CTDSpi *vntdspi;


HANDLE hEvent[MAX_EVENTNUM] =
{
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL

};

void Start()
{	 
	for (int i = 0; i < MAX_EVENTNUM; i++)
	{
		if (!hEvent[i])
		{
			char temp[10] = { 0 };
			_snprintf_s(temp, sizeof(temp), sizeof(temp), "hEvent%d", i);
			hEvent[i] = CreateEvent(NULL, FALSE, FALSE, temp);
		}
	}
	//InitializeCriticalSection(&g_csdata);
	ghTradedVolMutex = ::CreateMutex(NULL, FALSE, NULL);
}

void End()
{
	//DeleteCriticalSection(&g_csdata);

	if (ghTradedVolMutex)
	{
		::CloseHandle(ghTradedVolMutex);
		ghTradedVolMutex = NULL;
	}
}

int ReqUserLogin()
{
	return vntdspi->ReqUserLogin();
}



//乘数
extern std::map<std::string, double> gUnderlyingMultiple;
//保证金率
extern std::map<std::string, double> gMarginRate_long;
extern std::map<std::string, double> gMarginRate_short;
//手续费率
extern std::map<std::string, double> gCommissionRate;

//查询最大报单数量
extern std::map<std::string, int> gMaxOrderVolume;




extern int	Trade_dataA_Amount_B_Today[TYPE_NUM];		//多单持仓
extern int	Trade_dataA_Amount_B_History[TYPE_NUM];		//多单持仓
extern int	Trade_dataA_Amount_S_Today[TYPE_NUM];		//空单持仓
extern int	Trade_dataA_Amount_S_History[TYPE_NUM];		//空单持仓

extern double YestayAllAmount;
extern double TodayAllAmount;
extern double Available;

extern std::map<std::string, int> gPosition_S;
extern std::map<std::string, int> gPosition_B;

extern std::map<std::string, int> gPosition_S_Today;
extern std::map<std::string, int> gPosition_B_Today;
extern std::map<std::string, int> gPosition_S_History;
extern std::map<std::string, int> gPosition_B_History;

extern std::map<std::string, int> gTypeCheckState_S_Today;
extern std::map<std::string, int> gTypeCheckState_B_Today;
extern std::map<std::string, int> gTypeCheckState_S_History;
extern std::map<std::string, int> gTypeCheckState_B_History;

#define POSITION_SELL_TODAY     9001
#define POSITION_BUY_TODAY      9002
#define POSITION_SELL_HISTORY   9003
#define POSITION_BUY_HISTORY    9004
#define POSITION_SELL_ALL   9005
#define POSITION_BUY_ALL    9006


#define RATETYPE_LONG     0
#define RATETYPE_SHORT    1


int VN_EXPORT QryQueryMaxOrderVolume(char *BrokerID, char * InvestorID, char * Instrument, char * Direction, char * OffsetFlag, char * HedgeFlag, int MaxVolume)
{

	return 1;
}
int InsertOrderByRate(char *Instrument, char direction, char offsetFlag, char priceType, double price, double rate, bool BalanceType,int multiplier)
{
	//std::cout << __FUNCTION__ << Instrument << "\t" << direction << "\t"
	//<< offsetFlag << "\t" << priceType << "\t" << price << "\t"

	int num = 0;// rate*

	if (BalanceType==0)
	{
		if (TodayAllAmount < 1e-10)
		{
			return -2;//未获得资金容量数据
		}
		num =(int)( rate* TodayAllAmount*multiplier); //动态权益
	}
	else
	{
		if (YestayAllAmount < 1e-10)
		{
			return -2;//未获得资金容量数据
		}
		num = (int)(rate* YestayAllAmount*multiplier); //静态权益

	}


	printf("按开仓比例[%0.02f%%]下单手数[%d]\n", rate,num);
	//2021
	if (vntdspi->IsInitOK())
	{
		return vntdspi->InsertOrder(Instrument, direction, offsetFlag, priceType, price, num);
	}
	else
	{
		return -1;
	}
}


int InsertOrder(char *Instrument, char direction, char offsetFlag, char priceType, double price, int num)
{
	//std::cout << __FUNCTION__ << Instrument << "\t" << direction << "\t"
		//<< offsetFlag << "\t" << priceType << "\t" << price << "\t"
		//<< num << std::endl;
	//printf("下单手数[%d]\n", num);
	//2021
	if (vntdspi->IsInitOK())
	{
		return vntdspi->InsertOrder(Instrument, direction, offsetFlag, priceType, price, num);
	}
	else
	{
		return -1;
	}
}
 
int DeleteOrder(char *Instrument, int OrderRef)
{
	//std::cout << __FUNCTION__ << "\t" << OrderRef << std::endl;
	//2021
	if (vntdspi->IsInitOK())
	{
		return vntdspi->DeleteOrder(Instrument, OrderRef);
	}
	else
	{
		return -1;
	}
}

int QryTradedVol(int OrderRef)
{
	int ret = -1;
	::WaitForSingleObject(ghTradedVolMutex, INFINITE);
	if (gOrderRef2TradedVol.find(OrderRef) != gOrderRef2TradedVol.end())
	{
		ret = gOrderRef2TradedVol[OrderRef];
	}
	::ReleaseMutex(ghTradedVolMutex);
	return ret;
}

//查询乘数
double QryUnderlyingMultiple(char *Instrument)
{
	if (!vntdspi)
	{
		return -1; //未初始化完成
	}
	vntdspi->ReqQryInstrument(Instrument);//仓位管理		

	int num = 0;
	while (num<30)
	{
 

			if (gUnderlyingMultiple.find(Instrument) != gUnderlyingMultiple.end())
			{
				//printf("乘数:%0.02f\n\n", gUnderlyingMultiple[Instrument]);
				return gUnderlyingMultiple[Instrument];
				//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

				//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
			}
			else
			{
				//printf("没找到该合约%s\n", contract);
				return -2;
			}

 

		num++;
		Sleep(200);
	}


	return -3;//查询超时

}


//查询保证金率
double QryExchangeMarginRate(char *Instrument,int type)
{
	if (!vntdspi)
	{
		return -1; //未初始化完成
	}
	vntdspi->ReqQryInstrumentMarginRate(Instrument);//仓位管理		

	//vntdspi->ReqQryInvestorProductGroupMargin(Instrument);//仓位管理	
	
	int num = 0;
	while (num<30)
	{
		if (type == RATETYPE_LONG)
		{

			if (gMarginRate_long.find(Instrument) != gMarginRate_long.end())
			{

                return gMarginRate_long[Instrument];
				//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

				//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
			}
			else
			{
				//printf("没找到该合约%s\n", contract);
				return -2;
			}

			
		}

		else
		{
			if (gMarginRate_short.find(Instrument) != gMarginRate_short.end())
			{


			    return gMarginRate_short[Instrument];

				//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

				//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
			}
			else
			{
				//printf("没找到该合约%s\n", contract);
				return -2;
			}



		}


		num++;
		Sleep(200);
	}


	return -3;//查询超时
 
 
}


int  VN_EXPORT ReqQueryMaxOrderVolume(char * BrokerID,char * InvestorID,char * InstrumentID, char Direction, char  OffsetFlag,char  HedgeFlag, int MaxVolume)
{
	CThostFtdcQueryMaxOrderVolumeField req;
	memset(&req,0,sizeof(CThostFtdcQueryMaxOrderVolumeField));
	strncpy_s(req.BrokerID, sizeof(req.BrokerID), BrokerID, sizeof(req.BrokerID));
	strncpy_s(req.InvestorID, sizeof(req.InvestorID), InvestorID, sizeof(req.InvestorID));
	strncpy_s(req.InstrumentID, sizeof(req.InstrumentID), InstrumentID, sizeof(req.InstrumentID));
	req.Direction  = Direction ;
	req.OffsetFlag = OffsetFlag;
	req.HedgeFlag = HedgeFlag;
	req.MaxVolume = MaxVolume;
	return vntdspi->ReqQueryMaxOrderVolume(&req,1);//仓位管理		
}


int  VN_EXPORT ReqQryContractBank()
{
	CThostFtdcQryContractBankField req;
	memset(&req,0,sizeof(CThostFtdcQryContractBankField));
	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	//strncpy_s(tn.BrokerID, sizeof(tn.BrokerID), BrokerID, sizeof(tn.BrokerID));
	return vntdspi->ReqQryContractBank(&req, 1);//仓位管理		
}


int  VN_EXPORT  ReqFromBankToFutureByFuture(char * BankID,char *  BrokerBranchID,char *BankAccount,char * BankPassWord,char * AccountID,double  TradeAmount,int nRequestID)
{
	 
	/*
	///业务功能码
	TThostFtdcTradeCodeType	TradeCode;
	///银行代码
	TThostFtdcBankIDType	BankID;
	///银行分支机构代码
	TThostFtdcBankBrchIDType	BankBranchID;
	///期商代码
	TThostFtdcBrokerIDType	BrokerID;
	///期商分支机构代码
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///交易日期
	TThostFtdcTradeDateType	TradeDate;
	///交易时间
	TThostFtdcTradeTimeType	TradeTime;
	///银行流水号
	TThostFtdcBankSerialType	BankSerial;
	///交易系统日期 
	TThostFtdcTradeDateType	TradingDay;
	///银期平台消息流水号
	TThostFtdcSerialType	PlateSerial;
	///最后分片标志
	TThostFtdcLastFragmentType	LastFragment;
	///会话号
	TThostFtdcSessionIDType	SessionID;
	///客户姓名
	TThostFtdcIndividualNameType	CustomerName;
	///证件类型
	TThostFtdcIdCardTypeType	IdCardType;
	///证件号码
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///客户类型
	TThostFtdcCustTypeType	CustType;
	///银行帐号
	TThostFtdcBankAccountType	BankAccount;
	///银行密码
	TThostFtdcPasswordType	BankPassWord;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///期货密码
	TThostFtdcPasswordType	Password;
	///安装编号
	TThostFtdcInstallIDType	InstallID;
	///期货公司流水号
	TThostFtdcFutureSerialType	FutureSerial;
	///用户标识
	TThostFtdcUserIDType	UserID;
	///验证客户证件号码标志
	TThostFtdcYesNoIndicatorType	VerifyCertNoFlag;
	///币种代码
	TThostFtdcCurrencyIDType	CurrencyID;
	///转帐金额
	TThostFtdcTradeAmountType	TradeAmount;
	///期货可取金额
	TThostFtdcTradeAmountType	FutureFetchAmount;
	///费用支付标志
	TThostFtdcFeePayFlagType	FeePayFlag;
	///应收客户费用
	TThostFtdcCustFeeType	CustFee;
	///应收期货公司费用
	TThostFtdcFutureFeeType	BrokerFee;
	///发送方给接收方的消息
	TThostFtdcAddInfoType	Message;
	///摘要
	TThostFtdcDigestType	Digest;
	///银行帐号类型
	TThostFtdcBankAccTypeType	BankAccType;
	///渠道标志
	TThostFtdcDeviceIDType	DeviceID;
	///期货单位帐号类型
	TThostFtdcBankAccTypeType	BankSecuAccType;
	///期货公司银行编码
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///期货单位帐号
	TThostFtdcBankAccountType	BankSecuAcc;
	///银行密码标志
	TThostFtdcPwdFlagType	BankPwdFlag;
	///期货资金密码核对标志
	TThostFtdcPwdFlagType	SecuPwdFlag;
	///交易柜员
	TThostFtdcOperNoType	OperNo;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///交易ID
	TThostFtdcTIDType	TID;
	///转账交易状态
	TThostFtdcTransferStatusType	TransferStatus;
	///长客户姓名
	TThostFtdcLongIndividualNameType	LongCustomerName;
	*/
 
	CThostFtdcReqTransferField req;
	memset(&req,0,sizeof(CThostFtdcReqTransferField));

	//req.TradeCode;  
	//req.BankID; //必填  
	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());

	//req.BankBranchID; //必填  
	if (req.BankBranchID[0] == 0)
	{
		strcpy_s(req.BankBranchID,sizeof(TThostFtdcBankBrchIDType), "0000");
	}
	//req.BrokerID; //必填  
	//req.BrokerBranchID; //必填  
	//if (req.BrokerBranchID[0] == 0)
	//{
	//	strcpy(req.BrokerBranchID, "0000");
	//}
	//req.TradeDate;  
	//req.TradeTime;  
	//req.BankSerial;  
	//req.TradingDay;  
	//req.PlateSerial;  
	//req.LastFragment;  
	//req.SessionID;  
	//req.CustomerName;  
	//req.IdCardType;  
	//req.IdentifiedCardNo;  
	//req.CustType;  
	//req.BankAccount; //必填  
	//req.BankPassWord; //必填  
	strcpy_s(req.BankAccount,sizeof(TThostFtdcBankAccountType), BankAccount);
	strcpy_s(req.BankPassWord,sizeof(TThostFtdcPasswordType), BankPassWord);
					  //req.AccountID;  
	//req.AccountID; //必填  
	//req.Password; //必填  
				  //req.InstallID;  
				  //req.FutureSerial;  
	//strcpy(req.UserID, m_UserID);
	strcpy_s(req.AccountID,sizeof(TThostFtdcAccountIDType), gInvestorID.c_str());
	strcpy_s(req.UserID,sizeof(TThostFtdcUserIDType), gInvestorID.c_str());
	strcpy_s(req.Password,sizeof(TThostFtdcPasswordType), gPassword.c_str());


	//req.VerifyCertNoFlag;  
	//req.CurrencyID="CNY"; //人民币必填  
	strncpy_s(req.CurrencyID, sizeof(req.CurrencyID), "CNY", sizeof("CNY"));

	req.TradeAmount= TradeAmount;
	                 //必填  
					 //req.FutureFetchAmount;  
					 //req.FeePayFlag;  
					 //req.CustFee;  
					 //req.BrokerFee;  
					 //req.Message;  
					 //req.Digest;  
					 //req.BankAccType;  
					 //req.DeviceID;  
					 //req.BankSecuAccType;  
					 //req.BrokerIDByBank;  
					 //req.BankSecuAcc;  
					 //req.BankPwdFlag;  
	req.SecuPwdFlag =  THOST_FTDC_BPWDF_BlankCheck;
	//req.OperNo;  
	//req.RequestID = reqInfo.nRequestID;
	req.RequestID =  nRequestID;
	//req.TID;  
	//req.TransferStatus;  
 
	//return vntdspi->ReqFromBankToFutureByFuture(&tn,1);//仓位管理		
	return vntdspi->ReqFromBankToFutureByFuture(&req,nRequestID); //银行转期货  
 
}
int  VN_EXPORT  ReqFromFutureToBankByFuture(
	char * BankID,
	//char * BankBranchID,
	//char * BrokerID,
	char *  BrokerBranchID,
	char *BankAccount,
	char * BankPassWord,
	char * AccountID,
	//char *Password,
	//char * CurrencyID,
	double  TradeAmount,
	int nRequestID)
{
 
	CThostFtdcReqTransferField req;
	memset(&req, 0, sizeof(CThostFtdcReqTransferField));

	//req.TradeCode;  
	//req.BankID; //必填  
	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());

	//req.BankBranchID; //必填  
	if (req.BankBranchID[0] == 0)
	{
		strcpy_s(req.BankBranchID,sizeof(TThostFtdcBankBrchIDType), "0000");
	}
	//req.BrokerID; //必填  
	//req.BrokerBranchID; //必填  
	//if (req.BrokerBranchID[0] == 0)
	//{
	//	strcpy(req.BrokerBranchID, "0000");
	//}
	//req.TradeDate;  
	//req.TradeTime;  
	//req.BankSerial;  
	//req.TradingDay;  
	//req.PlateSerial;  
	//req.LastFragment;  
	//req.SessionID;  
	//req.CustomerName;  
	//req.IdCardType;  
	//req.IdentifiedCardNo;  
	//req.CustType;  
	//req.BankAccount; //必填  
	//req.BankPassWord; //必填  
	strcpy_s(req.BankAccount,sizeof(TThostFtdcBankAccountType), BankAccount);
	strcpy_s(req.BankPassWord,sizeof(TThostFtdcPasswordType), BankPassWord);
	//req.AccountID;  
	//req.AccountID; //必填  
	//req.Password; //必填  
	//req.InstallID;  
	//req.FutureSerial;  
	//strcpy(req.UserID, m_UserID);
	strcpy_s(req.AccountID,sizeof(TThostFtdcAccountIDType), gInvestorID.c_str());
	strcpy_s(req.UserID,sizeof(TThostFtdcUserIDType),gInvestorID.c_str());
	strcpy_s(req.Password,sizeof(TThostFtdcPasswordType), gPassword.c_str());


	//req.VerifyCertNoFlag;  
	//req.CurrencyID="CNY"; //人民币必填  
	strncpy_s(req.CurrencyID, sizeof(req.CurrencyID), "CNY", sizeof("CNY"));

	req.TradeAmount = TradeAmount;
	//必填  
	//req.FutureFetchAmount;  
	//req.FeePayFlag;  
	//req.CustFee;  
	//req.BrokerFee;  
	//req.Message;  
	//req.Digest;  
	//req.BankAccType;  
	//req.DeviceID;  
	//req.BankSecuAccType;  
	//req.BrokerIDByBank;  
	//req.BankSecuAcc;  
	//req.BankPwdFlag;  
	req.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;
	//req.OperNo;  
	//req.RequestID = reqInfo.nRequestID;
	req.RequestID = nRequestID;
	//req.TID;  
	//req.TransferStatus;  


	return  vntdspi->ReqFromFutureToBankByFuture(&req,nRequestID); //期货转银行  

}




int QryPosition(char *contract, int positiontype)
{

	//gPosition_S_Today::iterator it = gPosition_S_Today.find(contract);
	switch (positiontype)
	{
	case POSITION_SELL_TODAY:
	{
		// printf("POSITION_SELL_TODAY[%s][%d]\n", contract, gPosition_S_Today[contract]);
		if (gPosition_S_Today.find(contract) != gPosition_S_Today.end())
		{
			//printf("%s 查询POSITION_SELL_TODAY仓位%d\n", contract, gPosition_S_Today[contract]);


			return gPosition_S_Today[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			//printf("没找到该合约%s\n", contract);
			return 0;
		}



	}
	break;
	case POSITION_BUY_TODAY:
	{
		// printf("POSITION_BUY_TODAY[%s][%d]\n", contract, gPosition_B_Today[contract]);

		if (gPosition_B_Today.find(contract) != gPosition_B_Today.end())
		{
			// printf("%s 查询POSITION_BUY_TODAY仓位%d\n", contract, gPosition_B_Today[contract]);
			return gPosition_B_Today[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			//printf("没找到该合约%s\n", contract);
			return 0;
		}

	}
	break;
	case POSITION_SELL_HISTORY:
	{
		//printf("POSITION_SELL_HISTORY\n");
		if (gPosition_S_History.find(contract) != gPosition_S_History.end())
		{
			// printf("%s 查询POSITION_SELL_HISTORY仓位%d\n", contract, gPosition_S_History[contract]);
			return gPosition_S_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			//printf("没找到该合约%s\n", contract);
			return 0;
		}

	}
	break;
	case POSITION_BUY_HISTORY:
	{
		// printf("POSITION_BUY_HISTORY\n");
		if (gPosition_B_History.find(contract) != gPosition_B_History.end())
		{
			// printf("%s 查询POSITION_BUY_HISTORY仓位%d\n", contract, gPosition_B_History[contract]);
			return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			//printf("没找到该合约%s\n", contract);
			return 0;
		}

	}
	break;

	case POSITION_SELL_ALL:
	{  //  printf("POSITION_SELL_ALL\n");
		int allnum = 0;

		if (gPosition_S_History.find(contract) != gPosition_S_History.end())
		{
			// printf("%s A查询POSITION_SELL_ALL仓位%d\n", contract, gPosition_S_History[contract]);

			allnum = gPosition_S_History[contract];
			//return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		// else
		// {
		// printf("没找到该合约的策略%s\n", contract);
		//allnum= 0;
		// }


		if (gPosition_S_Today.find(contract) != gPosition_S_Today.end())
		{
			// printf("%s B查询POSITION_SELL_ALL仓位%d\n", contract, gPosition_S_Today[contract]);

			allnum = allnum + gPosition_S_Today[contract];
			//return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		// else
		// {
		// printf("没找到该合约的策略%s\n", contract);
		// allnum = 0;
		// }

		return allnum;
	}
	break;
	case POSITION_BUY_ALL:
	{   //printf("POSITION_BUY_ALL\n");
		int allnum = 0;

		if (gPosition_B_History.find(contract) != gPosition_B_History.end())
		{
			// printf("%s A查询POSITION_BUY_ALL仓位%d\n", contract, gPosition_B_History[contract]);

			allnum = gPosition_B_History[contract];
			//return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		//else
		// {
		// printf("没找到该合约的策略%s\n", contract);
		//allnum= 0;
		// }


		if (gPosition_B_Today.find(contract) != gPosition_B_Today.end())
		{
			// printf("%s B查询POSITION_BUY_ALL仓位%d\n", contract, gPosition_B_Today[contract]);

			allnum = allnum + gPosition_B_Today[contract];
			//return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		//else
		// {
		// printf("没找到该合约的策略%s\n", contract);
		// // allnum = 0;
		// }
		return allnum;

	}
	break;
	default:
		printf("err get\n");
		return 0;
		break;
	}
 
}


int  ReqQryInstrument()
{

	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(CThostFtdcQryInstrumentField));

	//strcpy(req.BrokerID, gBrokerID.c_str());

	//strncpy_s(req.CurrencyID, sizeof(req.CurrencyID), "CNY", sizeof("CNY"));

	//req.TradeAmount = TradeAmount;

	//req.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;
	//req.OperNo;  
	//req.RequestID = reqInfo.nRequestID;
	//r//eq.RequestID = nRequestID;


	//return  vntdspi->ReqQryInstrument(&req, nRequestID); //期货转银行  
	return  vntdspi->ReqQryInstrument(&req, 1); //期货转银行  


}



void *QryPositionList(int i)
{
	/*
	if (gStatus)
	{
		return NULL;
	}
	if (i < 0 || (i >= (int)gMarket.size()))
	{
		return NULL;
	}
	else
	{
		//return data[i];
		return depthdata[i];
	}
	*/
	return NULL;
}


double QryBalance(bool BalanceType)
{
	if (BalanceType)
	{
		return TodayAllAmount; //动态权益
	}
	else
	{
		return YestayAllAmount; //静态权益
	
	}

 
}

double QryAvailable()
{

	return Available;  //可用资金
 
}
void SetShowPosition(bool showstate)
{
	showpositionstate = showstate;
	//return Available;  //可用资金

}
 

extern DWORD WINAPI PositionThreadProc(void* p);	//更新排名
extern DWORD WINAPI ReqQryInstrumentMarginRateThreadProc(void* p);	//更新排名

int InitTD()
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;

	IniFile file;
	if (!file.Init("vnctptd.ini"))
	{
		//MessageBox(NULL, ("读取vnctptd.ini失败!"), ("错误提示"), MB_OK | MB_ICONWARNING);
		return 1;
	}

	gBrokerID = file.GetValueFromSection("setting", "brokeid");
	gInvestorID = file.GetValueFromSection("setting", "investor");
	gPassword = file.GetValueFromSection("setting", "password");
	gAppID = file.GetValueFromSection("setting", "appid");
	gAuthCode = file.GetValueFromSection("setting", "authcode");
	gUserProductInfo = file.GetValueFromSection("setting", "userproductinfo");
	gFrontAddr[0] = file.GetValueFromSection("setting", "address1");
	gFrontAddr[1] = file.GetValueFromSection("setting", "address2");
	gFrontAddr[2] = file.GetValueFromSection("setting", "address3");


	if (gBrokerID == "")
	{
		//MessageBox(NULL, ("vnctptd.ini中BrokerID字段未设置"), ("错误提示"), MB_OK | MB_ICONWARNING);
		return 1;
	}
	if (gInvestorID == "")
	{
		//MessageBox(NULL, ("vnctptd.ini中InvestorID字段未设置"), ("错误提示"), MB_OK | MB_ICONWARNING);
		return 1;
	}
	if (gPassword == "")
	{
		//MessageBox(NULL, ("vnctptd.ini中Password字段未设置"), ("错误提示"), MB_OK | MB_ICONWARNING);
		return 1;
	}
	if (gFrontAddr[0] == "" &&  gFrontAddr[1] == "" && gFrontAddr[2] == "")
	{
		//MessageBox(NULL, ("vnctptd.ini中FrontAddr字段至少要设置一个"), ("错误提示"), MB_OK | MB_ICONWARNING);
		return 1;
	}

	char dir[256] = { 0 };
	::GetCurrentDirectory(255, dir);
	std::string tempDir = std::string(dir).append("\\TdTemp\\");
	::CreateDirectory(tempDir.c_str(), NULL);
	vntdapi = CThostFtdcTraderApi::CreateFtdcTraderApi(tempDir.c_str());
	vntdapi->RegisterSpi(vntdspi);

	vntdapi->SubscribePublicTopic(THOST_TERT_QUICK);
	vntdapi->SubscribePrivateTopic(THOST_TERT_QUICK);

	vntdapi->RegisterFront((char *)gFrontAddr[0].c_str());
	vntdapi->RegisterFront((char *)gFrontAddr[1].c_str());
	vntdapi->RegisterFront((char *)gFrontAddr[2].c_str());

	cerr << "--->>> " << (char *)gFrontAddr[0].c_str() << std::endl;
	cerr << "--->>> " << (char *)gFrontAddr[1].c_str() << std::endl;
	cerr << "--->>> " << (char *)gFrontAddr[2].c_str() << std::endl;

	vntdapi->Init();
	//查询持仓线程
	HANDLE hThread3 = ::CreateThread(NULL, 0,  PositionThreadProc, NULL, 0, NULL);
	//HANDLE hThread4 = ::CreateThread(NULL, 0,  ReqQryInstrumentMarginRateThreadProc, NULL, 0, NULL);
	return 0;
}






void   VNRegOnRspUserLogin(void(*outputCallback)(const CThostFtdcRspUserLoginField* a))
{
	hEvent[EID_OnRspUserLogin] = CreateEvent(NULL, FALSE, FALSE, "EID_OnRspUserLogin");
	ResetEvent(hEvent[EID_OnRspUserLogin]);
	while (1)
	{
		ResetEvent(hEvent[EID_OnRspUserLogin]);
		WaitForSingleObject(hEvent[EID_OnRspUserLogin], INFINITE);

		CThostFtdcRspUserLoginField   a;
		memset(&a, 0, sizeof(CThostFtdcRspUserLoginField));
		outputCallback(&a);
		/*
		time_t t = time(0);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A %j  %z", localtime(&t));
		puts(tmp);
		*/
	}
}
void   VNRegOnRspUserLogout(void(*outputCallback)(const int* a))
{
	hEvent[EID_OnRspUserLogout] = CreateEvent(NULL, FALSE, FALSE, "EID_OnRspUserLogout");
	ResetEvent(hEvent[EID_OnRspUserLogout]);
	while (1)
	{
		ResetEvent(hEvent[EID_OnRspUserLogout]);
		WaitForSingleObject(hEvent[EID_OnRspUserLogout], INFINITE);

		int a = 1;
		outputCallback(&a);
		/*
		time_t t = time(0);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A %j  %z", localtime(&t));
		puts(tmp);
		*/
	}
}





void   VNRegOnRspQryTradingAccount(void(*outputCallback)(const int* a))
{
	hEvent[EID_OnRspQryTradingAccount] = CreateEvent(NULL, FALSE, FALSE, "EID_OnRspQryTradingAccount");
	ResetEvent(hEvent[EID_OnRspQryTradingAccount]);
	while (1)
	{
		ResetEvent(hEvent[EID_OnRspQryTradingAccount]);
		WaitForSingleObject(hEvent[EID_OnRspQryTradingAccount], INFINITE);

		int a = 1;
		outputCallback(&a);
	}
}

void   VNRegOnRspQryInvestorPosition(void(*outputCallback)(const int* a))
{
	hEvent[EID_OnRspQryInvestorPosition] = CreateEvent(NULL, FALSE, FALSE, "EID_OnRspQryInvestorPosition");
	ResetEvent(hEvent[EID_OnRspQryInvestorPosition]);
	while (1)
	{
		ResetEvent(hEvent[EID_OnRspQryInvestorPosition]);
		WaitForSingleObject(hEvent[EID_OnRspQryInvestorPosition], INFINITE);

		int a = 1;
		outputCallback(&a);
	}
}

void  VNRegOnFrontConnected(void(*outputCallback)())
{
	hEvent[EID_OnFrontConnected] = CreateEvent(NULL, FALSE, FALSE, "EID_OnFrontConnected");
	//thread t(test);

	while (1)
	{
		ResetEvent(hEvent[EID_OnFrontConnected]);
		WaitForSingleObject(hEvent[EID_OnFrontConnected], INFINITE);
		outputCallback();
		/*
		time_t t = time(0);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A %j  %z", localtime(&t));
		puts(tmp);
		*/
		Sleep(1);
	}

}


void   VNRegOnFrontDisconnected(void(*outputCallback)(int *a))
{
	hEvent[EID_OnFrontDisconnected] = CreateEvent(NULL, FALSE, FALSE, "EID_OnFrontDisconnected");
	//thread t(test);
	while (1)
	{
		ResetEvent(hEvent[EID_OnFrontDisconnected]);
		WaitForSingleObject(hEvent[EID_OnFrontDisconnected], INFINITE);
		int a = 1;
		outputCallback(&a);
		/*
		time_t t = time(0);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A %j  %z", localtime(&t));
		puts(tmp);
		*/
	}
}
