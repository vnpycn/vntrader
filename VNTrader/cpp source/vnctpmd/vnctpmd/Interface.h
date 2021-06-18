/*
1.本文件为VNTrader 期货CTP交易库底层代码
2.VNTrader及本C++库开源协议MIT协议

对VNPY开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.vnpy.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的vnpy库里。

官方网站：http://www.vnpy.cn
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

	 ///初始化
	 ///@remark 初始化运行环境,只有调用后,接口才开始工作
	 void VN_EXPORT Init();

	///订阅询价。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	int VN_EXPORT SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount);

	///退订询价。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	int VN_EXPORT UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount);

	///注册名字服务器用户信息
	///@param pFensUserInfo：用户信息。
	void VN_EXPORT RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo);

	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	 void VN_EXPORT Release();

	 ///注册回调接口
	 ///@param pSpi 派生自回调接口类的实例
	 void VN_EXPORT RegisterSpi(CThostFtdcMdSpi *pSpi);
	 */
	 /////////////////////////////////////////
	//用户登录请求
	int VN_EXPORT ReqUserLogin();
	//用户登出请求
	int VN_EXPORT ReqUserLogout();
	//取消订阅
	int VN_EXPORT UnSubscribeMarketData( char *InstrumentID);

	int VN_EXPORT SubscribeMarketData(char *InstrumentID);

    //询价
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
	//bool VN_EXPORT SetInstrumentNumber(int number);//取消了

 
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