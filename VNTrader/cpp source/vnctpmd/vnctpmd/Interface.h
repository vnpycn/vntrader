/*
1.���ļ�ΪVNTrader �ڻ�CTP���׿�ײ����
2.VNTrader����C++�⿪ԴЭ��MITЭ��

��VNPY��Դ���������׵ģ����õ�ԭʼ���߿϶��ģ���������http://www.vnpy.cn��վ�ϣ�
������ڡ���Դ˵���͸�л.txt�����������ļ����ϸ��·���ÿһ���°汾��vnpy���

�ٷ���վ��http://www.vnpy.cn
*/
#pragma once

#ifdef PYCTPMARKET_EXPORTS
#define VN_EXPORT __declspec(dllexport)
#else
#define VN_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"{
#endif 

	struct CAHR31
	{
		char value[31];
	};

	struct VN_LoginInfo
	{
		char Brokeid[9];
		char Investor[31];
		char Password[31];
		char AppID[31];
		char AuthCode[31];
		char ProductInfo[31];
		char Adder1[50];
		char Adder2[50];
		char Adder3[50];
 

	};



	void VN_EXPORT VNRegOnFrontConnected(void(*outputCallback)());

	void VN_EXPORT VNRegOnFrontDisconnected(void(*outputCallback)(int a));

	void VN_EXPORT VNRegOnRspUserLogin(void(*outputCallback)(const CThostFtdcRspUserLoginField * a));

	void VN_EXPORT VNRegOnRspUserLogout(void(*outputCallback)(const CThostFtdcUserLogoutField * a));

	void VN_EXPORT VNRegOnRtnDepthMarketData(void(*outputCallback)(const CThostFtdcDepthMarketDataField* a));

	void VN_EXPORT VNRegOnRspUnSubMarketData(void(*outputCallback)(const int* a));

	void VN_EXPORT VNRegOnRspSubMarketData(void(*outputCallback)(const int* a));

	// void VN_EXPORT InitMD(char *Brokeid, char *Investor, char * Password,char * AppID,char *AuthCode,char * ProductInfo, char * Adder1, char * Adder2, char * Adder3);

	int VN_EXPORT InitMD();


	VN_EXPORT char *  GetApiVersion();
	VN_EXPORT char *  GetTradingDay();
	void VN_EXPORT RegisterFront(char *pszFrontAddress);
	void VN_EXPORT RegisterNameServer(char *pszNsAddress);

	/*

	 ///��ʼ��
	 ///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	 void VN_EXPORT Init();

	///����ѯ�ۡ�
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	int VN_EXPORT SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount);

	///�˶�ѯ�ۡ�
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	int VN_EXPORT UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount);

	///ע�����ַ������û���Ϣ
	///@param pFensUserInfo���û���Ϣ��
	void VN_EXPORT RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo);

	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	 void VN_EXPORT Release();

	 ///ע��ص��ӿ�
	 ///@param pSpi �����Իص��ӿ����ʵ��
	 void VN_EXPORT RegisterSpi(CThostFtdcMdSpi *pSpi);
	 */
	 /////////////////////////////////////////
	//�û���¼����
	int VN_EXPORT ReqUserLogin();
	//�û��ǳ�����
	int VN_EXPORT ReqUserLogout();
	//ȡ������
	int VN_EXPORT UnSubscribeMarketData( char *InstrumentID);

	int VN_EXPORT SubscribeMarketData(char *InstrumentID);

    //ѯ��
	void VN_EXPORT SubscribeForQuoteRsp(char *InstrumentID);

	void VN_EXPORT SetPrintState(bool printfstate);

	void VN_EXPORT SetRejectdataTime(double  begintime1,double endtime1,double begintime2,double endtime2, double begintime3, double endtime3, double begintime4, double endtime4);

	void VN_EXPORT TestArr(char** pIpAddList);

	void VN_EXPORT Log(const char * filename, const char * content);

	bool VN_EXPORT ReadInstrument();

	//void  VN_EXPORT strategy1();
	//void VN_EXPORT TestArr(int &numbers, int a);
	//void VN_EXPORT SaveTick(int index);
    //VN_EXPORT char * Read_Ini(const char * filename, const char * option, const char * key);
	//char  VN_EXPORT Read_Ini(const char * filename, const char * option, const char * key);
	//void VN_EXPORT Log(const char *name,const char *content);
	//bool VN_EXPORT SetInstrumentNumber(int number);//ȡ����

 
	void VN_EXPORT AddPeriod(const char *InstrumentID, int periodtype, bool printfdata);

	void VN_EXPORT AddStopMonitor(const char *InstrumentID, int OrderRef,int mode,double percent);

	void VN_EXPORT DeleteStopMonitor(const char *InstrumentID, int OrderRef, int mode, double percent);
	
	double VN_EXPORT GetPeriodData(const char *InstrumentID, int periodtype, int PriceType, int ref);

	void VN_EXPORT *GetData(int index);

	int VN_EXPORT GetInstrumentNum();

	int VN_EXPORT IsInitOK();

	//double VN_EXPORT MA(const char *InstrumentID, int periodtype,int PriceType,int ref,int number);
	//double VN_EXPORT RSI(const char *InstrumentID, int periodtype, int PriceType, int ref, int number);
	//double VN_EXPORT SAR(const char *InstrumentID, int periodtype, int PriceType, int ref, int number);
	//double VN_EXPORT SAR(const char *InstrumentID, int periodtype, int PriceType, int ref, int number, double step, double maxvalue);
	//double VN_EXPORT MACD(const char *InstrumentID, int periodtype, int PriceType, int ref, int number1,int number2,int number3); 
	//double VN_EXPORT CCI(const char *InstrumentID, int periodtype, int PriceType, int ref, int number);
	//double VN_EXPORT ATR(const char *InstrumentID, int periodtype, int PriceType, int ref, int number);
	//double VN_EXPORT SAR(const char *InstrumentID, int periodtype, int PriceType, int ref, int number);
	//bool VN_EXPORT CROSSUP(int price_Small0, int priceLong0, int priceSmall1, int priceLong1);
	//bool VN_EXPORT CROSSDOWN(int priceSmall0, int priceLong0, int priceSmall1, int priceLong1);


	bool VN_EXPORT CROSSUP(const char *InstrumentID, int indicators,int periodtype, int PriceType, int period1, int period2, bool printstate);

	bool VN_EXPORT CROSSDOWN(const char *InstrumentID, int indicators, int periodtype, int PriceType, int period1, int period2, bool printstate);

	bool VN_EXPORT CROSSUP_S(int strategyid, int periodtype, int PriceType, int period1, int period2);

	bool VN_EXPORT CROSSDOWN_S(int strategyid, int periodtype, int PriceType, int period1, int period2);


#ifdef __cplusplus
}
#endif