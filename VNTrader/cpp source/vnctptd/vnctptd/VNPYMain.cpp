/*
1.���ļ�ΪVNTrader �ڻ�CTP���׿�ײ����
2.VNTrader����C++�⿪ԴЭ��MITЭ��

��VNPY��Դ���������׵ģ����õ�ԭʼ���߿϶��ģ���������http://www.vnpy.cn��վ�ϣ�
������ڡ���Դ˵���͸�л.txt�����������ļ����ϸ��·���ÿһ���°汾��vnpy���

�ٷ���վ��http://www.vnpy.cn
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



//����
extern std::map<std::string, double> gUnderlyingMultiple;
//��֤����
extern std::map<std::string, double> gMarginRate_long;
extern std::map<std::string, double> gMarginRate_short;
//��������
extern std::map<std::string, double> gCommissionRate;

//��ѯ��󱨵�����
extern std::map<std::string, int> gMaxOrderVolume;




extern int	Trade_dataA_Amount_B_Today[TYPE_NUM];		//�൥�ֲ�
extern int	Trade_dataA_Amount_B_History[TYPE_NUM];		//�൥�ֲ�
extern int	Trade_dataA_Amount_S_Today[TYPE_NUM];		//�յ��ֲ�
extern int	Trade_dataA_Amount_S_History[TYPE_NUM];		//�յ��ֲ�

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
			return -2;//δ����ʽ���������
		}
		num =(int)( rate* TodayAllAmount*multiplier); //��̬Ȩ��
	}
	else
	{
		if (YestayAllAmount < 1e-10)
		{
			return -2;//δ����ʽ���������
		}
		num = (int)(rate* YestayAllAmount*multiplier); //��̬Ȩ��

	}


	printf("�����ֱ���[%0.02f%%]�µ�����[%d]\n", rate,num);
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
	//printf("�µ�����[%d]\n", num);
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

//��ѯ����
double QryUnderlyingMultiple(char *Instrument)
{
	if (!vntdspi)
	{
		return -1; //δ��ʼ�����
	}
	vntdspi->ReqQryInstrument(Instrument);//��λ����		

	int num = 0;
	while (num<30)
	{
 

			if (gUnderlyingMultiple.find(Instrument) != gUnderlyingMultiple.end())
			{
				//printf("����:%0.02f\n\n", gUnderlyingMultiple[Instrument]);
				return gUnderlyingMultiple[Instrument];
				//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

				//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
			}
			else
			{
				//printf("û�ҵ��ú�Լ%s\n", contract);
				return -2;
			}

 

		num++;
		Sleep(200);
	}


	return -3;//��ѯ��ʱ

}


//��ѯ��֤����
double QryExchangeMarginRate(char *Instrument,int type)
{
	if (!vntdspi)
	{
		return -1; //δ��ʼ�����
	}
	vntdspi->ReqQryInstrumentMarginRate(Instrument);//��λ����		

	//vntdspi->ReqQryInvestorProductGroupMargin(Instrument);//��λ����	
	
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
				//printf("û�ҵ��ú�Լ%s\n", contract);
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
				//printf("û�ҵ��ú�Լ%s\n", contract);
				return -2;
			}



		}


		num++;
		Sleep(200);
	}


	return -3;//��ѯ��ʱ
 
 
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
	return vntdspi->ReqQueryMaxOrderVolume(&req,1);//��λ����		
}


int  VN_EXPORT ReqQryContractBank()
{
	CThostFtdcQryContractBankField req;
	memset(&req,0,sizeof(CThostFtdcQryContractBankField));
	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());
	//strncpy_s(tn.BrokerID, sizeof(tn.BrokerID), BrokerID, sizeof(tn.BrokerID));
	return vntdspi->ReqQryContractBank(&req, 1);//��λ����		
}


int  VN_EXPORT  ReqFromBankToFutureByFuture(char * BankID,char *  BrokerBranchID,char *BankAccount,char * BankPassWord,char * AccountID,double  TradeAmount,int nRequestID)
{
	 
	/*
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	TThostFtdcSerialType	PlateSerial;
	///����Ƭ��־
	TThostFtdcLastFragmentType	LastFragment;
	///�Ự��
	TThostFtdcSessionIDType	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	TThostFtdcIdCardTypeType	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	TThostFtdcCustTypeType	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	TThostFtdcInstallIDType	InstallID;
	///�ڻ���˾��ˮ��
	TThostFtdcFutureSerialType	FutureSerial;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	TThostFtdcYesNoIndicatorType	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ת�ʽ��
	TThostFtdcTradeAmountType	TradeAmount;
	///�ڻ���ȡ���
	TThostFtdcTradeAmountType	FutureFetchAmount;
	///����֧����־
	TThostFtdcFeePayFlagType	FeePayFlag;
	///Ӧ�տͻ�����
	TThostFtdcCustFeeType	CustFee;
	///Ӧ���ڻ���˾����
	TThostFtdcFutureFeeType	BrokerFee;
	///���ͷ������շ�����Ϣ
	TThostFtdcAddInfoType	Message;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	TThostFtdcBankAccTypeType	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	TThostFtdcBankAccTypeType	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	TThostFtdcPwdFlagType	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	TThostFtdcPwdFlagType	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	TThostFtdcRequestIDType	RequestID;
	///����ID
	TThostFtdcTIDType	TID;
	///ת�˽���״̬
	TThostFtdcTransferStatusType	TransferStatus;
	///���ͻ�����
	TThostFtdcLongIndividualNameType	LongCustomerName;
	*/
 
	CThostFtdcReqTransferField req;
	memset(&req,0,sizeof(CThostFtdcReqTransferField));

	//req.TradeCode;  
	//req.BankID; //����  
	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());

	//req.BankBranchID; //����  
	if (req.BankBranchID[0] == 0)
	{
		strcpy_s(req.BankBranchID,sizeof(TThostFtdcBankBrchIDType), "0000");
	}
	//req.BrokerID; //����  
	//req.BrokerBranchID; //����  
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
	//req.BankAccount; //����  
	//req.BankPassWord; //����  
	strcpy_s(req.BankAccount,sizeof(TThostFtdcBankAccountType), BankAccount);
	strcpy_s(req.BankPassWord,sizeof(TThostFtdcPasswordType), BankPassWord);
					  //req.AccountID;  
	//req.AccountID; //����  
	//req.Password; //����  
				  //req.InstallID;  
				  //req.FutureSerial;  
	//strcpy(req.UserID, m_UserID);
	strcpy_s(req.AccountID,sizeof(TThostFtdcAccountIDType), gInvestorID.c_str());
	strcpy_s(req.UserID,sizeof(TThostFtdcUserIDType), gInvestorID.c_str());
	strcpy_s(req.Password,sizeof(TThostFtdcPasswordType), gPassword.c_str());


	//req.VerifyCertNoFlag;  
	//req.CurrencyID="CNY"; //����ұ���  
	strncpy_s(req.CurrencyID, sizeof(req.CurrencyID), "CNY", sizeof("CNY"));

	req.TradeAmount= TradeAmount;
	                 //����  
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
 
	//return vntdspi->ReqFromBankToFutureByFuture(&tn,1);//��λ����		
	return vntdspi->ReqFromBankToFutureByFuture(&req,nRequestID); //����ת�ڻ�  
 
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
	//req.BankID; //����  
	strcpy_s(req.BrokerID,sizeof(TThostFtdcBrokerIDType), gBrokerID.c_str());

	//req.BankBranchID; //����  
	if (req.BankBranchID[0] == 0)
	{
		strcpy_s(req.BankBranchID,sizeof(TThostFtdcBankBrchIDType), "0000");
	}
	//req.BrokerID; //����  
	//req.BrokerBranchID; //����  
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
	//req.BankAccount; //����  
	//req.BankPassWord; //����  
	strcpy_s(req.BankAccount,sizeof(TThostFtdcBankAccountType), BankAccount);
	strcpy_s(req.BankPassWord,sizeof(TThostFtdcPasswordType), BankPassWord);
	//req.AccountID;  
	//req.AccountID; //����  
	//req.Password; //����  
	//req.InstallID;  
	//req.FutureSerial;  
	//strcpy(req.UserID, m_UserID);
	strcpy_s(req.AccountID,sizeof(TThostFtdcAccountIDType), gInvestorID.c_str());
	strcpy_s(req.UserID,sizeof(TThostFtdcUserIDType),gInvestorID.c_str());
	strcpy_s(req.Password,sizeof(TThostFtdcPasswordType), gPassword.c_str());


	//req.VerifyCertNoFlag;  
	//req.CurrencyID="CNY"; //����ұ���  
	strncpy_s(req.CurrencyID, sizeof(req.CurrencyID), "CNY", sizeof("CNY"));

	req.TradeAmount = TradeAmount;
	//����  
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


	return  vntdspi->ReqFromFutureToBankByFuture(&req,nRequestID); //�ڻ�ת����  

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
			//printf("%s ��ѯPOSITION_SELL_TODAY��λ%d\n", contract, gPosition_S_Today[contract]);


			return gPosition_S_Today[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			//printf("û�ҵ��ú�Լ%s\n", contract);
			return 0;
		}



	}
	break;
	case POSITION_BUY_TODAY:
	{
		// printf("POSITION_BUY_TODAY[%s][%d]\n", contract, gPosition_B_Today[contract]);

		if (gPosition_B_Today.find(contract) != gPosition_B_Today.end())
		{
			// printf("%s ��ѯPOSITION_BUY_TODAY��λ%d\n", contract, gPosition_B_Today[contract]);
			return gPosition_B_Today[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			//printf("û�ҵ��ú�Լ%s\n", contract);
			return 0;
		}

	}
	break;
	case POSITION_SELL_HISTORY:
	{
		//printf("POSITION_SELL_HISTORY\n");
		if (gPosition_S_History.find(contract) != gPosition_S_History.end())
		{
			// printf("%s ��ѯPOSITION_SELL_HISTORY��λ%d\n", contract, gPosition_S_History[contract]);
			return gPosition_S_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			//printf("û�ҵ��ú�Լ%s\n", contract);
			return 0;
		}

	}
	break;
	case POSITION_BUY_HISTORY:
	{
		// printf("POSITION_BUY_HISTORY\n");
		if (gPosition_B_History.find(contract) != gPosition_B_History.end())
		{
			// printf("%s ��ѯPOSITION_BUY_HISTORY��λ%d\n", contract, gPosition_B_History[contract]);
			return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		else
		{
			//printf("û�ҵ��ú�Լ%s\n", contract);
			return 0;
		}

	}
	break;

	case POSITION_SELL_ALL:
	{  //  printf("POSITION_SELL_ALL\n");
		int allnum = 0;

		if (gPosition_S_History.find(contract) != gPosition_S_History.end())
		{
			// printf("%s A��ѯPOSITION_SELL_ALL��λ%d\n", contract, gPosition_S_History[contract]);

			allnum = gPosition_S_History[contract];
			//return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		// else
		// {
		// printf("û�ҵ��ú�Լ�Ĳ���%s\n", contract);
		//allnum= 0;
		// }


		if (gPosition_S_Today.find(contract) != gPosition_S_Today.end())
		{
			// printf("%s B��ѯPOSITION_SELL_ALL��λ%d\n", contract, gPosition_S_Today[contract]);

			allnum = allnum + gPosition_S_Today[contract];
			//return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		// else
		// {
		// printf("û�ҵ��ú�Լ�Ĳ���%s\n", contract);
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
			// printf("%s A��ѯPOSITION_BUY_ALL��λ%d\n", contract, gPosition_B_History[contract]);

			allnum = gPosition_B_History[contract];
			//return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		//else
		// {
		// printf("û�ҵ��ú�Լ�Ĳ���%s\n", contract);
		//allnum= 0;
		// }


		if (gPosition_B_Today.find(contract) != gPosition_B_Today.end())
		{
			// printf("%s B��ѯPOSITION_BUY_ALL��λ%d\n", contract, gPosition_B_Today[contract]);

			allnum = allnum + gPosition_B_Today[contract];
			//return gPosition_B_History[contract];
			//memcpy(data[gMarket[pDepthMarketData->InstrumentID]],  pDepthMarketData, size);

			//	memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		}
		//else
		// {
		// printf("û�ҵ��ú�Լ�Ĳ���%s\n", contract);
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


	//return  vntdspi->ReqQryInstrument(&req, nRequestID); //�ڻ�ת����  
	return  vntdspi->ReqQryInstrument(&req, 1); //�ڻ�ת����  


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
		return TodayAllAmount; //��̬Ȩ��
	}
	else
	{
		return YestayAllAmount; //��̬Ȩ��
	
	}

 
}

double QryAvailable()
{

	return Available;  //�����ʽ�
 
}
void SetShowPosition(bool showstate)
{
	showpositionstate = showstate;
	//return Available;  //�����ʽ�

}
 

extern DWORD WINAPI PositionThreadProc(void* p);	//��������
extern DWORD WINAPI ReqQryInstrumentMarginRateThreadProc(void* p);	//��������

int InitTD()
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;

	IniFile file;
	if (!file.Init("vnctptd.ini"))
	{
		//MessageBox(NULL, ("��ȡvnctptd.iniʧ��!"), ("������ʾ"), MB_OK | MB_ICONWARNING);
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
		//MessageBox(NULL, ("vnctptd.ini��BrokerID�ֶ�δ����"), ("������ʾ"), MB_OK | MB_ICONWARNING);
		return 1;
	}
	if (gInvestorID == "")
	{
		//MessageBox(NULL, ("vnctptd.ini��InvestorID�ֶ�δ����"), ("������ʾ"), MB_OK | MB_ICONWARNING);
		return 1;
	}
	if (gPassword == "")
	{
		//MessageBox(NULL, ("vnctptd.ini��Password�ֶ�δ����"), ("������ʾ"), MB_OK | MB_ICONWARNING);
		return 1;
	}
	if (gFrontAddr[0] == "" &&  gFrontAddr[1] == "" && gFrontAddr[2] == "")
	{
		//MessageBox(NULL, ("vnctptd.ini��FrontAddr�ֶ�����Ҫ����һ��"), ("������ʾ"), MB_OK | MB_ICONWARNING);
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
	//��ѯ�ֲ��߳�
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
