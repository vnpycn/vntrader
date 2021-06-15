/*
1.本文件为VNTrader 期货CTP交易库底层代码
2.VNTrader及本C++库开源协议MIT协议

对VNPY开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.vnpy.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的vnpy库里。

官方网站：http://www.vnpy.cn
*/
#include "stdafx.h"
#include "MdSpi.h"
#include "mapdef.h"
#include <map>
#include <string>
#include <algorithm>
#include "iostream"

#include <iostream> 
#include <windows.h>
using namespace std;
extern void(*OnFrontConnectedCallback)();
extern void(*OnFrontDisconnectedCallback)(int *a);
extern void(*OnRspUserLoginCallback)();
extern void(*OnRspUserLogoutCallback)();
extern void(*OnRspSubMarketDataCallback)(const int* a);
extern void(*OnRspUnSubMarketDataCallback)();
extern void(*OnRtnDepthMarketDataCallback)(const CThostFtdcDepthMarketDataField* a);


CThostFtdcMdApi *mpUserApi = NULL;

 // extern HANDLE hEvent[MAX_EVENTNUM];

#pragma warning(disable : 4996)

 int max_pure_tick_num = 130;  //保存120TICK的买卖量 的数量
 int max_pure_tick_move_num = 129; //保存120TICK的买卖量 的数量

 int max_pure_m1_num = 480;  //每1分钟 120TICK
 int max_pure_m1_move_num = 479;  //每1分钟 120TICK

 int max_pure_m3_num = 361;  //160  //每3分钟 360TICK
 int max_pure_m3_move_num = 360;  //159  //每3分钟 360TICK

 int max_pure_m5_num = 96;  //每3分钟 360TICK
 int max_pure_m5_move_num = 95; //每3分钟 360TICK

 int max_pure_m10_num = 96;  //每3分钟 360TICK
 int max_pure_m10_move_num = 95; //每3分钟 360TICK

 int  max_pure_m15_num = 96;  //每3分钟 360TICK
 int  max_pure_m15_move_num = 95; //每3分钟 360TICK

 int  max_pure_m30_num = 96;  //每3分钟 360TICK
 int  max_pure_m30_move_num = 95; //每3分钟 360TICK

 int  max_pure_m60_num = 96; //每3分钟 360TICK
 int  max_pure_m60_move_num = 95; //每3分钟 360TICK

 int  max_pure_m120_num = 96;  //每3分钟 360TICK
 int  max_pure_m120_move_num = 95; //每3分钟 360TICK

 int  max_pure_m240_num = 96;  //每3分钟 360TICK
 int  max_pure_m240_move_num = 95;//每3分钟 360TICK

 int  max_pure_d1_num = 30;  //每3分钟 360TICK
 int  max_pure_d1_move_num = 29; //每3分钟 360TICK

 int  max_pure_m480_num = 100;  //按每天8小时估算为480分钟
 int  max_pure_m480_move_num = 99;  //按每天8小时估算为480分钟
// 配置参数
extern std::string gFrontAddr[3];
extern std::string gBrokerID;
extern std::string gInvestorID;
extern std::string gPassword;

extern char* ppInstrumentID[];	
extern CThostFtdcDepthMarketDataField *depthdata[];
extern int size;
extern int amount;
extern std::map<std::string, int> gMarket;

 
extern CThostFtdcDepthMarketDataField* depthdata1;
 
using namespace std;
typedef map<string, InstrumentInfo> QS_Strategy_Map;
typedef hash_map<string, GuestOnlineHash> QS_Data_Map;
extern QS_Data_Map     mapData;
extern QS_Strategy_Map mapStrategy;
//新增加

/*
QS_Strategy_Map::iterator it = mapData->find(listinfo->InstrumentID);
if (it != mapData->end())
{
it->second.leftamount = leftamount;
it->second.enablestate[1] = false;
it->second.colorstate = false;
it->second.color = RGB(255, 246, 128);
lastcolor = true;
}
*/


int errornum_del = 0;
int errornum_add = 0;


bool NotFirst_3M = false;
bool NotFirst_5M = false;
bool NotFirst_10M = false;
bool NotFirst_15M = false;
bool NotFirst_30M = false;
bool NotFirst_60M = false;

double CAL_MIDDLE_AMOUNT = 80;
double CAL_BIG_AMOUNT = 50;// = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 ,100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };        //大品种大单的定义
bool CAL_AMOUNT_STATE = true;// = { true, true, true, true, true, true, true, true, true, true ,true, true, true, true, true, true, true, true, true, true };        //true需要更新BIG_AMOUNT
int  GetMin(double thistime)
{
	int h = (int)(thistime * 100); //  21
	int m = (int)(thistime * 10000) - 100 * h; //  2130-2100=30
	return  (h * 60 + m);
}
string dbtoch(double nums)		//将double 转换为 string 进行保存
{
	char chr[20] = { 0 };
	sprintf(chr, "%.0f", nums);
	string tt = chr;
	return tt;
}


void WirteSingleRecordToFile8(int id, const char * content)
{
	char str[200] = { 0 };
	strcat_s(str, 200, "TradeRecord.txt");

	//检查文件是否存在，是否需要新建文本文件
	ifstream inf;
	ofstream ouf;
	//inf.open(TickFileWritepaths[i], ios::out);
	inf.open(str, ios::out);
	//}

	//记录TICK数据
	ofstream o_file(str, ios::app);
	//if ( RunMode && ( check0 || check1 || check2) )

	//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

	o_file << content << "\t" << endl;

	//else
	//{
	//o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //将内容写入到文本文件中
	//}
	o_file.close();						//关闭文件

}

inline bool  CheckData(double data)
{
	if ((!_isnan(data)) && data > 1e-10)
	{
		return true;
	}
	return false;
}
inline bool  CheckDataOnlyNum(double data)
{
	if (!_isnan(data))
	{
		return true;
	}
	return false;
}
double zero_max(double a, double b)
{
	if (a == 0 && b == 0)
	{
		return 0;
	}
	else if (a == 0)
	{
		return b;
	}
	else if (b == 0)
	{
		return a;
	}
	else
	{

		return max(a, b);
	}


}
double zero_min(double a, double b)
{
	if (a == 0 && b == 0)
	{
		return 0;
	}
	else if (a == 0)
	{
		return b;
	}
	else if (b == 0)
	{
		return a;
	}
	else
	{

		return min(a, b);
	}
}
bool CheckFirst(double time, int cycle)
{
	//0.091205
	int temp = (int)(time * 10000) - 100 * int(time * 100);
	if (temp % cycle == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ModifyMap( GuestOnlineHash value, const char * InstrumentID, CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	std::hash_map<string, GuestOnlineHash>::iterator it;
	it = mapData.find(InstrumentID);
	if (it == mapData.end())
		mapData.insert(std::make_pair(InstrumentID, value));
	else
	{
		GuestOnlineHash * q = &(it->second);
		char names[10] = { 0 };
		char times[10] = { 0 };

		strcpy_s(times, sizeof(times), (char*)(&pDepthMarketData->UpdateTime));
		string str0 = names;
		string str1 = times;
		string str2 = times;
		string str3 = times;

		str1 = str1.substr(0, 2);
		str2 = str2.substr(3, 2);
		str3 = str3.substr(6, 2);
		int  hours = atoi(str1.c_str());
		int  minutes = atoi(str2.c_str());
		int  seconds = atoi(str3.c_str());
		double thistime = 0.01* hours + 0.0001* minutes + 0.000001*seconds;
		{
			


			int Q_BarTime_1 = hours * 60 * 60 + minutes * 60 + seconds;						//时间采用秒计
			double Q_BarTime_2 = (1 / 10e1)*hours + (1 / 10e3)*minutes + (1 / 10e5)*seconds;	//时间格式0.145100 = 14：51：00	

																								//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX这个应该见识是否恢复Q_BarTime_1n[i] = Q_BarTime_1;

			q->tick_data[0] = 2;		//设置标志位
			q->tick_data[1] = atof(pDepthMarketData->TradingDay);
			q->tick_data[2] = Q_BarTime_2;//pDepthMarketData->UpdateTime;
			q->tick_data[3] = pDepthMarketData->UpdateMillisec;

			q->tick_data[4] = pDepthMarketData->LastPrice;
			q->tick_data[5] = pDepthMarketData->AveragePrice;
			q->tick_data[6] = pDepthMarketData->HighestPrice;
			q->tick_data[7] = pDepthMarketData->LowestPrice;

			//q->LastHighPrice = z_max(q->LastHighPrice, NewPrice);
			//q->LastLowPrice = z_min(q->LastLowPrice, NewPrice);

			bool check1, check2, check3, check4, check5, check6, check7;

			bool typecheck1 = (q->TYPEID == TYPE_T) || (q->TYPEID == TYPE_TF);
			bool typecheck2 = (q->TYPEID == TYPE_IC) || (q->TYPEID == TYPE_IF) || (q->TYPEID == TYPE_IH);
			bool typecheck3 = (!typecheck1) && (!typecheck2); //(q->TYPEID == TYPE_PP) || (q->TYPEID == TYPE_CS) || (q->TYPEID == TYPE_JD) || (q->TYPEID == TYPE_L) || (q->TYPEID == TYPE_V) || (q->TYPEID == TYPE_C) || (q->TYPEID == TYPE_WH) || (q->TYPEID == TYPE_SM) || (q->TYPEID == TYPE_SF);
			bool typecheck4 = (q->TYPEID == TYPE_I) || (q->TYPEID == TYPE_TA) || (q->TYPEID == TYPE_A) || (q->TYPEID == TYPE_M) || (q->TYPEID == TYPE_Y) || (q->TYPEID == TYPE_P) || (q->TYPEID == TYPE_MA) || (q->TYPEID == TYPE_FG);
			bool typecheck5 = (q->TYPEID == TYPE_RU) || (q->TYPEID == TYPE_BU) || (q->TYPEID == TYPE_RB);
			bool typecheck6 = (q->TYPEID == TYPE_NI) || (q->TYPEID == TYPE_ZN) || (q->TYPEID == TYPE_AL) || (q->TYPEID == TYPE_CU);  // (!typecheck2) && (!typecheck3);
			bool typecheck7 = (q->TYPEID == TYPE_AU) || (q->TYPEID == TYPE_AG);




			check1 = (q->tick_data[2]>0.0910 && q->tick_data[2]<0.0915 && typecheck1) || (q->tick_data[2]<0.1518 && q->tick_data[2]>0.0914 && !q->FristTick && typecheck1);  //国债
			check2 = (q->tick_data[2]>0.0925 && q->tick_data[2]<0.0930 && typecheck2) || (q->tick_data[2]<0.1505 && q->tick_data[2]>0.0929 && !q->FristTick && typecheck2); //股指
			check3 = (q->tick_data[2]>0.0856 && q->tick_data[2]<0.0900 && typecheck3) || (q->tick_data[2]<0.1505 && q->tick_data[2]>0.0859 && !q->FristTick && typecheck3);  //白天
			check4 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck4) || ((q->tick_data[2]<0.2335 && q->tick_data[2]>0.2059) && !q->FristTick && typecheck4); //有夜盘  23:30结束
			check5 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck5) || ((q->tick_data[2]<0.2305 && q->tick_data[2]>0.2059) && !q->FristTick && typecheck5); //有夜盘  23:00结束
			check6 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck6) || ((q->tick_data[2]<0.0105 || q->tick_data[2]>0.2059) && !q->FristTick && typecheck6); //有夜盘  1:00结束
			check7 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck7) || ((q->tick_data[2]<0.0235 || q->tick_data[2]>0.2059) && !q->FristTick && typecheck7); //有夜盘  2:30结束  


			if ((check1 || check2 || check3 || check4 || check5 || check6 || check7))
			{
				q->M1Klinesig = true;
				q->tick_data[8] = pDepthMarketData->UpperLimitPrice;	//涨停价
				q->tick_data[9] = pDepthMarketData->LowerLimitPrice;	//跌停价					


				if ((pDepthMarketData->OpenPrice<1e-10) || _isnan(pDepthMarketData->OpenPrice))
				{
					//printf("pDepthMarketData->OpenPrice <1e-10\n");

					char chr[150] = { 0 };
					_snprintf_s(chr, sizeof(chr), sizeof(chr), "OpenPrice<1e-10或OpenPrice不是有效的数字[%0.06f][%0.02f]", Q_BarTime_2, pDepthMarketData->LastPrice);
					WirteSingleRecordToFile8(0, chr);
					return;
				}

				if (pDepthMarketData->OpenPrice>((pDepthMarketData->UpperLimitPrice) + 1) || pDepthMarketData->OpenPrice<((pDepthMarketData->LowerLimitPrice) - 1))
				{
					//printf("pDepthMarketData->OpenPrice超出涨跌停价\n");

					char chr[150] = { 0 };
					_snprintf_s(chr, sizeof(chr), sizeof(chr), "pDepthMarketData->OpenPrice超出涨跌停价[%0.06f][%0.02f] 涨停[%0.02f] 跌停[%0.02f]", Q_BarTime_2, pDepthMarketData->LastPrice, pDepthMarketData->UpperLimitPrice, pDepthMarketData->LowerLimitPrice);
					WirteSingleRecordToFile8(0, chr);
					return;
				}

				q->Day_open[0] = pDepthMarketData->OpenPrice;

				q->M1_open[1] = q->Day_open[0];
				q->M1_high[1] = q->Day_open[0];
				q->M1_low[1] = q->Day_open[0];
				q->M1_close[1] = q->Day_open[0];

				q->M1_open[0] = q->Day_open[0];
				q->M1_high[0] = q->Day_open[0];		    //为什么不影响刷新？是否有必要存在？-----------------------------------------------------------------------------------
				q->M1_low[0] = q->Day_open[0];
				q->M1_close[0] = q->Day_open[0];
				//为什么不影响刷新？是否有必要存在？-----------------------------------------------------------------------------------

				//恢复 printf("\nPPPPPPPPPPPPPPPPPPPPPPPPP__________________________多次运行了____________________________pppppppppppppppppppppppppp");
				q->M1_BS_Time[0] = q->tick_BS_Time[0];

				q->M3_open[1] = q->Day_open[0];
				q->M3_high[1] = q->Day_open[0];
				q->M3_low[1] = q->Day_open[0];
				q->M3_close[1] = q->Day_open[0];
				q->M3_open[0] = q->Day_open[0];
				q->M3_high[0] = q->Day_open[0];
				q->M3_low[0] = q->Day_open[0];
				q->M3_close[0] = q->Day_open[0];
				q->M3_BS_Time[0] = q->tick_BS_Time[0];


				q->M5_open[1] = q->Day_open[0];
				q->M5_high[1] = q->Day_open[0];
				q->M5_low[1] = q->Day_open[0];
				q->M5_close[1] = q->Day_open[0];
				q->M5_open[0] = q->Day_open[0];
				q->M5_high[0] = q->Day_open[0];
				q->M5_low[0] = q->Day_open[0];
				q->M5_close[0] = q->Day_open[0];
				q->M5_BS_Time[0] = q->tick_BS_Time[0];	   //为什么每次都刷新


				q->M10_open[1] = q->Day_open[0];
				q->M10_high[1] = q->Day_open[0];
				q->M10_low[1] = q->Day_open[0];
				q->M10_close[1] = q->Day_open[0];
				q->M10_open[0] = q->Day_open[0];
				q->M10_high[0] = q->Day_open[0];
				q->M10_low[0] = q->Day_open[0];
				q->M10_close[0] = q->Day_open[0];
				q->M10_BS_Time[0] = q->tick_BS_Time[0];	

				q->M15_open[1] = q->Day_open[0];
				q->M15_high[1] = q->Day_open[0];
				q->M15_low[1] = q->Day_open[0];
				q->M15_close[1] = q->Day_open[0];
				q->M15_open[0] = q->Day_open[0];
				q->M15_high[0] = q->Day_open[0];
				q->M15_low[0] = q->Day_open[0];
				q->M15_close[0] = q->Day_open[0];
				q->M15_BS_Time[0] = q->tick_BS_Time[0];	


				q->M30_open[1] = q->Day_open[0];
				q->M30_high[1] = q->Day_open[0];
				q->M30_low[1] = q->Day_open[0];
				q->M30_close[1] = q->Day_open[0];
				q->M30_open[0] = q->Day_open[0];
				q->M30_high[0] = q->Day_open[0];
				q->M30_low[0] = q->Day_open[0];
				q->M30_close[0] = q->Day_open[0];
				q->M30_BS_Time[0] = q->tick_BS_Time[0];


				q->M60_open[1] = q->Day_open[0];
				q->M60_high[1] = q->Day_open[0];
				q->M60_low[1] = q->Day_open[0];
				q->M60_close[1] = q->Day_open[0];
				q->M60_open[0] = q->Day_open[0];
				q->M60_high[0] = q->Day_open[0];
				q->M60_low[0] = q->Day_open[0];
				q->M60_close[0] = q->Day_open[0];
				q->M60_BS_Time[0] = q->tick_BS_Time[0];	

														   //?????????????有啥用
														   //q->Day_CloseProfit = 0;
														   //q->Day_CloseProfitA = 0;
														   //q->Day_CloseProfitB = 0;

														   //q->Day_TradeNumb = 0;

			}






			//接上面60-129
			for (int j = max_pure_tick_move_num; j >MAX_TICK_MOVE_NUM; j--) //注意和下面数字如何接循环的
			{

				// printf("____X:%d\n",j);

				q->tick_BS_BUY_Value[j] = q->tick_BS_BUY_Value[j - 1];
				//tick_BS_BUY_Time[j] = tick_BS_BUY_Time[j-1];
				q->tick_BS_SELL_Value[j] = q->tick_BS_SELL_Value[j - 1];
				//tick_BS_SELL_Time[j] = tick_BS_SELL_Time[j-1];
				q->tick_BS_PURE_Value[j] = q->tick_BS_PURE_Value[j - 1];
				q->tick_BS_Time[j] = q->tick_BS_Time[j - 1];
				q->tick_BS_BL_Value[j] = q->tick_BS_BL_Value[j - 1];
				//			tick_BS_BL_Time[j] = tick_BS_BL_Time[j-1];

			}

			//1-59
			for (int j = MAX_TICK_MOVE_NUM; j >0; j--)
			{
				//printf("\nX:%d",j);
				q->tick_AskPrice1[j] = q->tick_AskPrice1[j - 1];
				q->tick_BidPrice1[j] = q->tick_BidPrice1[j - 1];
				q->tick_AskVolume1[j] = q->tick_AskVolume1[j - 1];
				q->tick_BidVolume1[j] = q->tick_BidVolume1[j - 1];
				q->tick_Volume[j] = q->tick_Volume[j - 1];
				q->tick_OpenInterest[j] = q->tick_OpenInterest[j - 1]; //持仓量

																	   
				q->tick_BS_BUY_Value[j] = q->tick_BS_BUY_Value[j - 1];
				//tick_BS_BUY_Time[j] = tick_BS_BUY_Time[j-1];
				q->tick_BS_SELL_Value[j] = q->tick_BS_SELL_Value[j - 1];
				//tick_BS_SELL_Time[j] = tick_BS_SELL_Time[j-1];
				q->tick_BS_PURE_Value[j] = q->tick_BS_PURE_Value[j - 1];
				q->tick_BS_Time[j] = q->tick_BS_Time[j - 1];
				q->tick_BS_BL_Value[j] = q->tick_BS_BL_Value[j - 1];
				//			tick_BS_BL_Time[j] = tick_BS_BL_Time[j-1];

			}



			//实盘开盘前登录，交易所此处插入的最大值，需屏蔽处理
			//bool sstate;
			// if (serverselect) //实盘时间
			//{
			//老的sstate=(tick_data[2]>0.0250 && tick_data[2]<0.0850) || (tick_data[2]>0.1550 && tick_data[2]<0.2050); 
			bool sstate = (q->tick_data[2]>0.0250 && q->tick_data[2]<0.0850) || (q->tick_data[2]>0.1550 && q->tick_data[2]<0.2050);
			//}
			//else            //24小时服务器
			//{
			//  sstate = false;//(tick_data[2]>0.0250 && tick_data[2]<0.0850) || (tick_data[2]>0.1550 && tick_data[2]<0.2050);
			//}

			if (sstate)
				//if (AskPrice1t>99999 || BidPrice1t>99999)
			{
				q->tick_AskPrice1[0] = 0;
				q->tick_BidPrice1[0] = 0;
			}
			else
			{
				q->tick_AskPrice1[0] = pDepthMarketData->AskPrice1;
				q->tick_BidPrice1[0] = pDepthMarketData->BidPrice1;
			}

			q->tick_AskVolume1[0] = pDepthMarketData->AskVolume1;
			q->tick_BidVolume1[0] = pDepthMarketData->BidVolume1;
			q->tick_Volume[0] = pDepthMarketData->Volume;  //总成交量   单位时间成交量tick_Volume[0]-tick_Volume[1]
			q->tick_OpenInterest[0] = pDepthMarketData->OpenInterest;



			//printf("\n问价 [%.02f] 卖一价[%.02f] 买一价[%.02f]\n",tick_data[4],tick_AskPrice1[0],tick_BidPrice1[0]);
			//恢复a  		printf("\n 卖一[%.02f] 买一[%.02f] 毫秒[%.02f]时间[%.08f]\n",tick_AskVolume1[0]-tick_BidVolume1[1],tick_OpenInterest[0],tick_data[3],Q_BarTime_2); //2次毫秒数都一样，说明没更新

			//输出每笔成交量	

			//char DD[200];
			//string ttt=DD;


			//为何重复2次
			double thisvol = 0;
			//检查
			if (q->tick_Volume[1]>0)
			{
				thisvol = q->tick_Volume[0] - q->tick_Volume[1];
			}
			//检查

			if (q->tick_data[4] == q->tick_AskPrice1[0])  //卖一价成交，应该是主动买入
			{
				q->tick_BS_BUY_Value[0] = thisvol;
				q->tick_BS_SELL_Value[0] = 0;
				q->tick_BS_PURE_Value[0] = thisvol;
				// tick_BS_BUY_Time[0]=  tick_data[2];
				// tick_BS_SELL_Time[0]= tick_data[2];
				q->tick_BS_Time[0] = q->tick_data[2];
				//printf("\n————量 [%.00f] 卖一量[%.00f] 买一量[%.00f]\n",tick_Volume[0]-tick_Volume[1],tick_AskVolume1[0],tick_BidVolume1[0]);
				// printf("\n————量 [%.00f]\n",tick_Volume[0]-tick_Volume[1]);
			}
			else //买一价成交，应该是主动卖入
			{
				q->tick_BS_BUY_Value[0] = 0;
				q->tick_BS_SELL_Value[0] = thisvol;//pDepthMarketData->Volume;
				q->tick_BS_PURE_Value[0] = (-1)*thisvol;//(-1)*pDepthMarketData->Volume;
														// tick_BS_BUY_Time[0]=  tick_data[2];
														// tick_BS_SELL_Time[0]= tick_data[2];
				q->tick_BS_Time[0] = q->tick_data[2];
				// printf("\n————量 [%.00f]\n",tick_Volume[1]-tick_Volume[0]);

			}

		

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////



			//printf("%s 检查是否更新数据\n", InstrumentID);
			bool Timemore0 = false, Timemore1 = false, Timemore2 = false;

			//switch (TYPEID)
			switch (q->TYPEID)
			{
				////////////////////////////////////////////////////
			case TYPE_PP:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_CS:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_JD:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_L:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_V:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_C:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;


			case TYPE_WH:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;

			case TYPE_SM:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;

			case TYPE_SF:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;

			///////////////////////////////////////
			case TYPE_T:  //注意国债时间没更正
			{
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
			}
			break;
			case TYPE_TF:
			{
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
			}
			break;
			///////////////////////////////////////
			case TYPE_IC:
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
				break;
			case TYPE_IF:
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
				break;
			case TYPE_IH:
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
				break;
				////////////////////////////////////////
			case TYPE_I:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_TA:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_A:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_M:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_Y:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_P:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_MA:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;


			case TYPE_FG:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case TYPE_RU:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2300);
				break;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case TYPE_NI:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;
				break;
			case TYPE_ZN:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;
				break;
			case TYPE_AL:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;
				break;
			case TYPE_CU:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;
				break;
			case TYPE_BU:
				//Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				//Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				//Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;

				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2300);
				break;
			case TYPE_RB:
				//Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				//Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				//Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;

				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2300);
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////
			case TYPE_AU:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0230;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0230;
				break;
			case TYPE_AG:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0230;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0230;
				break;
			case TYPE_NEW1:

				break;
			case TYPE_NEW2:

				break;
			case TYPE_NEW3:

				break;
			case TYPE_NEW4:

				break;
			case TYPE_NEW5:

				break;
			case TYPE_NEW6:

				break;
			case TYPE_NEW7:

				break;
			case TYPE_NEW8:

				break;
			case TYPE_NEW9:

				break;
			case TYPE_NEW10:

				break;
			case TYPE_NEW11:

				break;
			case TYPE_NEW12:

				break;
			case TYPE_NEW13:

				break;
			case TYPE_NEW14:

				break;
			case TYPE_NEW15:

				break;
			case TYPE_NEW16:

				break;
			case TYPE_NEW17:

				break;
			case TYPE_NEW18:

				break;
			case TYPE_NEW19:

				break;
			case TYPE_NEW20:

				break;

				///////////////////////////////////////////////////////////////////////////////////////////////////////////
			}

			//it->instructment
			//if (!Timemore0)
			//printf ("pass 1\n");
			//if (!Timemore1)
			//printf ("pass 2\n");
			//if (!Timemore2)
			//printf("pass 3\n");
			//if (!(seconds >= 0 && seconds < 40))
			//printf ("pass 4\n");
			//if (!(pDepthMarketData->Volume > q->tick_VolumeLast))
			//printf ("pass 5\n");
			//if (!q->M1Klinesig == false)
			//printf ("pass 6\n");

			if (Timemore0 && Timemore1  && Timemore2 && seconds >= 0 && seconds<40 && pDepthMarketData->Volume>q->tick_VolumeLast && q->M1Klinesig == false)
			{
				//printf("************************更新**********************\n\n");

				//char th[20] = { 0 };
				//sprintf_s(th,sizeof(th),"%d   %0.06f", q->period_M1, Q_BarTime_2);
				//WirteSingleRecordToFile8(0, th);


				q->tick_VolumeLast = pDepthMarketData->Volume;

				q->M1Klinesig = true;



				if (q->period_M1)
				{

					//printf("%s 更新M1\n", InstrumentID);

					for (int j = max_pure_m1_move_num; j >0; j--) //1~59执行了
					{


						//printf("M1 pDepthMarketData->LastPrice[%d][%0.02f][%0.02f][%0.02f][%0.02f]\n",j, q->M1_open[j], q->M1_high[j], q->M1_low[j], q->M1_close[j]);

		
						//if (M1_open[j - 1] > 1e-10 && M1_high[j - 1] > 1e-10 && M1_low[j - 1] > 1e-10  && M1_close[j - 1] > 1e-10   && M1_BS_Time[j - 1] > 1e-10 && M1_BS_BUY_Value[j - 1] > 1e-10 && M1_BS_SELL_Value[j - 1] > 1e-10)
						if (CheckData(q->M1_open[j - 1]) && CheckData(q->M1_high[j - 1]) && CheckData(q->M1_low[j - 1]) && CheckData(q->M1_close[j - 1]) && CheckDataOnlyNum(q->M1_BS_Time[j - 1]) && q->M1_BS_Time[j - 1]>-0.5
							&& CheckDataOnlyNum(q->M1_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M1_BS_SELL_Value[j - 1])
							)

						{

							q->M1_open[j] = q->M1_open[j - 1];
							q->M1_high[j] = q->M1_high[j - 1];
							q->M1_low[j] = q->M1_low[j - 1];
							q->M1_close[j] = q->M1_close[j - 1];
							q->M1_BS_Time[j] = q->M1_BS_Time[j - 1];

							q->M1_BS_PURE_Value[j] = q->M1_BS_PURE_Value[j - 1];
							q->M1_BS_BUY_Value[j] = q->M1_BS_BUY_Value[j - 1];
							q->M1_BS_SELL_Value[j] = q->M1_BS_SELL_Value[j - 1];
							q->M1_BS_BL_Value[j] = q->M1_BS_BL_Value[j - 1];

							q->M1_BS_SMALL_PURE_Value[j] = q->M1_BS_SMALL_PURE_Value[j - 1];
							q->M1_BS_SMALL_BUY_Value[j] = q->M1_BS_SMALL_BUY_Value[j - 1];
							q->M1_BS_SMALL_SELL_Value[j] = q->M1_BS_SMALL_SELL_Value[j - 1];
							q->M1_BS_SMALL_BL_Value[j] = q->M1_BS_SMALL_BL_Value[j - 1];

							q->M1_BS_MIDDLE_PURE_Value[j] = q->M1_BS_MIDDLE_PURE_Value[j - 1];
							q->M1_BS_MIDDLE_BUY_Value[j] = q->M1_BS_MIDDLE_BUY_Value[j - 1];
							q->M1_BS_MIDDLE_SELL_Value[j] = q->M1_BS_MIDDLE_SELL_Value[j - 1];
							q->M1_BS_MIDDLE_BL_Value[j] = q->M1_BS_MIDDLE_BL_Value[j - 1];

							q->M1_BS_BIG_PURE_Value[j] = q->M1_BS_BIG_PURE_Value[j - 1];
							q->M1_BS_BIG_BUY_Value[j] = q->M1_BS_BIG_BUY_Value[j - 1];
							q->M1_BS_BIG_SELL_Value[j] = q->M1_BS_BIG_SELL_Value[j - 1];
							q->M1_BS_BIG_BL_Value[j] = q->M1_BS_BIG_BL_Value[j - 1];

							q->M1_BS_TOTAL_PURE_Value[j] = q->M1_BS_TOTAL_PURE_Value[j - 1];
							q->M1_BS_TOTAL_BUY_Value[j] = q->M1_BS_TOTAL_BUY_Value[j - 1];
							q->M1_BS_TOTAL_SELL_Value[j] = q->M1_BS_TOTAL_SELL_Value[j - 1];
							q->M1_BS_TOTAL_BL_Value[j] = q->M1_BS_TOTAL_BL_Value[j - 1];

							q->M1_BS_TOTAL_SMALL_PURE_Value[j] = q->M1_BS_TOTAL_SMALL_PURE_Value[j - 1];
							q->M1_BS_TOTAL_SMALL_BUY_Value[j] = q->M1_BS_TOTAL_SMALL_BUY_Value[j - 1];
							q->M1_BS_TOTAL_SMALL_SELL_Value[j] = q->M1_BS_TOTAL_SMALL_SELL_Value[j - 1];
							q->M1_BS_TOTAL_SMALL_BL_Value[j] = q->M1_BS_TOTAL_SMALL_BL_Value[j - 1];


							q->M1_BS_TOTAL_MIDDLE_PURE_Value[j] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[j - 1];
							q->M1_BS_TOTAL_MIDDLE_BUY_Value[j] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[j - 1];
							q->M1_BS_TOTAL_MIDDLE_SELL_Value[j] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[j - 1];
							q->M1_BS_TOTAL_MIDDLE_BL_Value[j] = q->M1_BS_TOTAL_MIDDLE_BL_Value[j - 1];

							q->M1_BS_TOTAL_BIG_PURE_Value[j] = q->M1_BS_TOTAL_BIG_PURE_Value[j - 1];
							q->M1_BS_TOTAL_BIG_BUY_Value[j] = q->M1_BS_TOTAL_BIG_BUY_Value[j - 1];
							q->M1_BS_TOTAL_BIG_SELL_Value[j] = q->M1_BS_TOTAL_BIG_SELL_Value[j - 1];
							q->M1_BS_TOTAL_BIG_BL_Value[j] = q->M1_BS_TOTAL_BIG_BL_Value[j - 1];

							q->M1_BS_CROSS_TOTAL_PURE_Value[j] = q->M1_BS_CROSS_TOTAL_PURE_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BUY_Value[j] = q->M1_BS_CROSS_TOTAL_BUY_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_SELL_Value[j] = q->M1_BS_CROSS_TOTAL_SELL_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BL_Value[j] = q->M1_BS_CROSS_TOTAL_BL_Value[j - 1];

							q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[j] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[j] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[j] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_SMALL_BL_Value[j] = q->M1_BS_CROSS_TOTAL_SMALL_BL_Value[j - 1];

							q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[j] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[j] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[j] = q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_MIDDLE_BL_Value[j] = q->M1_BS_CROSS_TOTAL_MIDDLE_BL_Value[j - 1];

							q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[j] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[j] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[j] = q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BIG_BL_Value[j] = q->M1_BS_CROSS_TOTAL_BIG_BL_Value[j - 1];

							bool tempdata = true; //临时声明，应该删除声明
						}
						else if (_isnan(q->M1_open[j - 1]) || _isnan(q->M1_high[j - 1]) || _isnan(q->M1_low[j - 1]) || _isnan(q->M1_close[j - 1]) || _isnan(q->M1_BS_Time[j - 1]) || _isnan(q->M1_BS_BUY_Value[j - 1]) || _isnan(q->M1_BS_SELL_Value[j - 1]))
						{

							if (j - 1 < 4)
							{
								//printf("错误(不是数字) [%d][%0.06f] M1_open[%0.02f] M1_high[%0.02f] M1_low[%0.02f] M1_close[%0.02f] M1_BS_Time[%0.02f] M1_BS_BUY_Value[%0.02f] M1_BS_SELL_Value[%0.02f]\n", j - 1, Q_BarTime_2, M1_open[j - 1], M1_high[j - 1], M1_low[j - 1], M1_close[j - 1], M1_BS_Time[j - 1], M1_BS_BUY_Value[j - 1], M1_BS_SELL_Value[j - 1]);

								//char chr[150] = { 0 };
								//sprintf_s(chr, sizeof(chr), "错误(不是数字) [%d][%s]M1_open[%0.02f] M1_high[%0.02f] M1_low[%0.02f] M1_close[%0.02f] M1_BS_Time[%0.02f] M1_BS_BUY_Value[%0.02f] M1_BS_SELL_Value[%0.02f]", j - 1, thistime, q->M1_open[j - 1], q->M1_high[j - 1], q->M1_low[j - 1], q->M1_close[j - 1], q->M1_BS_Time[j - 1], q->M1_BS_BUY_Value[j - 1], q->M1_BS_SELL_Value[j - 1]);
								//WirteTradeRecordToFileMainThreadB(0, chr);

							}

						}


			
					}


					char tttt[50] = { 0 };
					_snprintf_s(tttt, sizeof(tttt), sizeof(tttt), "%s", InstrumentID);
					WirteSingleRecordToFile8(0, tttt);

					for (int i = 1; i < 20; i++)
					{
						//if(q->M1_open[i]>1e-10 || q->M1_close[i]>1e-10 || q->M1_high[i]>1e-10 || q->M1_low[i]>1e-10)
						if (q->M1_open[i]>1e-10)
						{
							printf("M1:[%d]开盘[%0.02f]收盘[%0.02f]最高[%0.02f]最低[%0.02f]\n", i, q->M1_open[i], q->M1_close[i], q->M1_high[i], q->M1_low[i]);

							char th[100] = { 0 };
							_snprintf_s(th, sizeof(th), sizeof(th), "[%d]%0.06f M1 open[%0.02f]close[%0.02f]high[%0.02f]low[%0.02f]", i, q->M1_BS_Time[i], q->M1_open[i], q->M1_close[i], q->M1_high[i], q->M1_low[i]);
							WirteSingleRecordToFile8(0, th);
						}
					}
					printf("\n");
					WirteSingleRecordToFile8(0, "");
					for (int i = 1; i < 5; i++)
					{
						//if (q->M3_open[i] > 1e-10 || q->M3_close[i] > 1e-10 || q->M3_high[i] > 1e-10 || q->M3_low[i] > 1e-10) 
						if (q->M3_open[i]>1e-10)
						{
							printf("M3:[%d]开盘[%0.02f]收盘[%0.02f]最高[%0.02f]最低[%0.02f]\n", i, q->M3_open[i], q->M3_close[i], q->M3_high[i], q->M3_low[i]);

							char th[100] = { 0 };
							_snprintf_s(th, sizeof(th), sizeof(th), "[%d]%0.06f M3 open[%0.02f]close[%0.02f]high[%0.02f]low[%0.02f]", i, q->M3_BS_Time[i], q->M3_open[i], q->M3_close[i], q->M3_high[i], q->M3_low[i]);
							WirteSingleRecordToFile8(0, th);
						}



					}
					printf("\n");
					WirteSingleRecordToFile8(0, "");

					for (int i = 1; i < 5; i++)
					{
						//if (q->M5_open[i]>1e-10 || q->M5_close[i]>1e-10 || q->M5_high[i]>1e-10 || q->M5_low[i]>1e-10)
						if (q->M5_open[i]>1e-10)
						{
							printf("M5:[%d]开盘[%0.02f]收盘[%0.02f]最高[%0.02f]最低[%0.02f]\n", i, q->M5_open[i], q->M5_close[i], q->M5_high[i], q->M5_low[i]);

							char th[100] = { 0 };
							_snprintf_s(th, sizeof(th), sizeof(th), "[%d]%0.06f M5 open[%0.02f]close[%0.02f]high[%0.02f]low[%0.02f]", i, q->M5_BS_Time[i], q->M5_open[i], q->M5_close[i], q->M5_high[i], q->M5_low[i]);
							WirteSingleRecordToFile8(0, th);
						}



					}
















				}

				int tempm = 0;
				if (q->period_M3)
				{
					//printf("%s 更新M3\n", InstrumentID);
					//M3更新
					int tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M3_BS_Time[1]);
					if (NotFirst_3M)
					{

						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M3_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M3_BS_Time[1]));
						//if (tempm >= 3-1 || (tempm<0 && tempm >= -1437-1))
						if (tempm >= 3 || (tempm<0 && tempm >= -1437))
						{
							for (int j = max_pure_m3_move_num; j >1; j--) //2~59执行了
							{
				
								//if (M3_open[j - 1] > 1e-10 && M3_high[j - 1] > 1e-10 && M3_low[j - 1] > 1e-10  &&  M3_close[j - 1] > 1e-10 && M3_BS_Time[j - 1] > 1e-10 && M3_BS_BUY_Value[j - 1] > 1e-10 && M3_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M3_open[j - 1]) && CheckData(q->M3_high[j - 1]) && CheckData(q->M3_low[j - 1]) && CheckData(q->M3_close[j - 1]) && CheckDataOnlyNum(q->M3_BS_Time[j - 1]) && q->M3_BS_Time[j - 1]>-0.5

									&& CheckDataOnlyNum(q->M3_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M3_BS_SELL_Value[j - 1])
									)

								{
									q->M3_open[j] = q->M3_open[j - 1];
									q->M3_high[j] = q->M3_high[j - 1];
									q->M3_low[j] = q->M3_low[j - 1];
									q->M3_close[j] = q->M3_close[j - 1];
									q->M3_BS_Time[j] = q->M3_BS_Time[j - 1];

									q->M3_BS_PURE_Value[j] = q->M3_BS_PURE_Value[j - 1];
									q->M3_BS_BUY_Value[j] = q->M3_BS_BUY_Value[j - 1];
									q->M3_BS_SELL_Value[j] = q->M3_BS_SELL_Value[j - 1];
									q->M3_BS_BL_Value[j] = q->M3_BS_BL_Value[j - 1];

									q->M3_BS_BIG_PURE_Value[j] = q->M3_BS_BIG_PURE_Value[j - 1];
									q->M3_BS_BIG_BUY_Value[j] = q->M3_BS_BIG_BUY_Value[j - 1];
									q->M3_BS_BIG_SELL_Value[j] = q->M3_BS_BIG_SELL_Value[j - 1];
									q->M3_BS_BIG_BL_Value[j] = q->M3_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}

							}

							//比M1增加的处理部分
							q->M3_open[1] = q->M1_open[3];//M3_open[0];
							q->M3_high[1] = zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1])); //M3_high[j - 1];
							q->M3_low[1] = zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])); //M3_low[j - 1];
							q->M3_close[1] = q->M1_close[1]; //M3_close[j - 1];
							q->M3_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //M1_BS_Time[0];//M3_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M3_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// M3_BS_PURE_Value[j - 1];
							q->M3_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// M3_BS_BUY_Value[j - 1];
							q->M3_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//M3_BS_SELL_Value[j - 1];
							q->M3_BS_BL_Value[1] = q->M3_BS_BUY_Value[1] / q->M3_BS_SELL_Value[1];   //M3_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M3_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// M3_BS_BIG_PURE_Value[j - 1];
							q->M3_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// M3_BS_BIG_BUY_Value[j - 1];
							q->M3_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// M3_BS_BIG_SELL_Value[j - 1];
							q->M3_BS_BIG_BL_Value[1] = q->M3_BS_BIG_BUY_Value[1] / q->M3_BS_BIG_SELL_Value[1];// M3_BS_BIG_BL_Value[j - 1];
																											  //比M1增加的处理部分
																											  // int id = i;
																											  //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]M3_BS_Time[%0.02f] M3_close[%0.02f] M3_BS_PURE_Value[%0.02f] M3_BS_BUY_Value[%0.02f] M3_BS_SELL_Value[%0.02f] \n",
																											  //  tick_data[2], InstrumentID_n[id], i, M3_BS_Time[id][1], M3_close[id][1], M3_BS_PURE_Value[id][1], M3_BS_BUY_Value[id][1], M3_BS_SELL_Value[id][1]);
						}


					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 3 || (tempm<0 && tempm >= -1437)) && CheckFirst(q->M1_BS_Time[1], 3))
						{

							NotFirst_3M = true;

						}
					}
					//M3更新		
				}


				if (q->period_M5)
				{
					//printf("%s 更新M5\n", InstrumentID);
					//M5更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M5_BS_Time[1]);
					if (NotFirst_5M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M5_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M5_BS_Time[1]));
						//if (tempm >= 5-1 || (tempm < 0 && tempm >= -1435-1))

						if (tempm >= 5 || (tempm < 0 && tempm >= -1435))
						{
							for (int j = max_pure_m5_move_num; j > 1; j--) //2~59执行了
							{

								//if (M5_open[j - 1] > 1e-10 && M5_high[j - 1] > 1e-10 && M5_low[j - 1] > 1e-10  &&  M5_close[j - 1] > 1e-10 && M5_BS_Time[j - 1] > 1e-10 && M5_BS_BUY_Value[j - 1] > 1e-10 && M5_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M5_open[j - 1]) && CheckData(q->M5_high[j - 1]) && CheckData(q->M5_low[j - 1]) && CheckData(q->M5_close[j - 1]) && CheckDataOnlyNum(q->M5_BS_Time[j - 1]) && q->M5_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M5_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M5_BS_SELL_Value[j - 1])
									)

								{
									q->M5_open[j] = q->M5_open[j - 1];
									q->M5_high[j] = q->M5_high[j - 1];
									q->M5_low[j] = q->M5_low[j - 1];
									q->M5_close[j] = q->M5_close[j - 1];
									q->M5_BS_Time[j] = q->M5_BS_Time[j - 1];

									q->M5_BS_PURE_Value[j] = q->M5_BS_PURE_Value[j - 1];
									q->M5_BS_BUY_Value[j] = q->M5_BS_BUY_Value[j - 1];
									q->M5_BS_SELL_Value[j] = q->M5_BS_SELL_Value[j - 1];
									q->M5_BS_BL_Value[j] = q->M5_BS_BL_Value[j - 1];

									q->M5_BS_BIG_PURE_Value[j] = q->M5_BS_BIG_PURE_Value[j - 1];
									q->M5_BS_BIG_BUY_Value[j] = q->M5_BS_BIG_BUY_Value[j - 1];
									q->M5_BS_BIG_SELL_Value[j] = q->M5_BS_BIG_SELL_Value[j - 1];
									q->M5_BS_BIG_BL_Value[j] = q->M5_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}

							}

							//比M1增加的处理部分
							q->M5_open[1] = q->M1_open[5];//M5_open[0];
							q->M5_high[1] = zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))));
							q->M5_low[1] = zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))));
							q->M5_close[1] = q->M1_close[1]; //M5_close[j - 1];
							q->M5_BS_Time[1] = q->M1_BS_Time[1]; //5个周期的最后1个（结束时间）   //M1_BS_Time[0];//M5_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确


																 //未修正
							q->M5_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// M5_BS_PURE_Value[j - 1];
							q->M5_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// M5_BS_BUY_Value[j - 1];
							q->M5_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//M5_BS_SELL_Value[j - 1];
							q->M5_BS_BL_Value[1] = q->M5_BS_BUY_Value[1] / q->M5_BS_SELL_Value[1];   //M5_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M5_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// M5_BS_BIG_PURE_Value[j - 1];
							q->M5_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// M5_BS_BIG_BUY_Value[j - 1];
							q->M5_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// M5_BS_BIG_SELL_Value[j - 1];
							q->M5_BS_BIG_BL_Value[1] = q->M5_BS_BIG_BUY_Value[1] / q->M5_BS_BIG_SELL_Value[1];// M5_BS_BIG_BL_Value[j - 1];

																											  //未修正
																											  //比M1增加的处理部分
																											  // int id = i;
																											  //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]M5_BS_Time[%0.02f] M5_close[%0.02f] M5_BS_PURE_Value[%0.02f] M5_BS_BUY_Value[%0.02f] M5_BS_SELL_Value[%0.02f] \n",
																											  //  tick_data[2], InstrumentID_n[id], i, M5_BS_Time[id][1], M5_close[id][1], M5_BS_PURE_Value[id][1], M5_BS_BUY_Value[id][1], M5_BS_SELL_Value[id][1]);
						}
					}
					else
					{

						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 5 || (tempm<0 && tempm >= -1435)) && CheckFirst(q->M1_BS_Time[1], 5))
						{

							NotFirst_5M = true;

						}


					}
					//M5更新		

				}


				if (q->period_M10)
				{
					//printf("%s 更新M10\n", InstrumentID);
					//M10更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M10_BS_Time[1]);
					if (NotFirst_10M)
					{

						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M10_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M10_BS_Time[1]));
						if (tempm >= 10 || (tempm < 0 && tempm >= -1430))
						{
							for (int j = max_pure_m10_move_num; j > 1; j--) //2~59执行了
							{
	
								//if (M10_open[j - 1] > 1e-10 && M10_high[j - 1] > 1e-10 && M10_low[j - 1] > 1e-10  &&  M10_close[j - 1] > 1e-10 && M10_BS_Time[j - 1] > 1e-10 && M10_BS_BUY_Value[j - 1] > 1e-10 && M10_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M10_open[j - 1]) && CheckData(q->M10_high[j - 1]) && CheckData(q->M10_low[j - 1]) && CheckData(q->M10_close[j - 1]) && CheckDataOnlyNum(q->M10_BS_Time[j - 1]) && q->M10_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M10_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M10_BS_SELL_Value[j - 1])
									)

								{
									q->M10_open[j] = q->M10_open[j - 1];
									q->M10_high[j] = q->M10_high[j - 1];
									q->M10_low[j] = q->M10_low[j - 1];
									q->M10_close[j] = q->M10_close[j - 1];
									q->M10_BS_Time[j] = q->M10_BS_Time[j - 1];

									q->M10_BS_PURE_Value[j] = q->M10_BS_PURE_Value[j - 1];
									q->M10_BS_BUY_Value[j] = q->M10_BS_BUY_Value[j - 1];
									q->M10_BS_SELL_Value[j] = q->M10_BS_SELL_Value[j - 1];
									q->M10_BS_BL_Value[j] = q->M10_BS_BL_Value[j - 1];

									q->M10_BS_BIG_PURE_Value[j] = q->M10_BS_BIG_PURE_Value[j - 1];
									q->M10_BS_BIG_BUY_Value[j] = q->M10_BS_BIG_BUY_Value[j - 1];
									q->M10_BS_BIG_SELL_Value[j] = q->M10_BS_BIG_SELL_Value[j - 1];
									q->M10_BS_BIG_BL_Value[j] = q->M10_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}

							}

							//比M1增加的处理部分
							q->M10_open[1] = q->M1_open[10];//q->M10_open[0];
							q->M10_high[1] = zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))))))))); //q->M10_high[j - 1];
							q->M10_low[1] = zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))))))))); //q->M10_low[j - 1];
							q->M10_close[1] = q->M1_close[1]; //q->M10_close[j - 1];
							q->M10_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //M1_BS_Time[0];//q->M10_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M10_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M10_BS_PURE_Value[j - 1];
							q->M10_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M10_BS_BUY_Value[j - 1];
							q->M10_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M10_BS_SELL_Value[j - 1];
							q->M10_BS_BL_Value[1] = q->M10_BS_BUY_Value[1] / q->M10_BS_SELL_Value[1];   //q->M10_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M10_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M10_BS_BIG_PURE_Value[j - 1];
							q->M10_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M10_BS_BIG_BUY_Value[j - 1];
							q->M10_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M10_BS_BIG_SELL_Value[j - 1];
							q->M10_BS_BIG_BL_Value[1] = q->M10_BS_BIG_BUY_Value[1] / q->M10_BS_BIG_SELL_Value[1];// q->M10_BS_BIG_BL_Value[j - 1];
																												 //比M1增加的处理部分
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M10_BS_Time[%0.02f] q->M10_close[%0.02f] q->M10_BS_PURE_Value[%0.02f] q->M10_BS_BUY_Value[%0.02f] q->M10_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M10_BS_Time[id][1], q->M10_close[id][1], q->M10_BS_PURE_Value[id][1], q->M10_BS_BUY_Value[id][1], q->M10_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 10 || (tempm<0 && tempm >= -1430)) && CheckFirst(q->M1_BS_Time[1], 10))
						{

							NotFirst_10M = true;

						}
					}
					//M10更新


				}

				if (q->period_M15)
				{
					//printf("%s 更新M15\n", InstrumentID);
					//M15更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M15_BS_Time[1]);
					if (NotFirst_15M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M15_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M15_BS_Time[1]));
						if (tempm >= 15 || (tempm < 0 && tempm >= -1425))
						{
							for (int j = max_pure_m15_move_num; j > 1; j--) //2~59执行了
							{

								//if (M15_open[j - 1] > 1e-10 && M15_high[j - 1] > 1e-10 && M15_low[j - 1] > 1e-10  &&  M15_close[j - 1] > 1e-10 && M15_BS_Time[j - 1] > 1e-10 && M15_BS_BUY_Value[j - 1] > 1e-10 && M15_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M15_open[j - 1]) && CheckData(q->M15_high[j - 1]) && CheckData(q->M15_low[j - 1]) && CheckData(q->M15_close[j - 1]) && CheckDataOnlyNum(q->M15_BS_Time[j - 1]) && q->M15_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M15_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M15_BS_SELL_Value[j - 1])
									)

								{
									q->M15_open[j] = q->M15_open[j - 1];
									q->M15_high[j] = q->M15_high[j - 1];
									q->M15_low[j] = q->M15_low[j - 1];
									q->M15_close[j] = q->M15_close[j - 1];
									q->M15_BS_Time[j] = q->M15_BS_Time[j - 1];

									q->M15_BS_PURE_Value[j] = q->M15_BS_PURE_Value[j - 1];
									q->M15_BS_BUY_Value[j] = q->M15_BS_BUY_Value[j - 1];
									q->M15_BS_SELL_Value[j] = q->M15_BS_SELL_Value[j - 1];
									q->M15_BS_BL_Value[j] = q->M15_BS_BL_Value[j - 1];

									q->M15_BS_BIG_PURE_Value[j] = q->M15_BS_BIG_PURE_Value[j - 1];
									q->M15_BS_BIG_BUY_Value[j] = q->M15_BS_BIG_BUY_Value[j - 1];
									q->M15_BS_BIG_SELL_Value[j] = q->M15_BS_BIG_SELL_Value[j - 1];
									q->M15_BS_BIG_BL_Value[j] = q->M15_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}

							}

							//比M1增加的处理部分
							q->M15_open[1] = q->M1_open[15];//M15_open[0];

							q->M15_high[1] = zero_max(q->M1_high[15], zero_max(q->M1_high[14], zero_max(q->M1_high[13], zero_max(q->M1_high[12], zero_max(q->M1_high[11], zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1])))))))))))))); //q->M10_high[j - 1];
							q->M15_low[1] = zero_min(q->M1_low[15], zero_min(q->M1_low[14], zero_min(q->M1_low[13], zero_min(q->M1_low[12], zero_min(q->M1_low[11], zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])))))))))))))); //M15_low[j - 1];
							q->M15_close[1] = q->M1_close[1]; //q->M15_close[j - 1];
							q->M15_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //q->M1_BS_Time[0];//q->M15_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M15_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M15_BS_PURE_Value[j - 1];
							q->M15_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M15_BS_BUY_Value[j - 1];
							q->M15_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M15_BS_SELL_Value[j - 1];
							q->M15_BS_BL_Value[1] = q->M15_BS_BUY_Value[1] / q->M15_BS_SELL_Value[1];   //q->M15_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M15_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M15_BS_BIG_PURE_Value[j - 1];
							q->M15_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M15_BS_BIG_BUY_Value[j - 1];
							q->M15_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M15_BS_BIG_SELL_Value[j - 1];
							q->M15_BS_BIG_BL_Value[1] = q->M15_BS_BIG_BUY_Value[1] / q->M15_BS_BIG_SELL_Value[1];// q->M15_BS_BIG_BL_Value[j - 1];
																												 //比M1增加的处理部分
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M15_BS_Time[%0.02f] q->M15_close[%0.02f] q->M15_BS_PURE_Value[%0.02f] q->M15_BS_BUY_Value[%0.02f] q->M15_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M15_BS_Time[id][1], q->M15_close[id][1], q->M15_BS_PURE_Value[id][1], q->M15_BS_BUY_Value[id][1], q->M15_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 15 || (tempm<0 && tempm >= -1425)) && CheckFirst(q->M1_BS_Time[1], 15))
						{

							NotFirst_15M = true;

						}
					}
					//M15更新			

				}


				if (q->period_M30)
				{

					//M30更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M30_BS_Time[1]);
					if (NotFirst_30M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] q->M1_BS_Time[%0.06f] M30_BS_Time[%0.06f]\n", tempm, (q->M1_BS_Time[1]), (M30_BS_Time[1]));
						if (tempm >= 30 || (tempm < 0 && tempm >= -1410))
						{
							for (int j = max_pure_m30_move_num; j > 1; j--) //2~59执行了
							{
								//if (M30_open[j - 1] > 1e-10 && M30_high[j - 1] > 1e-10 && M30_low[j - 1] > 1e-10  &&  M30_close[j - 1] > 1e-10 && M30_BS_Time[j - 1] > 1e-10 && M30_BS_BUY_Value[j - 1] > 1e-10 && M30_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M30_open[j - 1]) && CheckData(q->M30_high[j - 1]) && CheckData(q->M30_low[j - 1]) && CheckData(q->M30_close[j - 1]) && CheckDataOnlyNum(q->M30_BS_Time[j - 1]) && q->M30_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M30_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M30_BS_SELL_Value[j - 1])
									)

								{
									q->M30_open[j] = q->M30_open[j - 1];
									q->M30_high[j] = q->M30_high[j - 1];
									q->M30_low[j] = q->M30_low[j - 1];
									q->M30_close[j] = q->M30_close[j - 1];
									q->M30_BS_Time[j] = q->M30_BS_Time[j - 1];

									q->M30_BS_PURE_Value[j] = q->M30_BS_PURE_Value[j - 1];
									q->M30_BS_BUY_Value[j] = q->M30_BS_BUY_Value[j - 1];
									q->M30_BS_SELL_Value[j] = q->M30_BS_SELL_Value[j - 1];
									q->M30_BS_BL_Value[j] = q->M30_BS_BL_Value[j - 1];

									q->M30_BS_BIG_PURE_Value[j] = q->M30_BS_BIG_PURE_Value[j - 1];
									q->M30_BS_BIG_BUY_Value[j] = q->M30_BS_BIG_BUY_Value[j - 1];
									q->M30_BS_BIG_SELL_Value[j] = q->M30_BS_BIG_SELL_Value[j - 1];
									q->M30_BS_BIG_BL_Value[j] = q->M30_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}
							}

							//比M1增加的处理部分
							q->M30_open[1] = q->M1_open[30];//M30_open[0];
															//M30_high[1] = max(q->M1_high[3], max(q->M1_high[2], q->M1_high[1])); //M30_high[j - 1];
															//M30_low[1] = zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])); //M30_low[j - 1];

							q->M30_high[1] = zero_max(q->M1_high[30], zero_max(q->M1_high[29], zero_max(q->M1_high[28], zero_max(q->M1_high[27], zero_max(q->M1_high[26], zero_max(q->M1_high[25], zero_max(q->M1_high[24], zero_max(q->M1_high[23], zero_max(q->M1_high[22], zero_max(q->M1_high[21], zero_max(q->M1_high[20], zero_max(q->M1_high[19], zero_max(q->M1_high[18], zero_max(q->M1_high[17], zero_max(q->M1_high[16], zero_max(q->M1_high[15], zero_max(q->M1_high[14], zero_max(q->M1_high[13], zero_max(q->M1_high[12], zero_max(q->M1_high[11], zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))))))))))))))))))))))))))))); //q->M15_high[j - 1];
							q->M30_low[1] = zero_min(q->M1_low[30], zero_min(q->M1_low[29], zero_min(q->M1_low[28], zero_min(q->M1_low[27], zero_min(q->M1_low[26], zero_min(q->M1_low[25], zero_min(q->M1_low[24], zero_min(q->M1_low[23], zero_min(q->M1_low[22], zero_min(q->M1_low[21], zero_min(q->M1_low[20], zero_min(q->M1_low[19], zero_min(q->M1_low[18], zero_min(q->M1_low[17], zero_min(q->M1_low[16], zero_min(q->M1_low[15], zero_min(q->M1_low[14], zero_min(q->M1_low[13], zero_min(q->M1_low[12], zero_min(q->M1_low[11], zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))))))))))))))))))))))))))))); //q->M15_low[j - 1];

							q->M30_close[1] = q->M1_close[1]; //q->M30_close[j - 1];
							q->M30_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //q->M1_BS_Time[0];//q->M30_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M30_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M30_BS_PURE_Value[j - 1];
							q->M30_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M30_BS_BUY_Value[j - 1];
							q->M30_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M30_BS_SELL_Value[j - 1];
							q->M30_BS_BL_Value[1] = q->M30_BS_BUY_Value[1] / q->M30_BS_SELL_Value[1];   //q->M30_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M30_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M30_BS_BIG_PURE_Value[j - 1];
							q->M30_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M30_BS_BIG_BUY_Value[j - 1];
							q->M30_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M30_BS_BIG_SELL_Value[j - 1];
							q->M30_BS_BIG_BL_Value[1] = q->M30_BS_BIG_BUY_Value[1] / q->M30_BS_BIG_SELL_Value[1];// q->M30_BS_BIG_BL_Value[j - 1];
																												 //比M1增加的处理部分
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M30_BS_Time[%0.02f] q->M30_close[%0.02f] q->M30_BS_PURE_Value[%0.02f] q->M30_BS_BUY_Value[%0.02f] q->M30_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M30_BS_Time[id][1], q->M30_close[id][1], q->M30_BS_PURE_Value[id][1], q->M30_BS_BUY_Value[id][1], q->M30_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 30 || (tempm<0 && tempm >= -1410)) && CheckFirst(q->M1_BS_Time[1], 30))
						{
							NotFirst_30M = true;
						}
					}
					//M30更新			

				}

				if (q->period_M60)
				{

					//M60更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M60_BS_Time[1]);
					if (NotFirst_60M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] q->M1_BS_Time[%0.06f] q->M60_BS_Time[%0.06f]\n", tempm, (q->M1_BS_Time[1]), (q->M60_BS_Time[1]));
						if (tempm >= 60 || (tempm < 0 && tempm >= -1380))
						{
							for (int j = max_pure_m60_move_num; j > 1; j--) //2~59执行了
							{
								//if (M60_open[j - 1] > 1e-10 && M60_high[j - 1] > 1e-10 && M60_low[j - 1] > 1e-10  &&  M60_close[j - 1] > 1e-10 && M60_BS_Time[j - 1] > 1e-10 && M60_BS_BUY_Value[j - 1] > 1e-10 && M60_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M60_open[j - 1]) && CheckData(q->M60_high[j - 1]) && CheckData(q->M60_low[j - 1]) && CheckData(q->M60_close[j - 1]) && CheckDataOnlyNum(q->M60_BS_Time[j - 1]) && q->M60_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M60_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M60_BS_SELL_Value[j - 1])
									)

								{
									q->M60_open[j] = q->M60_open[j - 1];
									q->M60_high[j] = q->M60_high[j - 1];
									q->M60_low[j] = q->M60_low[j - 1];
									q->M60_close[j] = q->M60_close[j - 1];
									q->M60_BS_Time[j] = q->M60_BS_Time[j - 1];

									q->M60_BS_PURE_Value[j] = q->M60_BS_PURE_Value[j - 1];
									q->M60_BS_BUY_Value[j] = q->M60_BS_BUY_Value[j - 1];
									q->M60_BS_SELL_Value[j] = q->M60_BS_SELL_Value[j - 1];
									q->M60_BS_BL_Value[j] = q->M60_BS_BL_Value[j - 1];

									q->M60_BS_BIG_PURE_Value[j] = q->M60_BS_BIG_PURE_Value[j - 1];
									q->M60_BS_BIG_BUY_Value[j] = q->M60_BS_BIG_BUY_Value[j - 1];
									q->M60_BS_BIG_SELL_Value[j] = q->M60_BS_BIG_SELL_Value[j - 1];
									q->M60_BS_BIG_BL_Value[j] = q->M60_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}
							}

							//比M1增加的处理部分
							q->M60_open[1] = q->M1_open[60];//q->M60_open[0];	
							q->M60_high[1] = zero_max(q->M1_high[60],
								zero_max(q->M1_high[59],
									zero_max(q->M1_high[58],
										zero_max(q->M1_high[57],
											zero_max(q->M1_high[56],
												zero_max(q->M1_high[55],
													zero_max(q->M1_high[54],
														zero_max(q->M1_high[53],
															zero_max(q->M1_high[52],
																zero_max(q->M1_high[51],
																	zero_max(q->M1_high[50],
																		zero_max(q->M1_high[49],
																			zero_max(q->M1_high[48],
																				zero_max(q->M1_high[47],
																					zero_max(q->M1_high[46],
																						zero_max(q->M1_high[45],
																							zero_max(q->M1_high[44],
																								zero_max(q->M1_high[43],
																									zero_max(q->M1_high[42],
																										zero_max(q->M1_high[41],
																											zero_max(q->M1_high[40],
																												zero_max(q->M1_high[39],
																													zero_max(q->M1_high[38],
																														zero_max(q->M1_high[37],
																															zero_max(q->M1_high[36],
																																zero_max(q->M1_high[35],
																																	zero_max(q->M1_high[34],
																																		zero_max(q->M1_high[33],
																																			zero_max(q->M1_high[32],
																																				zero_max(q->M1_high[31],
																																					q->M30_high[1]))))))))))))))))))))))))))))));


							q->M60_low[1] = zero_min(q->M1_low[60], zero_min(q->M1_low[59], zero_min(q->M1_low[58], zero_min(q->M1_low[57], zero_min(q->M1_low[56], zero_min(q->M1_low[55], zero_min(q->M1_low[54], zero_min(q->M1_low[53], zero_min(q->M1_low[52], zero_min(q->M1_low[51], zero_min(q->M1_low[50], zero_min(q->M1_low[49], zero_min(q->M1_low[48], zero_min(q->M1_low[47], zero_min(q->M1_low[46], zero_min(q->M1_low[45], zero_min(q->M1_low[44], zero_min(q->M1_low[43], zero_min(q->M1_low[42], zero_min(q->M1_low[41], zero_min(q->M1_low[40], zero_min(q->M1_low[39], zero_min(q->M1_low[38], zero_min(q->M1_low[37], zero_min(q->M1_low[36], zero_min(q->M1_low[35], zero_min(q->M1_low[34], zero_min(q->M1_low[33], zero_min(q->M1_low[32], zero_min(q->M1_low[31], q->M30_high[1])))))))))))))))))))))))))))))); //M15_low[j - 1];



							q->M60_close[1] = q->M1_close[1];
							q->M60_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //q->M1_BS_Time[0];//q->M60_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M60_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M60_BS_PURE_Value[j - 1];
							q->M60_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M60_BS_BUY_Value[j - 1];
							q->M60_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M60_BS_SELL_Value[j - 1];
							q->M60_BS_BL_Value[1] = q->M60_BS_BUY_Value[1] / q->M60_BS_SELL_Value[1];   //q->M60_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M60_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M60_BS_BIG_PURE_Value[j - 1];
							q->M60_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M60_BS_BIG_BUY_Value[j - 1];
							q->M60_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M60_BS_BIG_SELL_Value[j - 1];
							q->M60_BS_BIG_BL_Value[1] = q->M60_BS_BIG_BUY_Value[1] / q->M60_BS_BIG_SELL_Value[1];// q->M60_BS_BIG_BL_Value[j - 1];
																												 //比M1增加的处理部分
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M60_BS_Time[%0.02f] q->M60_close[%0.02f] q->M60_BS_PURE_Value[%0.02f] q->M60_BS_BUY_Value[%0.02f] q->M60_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M60_BS_Time[id][1], q->M60_close[id][1], q->M60_BS_PURE_Value[id][1], q->M60_BS_BUY_Value[id][1], q->M60_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 60 || (tempm<0 && tempm >= -1380)) && CheckFirst(q->M1_BS_Time[1], 60))
						{
							NotFirst_60M = true;
						}
					}
					//M60更新		

				}






				//计算1M走势相关性,建议开一个新线程
				//if (tempdata == true)
				//{
				//恢复printf("\n计算相关性\n");
				//for (int k = 0;k<TYPE_NUM;k++)
				//	{
				//	CalRelativity(i, k);
				//	}

				//}

				//计算1M走势相关性，建议开一个新线程    

				//这个是否在挪位之前更新

				
				//printf("M1 pDepthMarketData->LastPrice[%0.02f][%0.02f][%0.02f][%0.02f]\n",q->M1_open[0],q->M1_high[0],q->M1_low[0], q->M1_close[0]);
				q->M1_open[0] = pDepthMarketData->LastPrice;
				q->M1_high[0] = pDepthMarketData->LastPrice;
				q->M1_low[0] = pDepthMarketData->LastPrice;
				q->M1_close[0] = pDepthMarketData->LastPrice;	//是否和前面每次TICK都=0重复了	，是否可以删除前面	=0赋值
																


																//TOTAL部分只累计，不清空置0


				q->M1_BS_Time[0] = q->tick_BS_Time[0];
				//每分钟之更新1次

				for (int j = 0; j < MAX_PURE_TICK_PERMIN_NUM; j++)
				{


					//printf("A3\n");
					//double temptime=tick_BS_Time[j];
					//double temptime2=tick_BS_Time[j+1];
					//四舍五入
					//cout.setf(ios::fixed);
					// cout.setf(ios::showpoint);
					//cout.precision(4);

					//cout<<temptime;
					//cout<<temptime2;
					//四舍五入
					//if(tick_BS_Time[j]>0)
					////printf("时间啊B%d[%.06f][%.06f]\n",j,tick_BS_Time[j], ((double)(tick_BS_Time[j])).precision(2));
					//printf("时间啊XXXX______________   %d[%.06f]_[%.06f]\n",j,tick_BS_Time[j],temptime);

					//if(temptime-temptime2>=0.0001 && (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //一定有时间数，但是要排除00:00点
					// if(  (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //一定有时间数，但是要排除00:00点
					// {

					q->M1_BS_PURE_Value[0] = q->M1_BS_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_BUY_Value[0] = q->M1_BS_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//逻辑是否有问题
					q->M1_BS_SELL_Value[0] = q->M1_BS_SELL_Value[0] + q->tick_BS_SELL_Value[j];

					//bl,middle在哪里？
					q->M1_BS_TOTAL_PURE_Value[0] = q->M1_BS_TOTAL_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_TOTAL_BUY_Value[0] = q->M1_BS_TOTAL_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//逻辑是否有问题
					q->M1_BS_TOTAL_SELL_Value[0] = q->M1_BS_TOTAL_SELL_Value[0] + q->tick_BS_SELL_Value[j];

					q->M1_BS_CROSS_TOTAL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_CROSS_TOTAL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//逻辑是否有问题
					q->M1_BS_CROSS_TOTAL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SELL_Value[0] + q->tick_BS_SELL_Value[j];


					//TOTAL 还是CROSS?
					//小单





					double tb = q->tick_BS_BUY_Value[j];
					double ts = q->tick_BS_SELL_Value[j];


					if (tb < CAL_MIDDLE_AMOUNT)
					{
						q->M1_BS_SMALL_BUY_Value[0] = q->M1_BS_SMALL_BUY_Value[0] + tb;
						q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_TOTAL_SMALL_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] + tb;  //检查


						q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] + tb;  //检查
					}

					//中单
					else if (tb >= CAL_MIDDLE_AMOUNT && tb < CAL_BIG_AMOUNT)
					{
						// printf("[%d]中单 %0.02f\n", j, tb);
						q->M1_BS_MIDDLE_BUY_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //检查

						q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //检查
					}

					else// if (tb >= CAL_BIG_AMOUNT)
					{
						//printf("[%d]大单 %0.02f\n", j, tb);
						q->M1_BS_BIG_BUY_Value[0] = q->M1_BS_BIG_BUY_Value[0] + tb;
						q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_TOTAL_BIG_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] + tb;  //检查

						q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] + tb;  //检查
					}



					if (ts < CAL_MIDDLE_AMOUNT)
					{
						q->M1_BS_SMALL_SELL_Value[0] = q->M1_BS_SMALL_SELL_Value[0] + ts;
						q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_TOTAL_SMALL_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] - ts;  //检查

						q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] - ts;  //检查
					}
					else if (ts >= CAL_MIDDLE_AMOUNT && ts < CAL_BIG_AMOUNT)
					{
						q->M1_BS_MIDDLE_SELL_Value[0] = q->M1_BS_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //检查

						q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //检查
					}
					else// if (ts >= CAL_BIG_AMOUNT)
					{
						q->M1_BS_BIG_SELL_Value[0] = q->M1_BS_BIG_SELL_Value[0] + ts;
						q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_TOTAL_BIG_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] - ts;  //检查

						q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] - ts;  //检查
					}


					if (q->M1_BS_SELL_Value[0] > 0)
					{
						q->M1_BS_BL_Value[0] = q->M1_BS_BUY_Value[0] / q->M1_BS_SELL_Value[0];
					}
					else
					{
						q->M1_BS_BL_Value[0] = 0;
					}

					if (q->M1_BS_SMALL_SELL_Value[0] > 0)
					{
						q->M1_BS_SMALL_BL_Value[0] = q->M1_BS_SMALL_BUY_Value[0] / q->M1_BS_SMALL_SELL_Value[0];
					}
					else
					{
						q->M1_BS_SMALL_BL_Value[0] = 0;
					}


					if (q->M1_BS_MIDDLE_SELL_Value[0] > 0)
					{
						q->M1_BS_MIDDLE_BL_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] / q->M1_BS_MIDDLE_SELL_Value[0];
					}
					else
					{
						q->M1_BS_MIDDLE_BL_Value[0] = 0;
					}


					if (q->M1_BS_BIG_SELL_Value[0] > 0)
					{
						q->M1_BS_BIG_BL_Value[0] = q->M1_BS_BIG_BUY_Value[0] / q->M1_BS_BIG_SELL_Value[0];
					}
					else
					{
						q->M1_BS_BIG_BL_Value[0] = 0;
					}


					//for (int k = 0; k<5; k++)
					//{
					//int id = i;
					//if(q->M1_BS_BUY_Value[id][k]>1e-10)
					// {
					//printf("当前时间：[%.06f]\n", tick_data[2]);
					//printf("\n__%d时间：[%.06f][%d][%s]tb[%0.02f]ts[%0.02f]净买[%0.02f]买[%0.02f]卖[%0.02f]比例[%0.02f]小净[%0.02f]小买[%0.02f]小卖[%0.02f]小单比例[%0.02f]中净[%0.02f]中买[%0.02f]中卖[%0.02f]中单比例[%0.02f]大净[%0.02f]大买[%0.02f]大卖[%0.02f]大单比例[%0.02f]\n",


					//}
					//printf("------------------\n");




				}






				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3更新


				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3更新




				//打印K线线数据
				//	 cerr << "--->>> K:" << InstrumentID_n << "_" <<dbtoch(tick_data[1])<< "_" << tick_data[2]<< "_" << q->M1_open[1]<< "_" << q->M1_high[1]<< "_" << q->M1_low[1]<< "_" << q->M1_close[1] << endl;
			}
			else
			{
				// printf("\n\n\n_______________________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx__________________________kaishi \n\n\n");
				//更新最后一根K线？自己添加理解
				if (q->M1_open[0] == 0) { q->M1_open[0] = pDepthMarketData->LastPrice; }

				q->M1_high[0] = max(q->M1_high[0], pDepthMarketData->LastPrice);
				q->M1_low[0] = zero_min(q->M1_low[0], pDepthMarketData->LastPrice);
				q->M1_close[0] = pDepthMarketData->LastPrice;

				//q->M1_BS_PURE_Value[1]=q->M1_BS_PURE_Value[0];
				//q->M1_BS_PURE_Value[0]=NewVolume;

				//q->M1_BS_PURE_Value[0]=tick_BS_PURE_Value[0]+tick_BS_PURE_Value[1]+tick_BS_PURE_Value[2];
				//q->M1_BS_BUY_Value[0]=tick_BS_BUY_Value[0]+tick_BS_BUY_Value[1]+tick_BS_BUY_Value[2];
				//q->M1_BS_SELL_Value[0]=tick_BS_SELL_Value[0]+tick_BS_SELL_Value[1]+tick_BS_SELL_Value[2];

				//TICK过滤后，每秒1次TICK，1分钟应该改为60比TICK。每秒2次TICK，应该位120比
				//q->M1_BS_PURE_Value[0]=0;
				//q->M1_BS_BUY_Value[0]=0;
				//q->M1_BS_SELL_Value[0]=0;
				//printf("BBBBB \n");
				// printf("下一分钟\n");
				//for(int j=0;j<MAX_PURE_TICK_PERMIN_NUM;j++)
				//{



				// cout<<temptime;
				//cout<<temptime2;
				//四舍五入
				//if(tick_BS_Time[j]>0)
				////printf("时间啊B%d[%.06f][%.06f]\n",j,tick_BS_Time[j], ((double)(tick_BS_Time[j])).precision(2));
				//printf("时间啊B   %d[%.06f]_[%.06f]\n",j,tick_BS_Time[j],temptime);
				//printf("CCCCC \n");

				//  if(temptime-temptime2>=0.0001 && (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //一定有时间数，但是要排除00:00点
				// if((tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //一定有时间数，但是要排除00:00点

				//if(tick_BS_Time[j]>0)
				// printf("时间差[%.06f][%.06f]_[%.06f]\n",tick_BS_Time[j],tick_BS_Time[j+1],temptime-temptime2);
				//if(tick_BS_Time[j]>0)
				//printf("时间差TICK[%.06f]M1[%.06f]_M2[%.06f]\n",tick_BS_Time[j],tempq->M1_A,tempq->M1_B);

				//if(tick_BS_Time[j]-tempq->M1_A>=0.000001 && (tempq->M1_A>0 || tempq->M1_B>0))
				//if()
				//{
				//if(j==0)
				double tb = q->tick_BS_BUY_Value[0];
				double ts = q->tick_BS_SELL_Value[0];



				q->M1_BS_PURE_Value[0] = q->M1_BS_PURE_Value[0] + q->tick_BS_PURE_Value[0];
				q->M1_BS_BUY_Value[0] = q->M1_BS_BUY_Value[0] + tb;
				q->M1_BS_SELL_Value[0] = q->M1_BS_SELL_Value[0] + ts;


				if (tb < CAL_MIDDLE_AMOUNT)
				{
					q->M1_BS_SMALL_BUY_Value[0] = q->M1_BS_SMALL_BUY_Value[0] + tb;
					q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_TOTAL_SMALL_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] + tb;  //检查

					q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] + tb;  //检查
				}

				//中单
				else if (tb >= CAL_MIDDLE_AMOUNT && tb < CAL_BIG_AMOUNT)
				{
					// printf("[%d]中单 %0.02f\n", j, tb);
					q->M1_BS_MIDDLE_BUY_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //检查

					q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //检查
				}

				else// if (tb >= CAL_BIG_AMOUNT)
				{
					//printf("[%d]大单 %0.02f\n", j, tb);
					q->M1_BS_BIG_BUY_Value[0] = q->M1_BS_BIG_BUY_Value[0] + tb;
					q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_TOTAL_BIG_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] + tb;  //检查

					q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] + tb;  //检查
				}

				if (ts < CAL_MIDDLE_AMOUNT)
				{
					q->M1_BS_SMALL_SELL_Value[0] = q->M1_BS_SMALL_SELL_Value[0] + ts;
					q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_TOTAL_SMALL_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] - ts;  //检查

					q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] + ts;
					q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] - ts;  //检查
				}
				else if (ts >= CAL_MIDDLE_AMOUNT && ts < CAL_BIG_AMOUNT)
				{
					q->M1_BS_MIDDLE_SELL_Value[0] = q->M1_BS_MIDDLE_SELL_Value[0] + ts;
					q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //检查
				}
				else// if (ts >= CAL_BIG_AMOUNT)
				{
					q->M1_BS_BIG_SELL_Value[0] = q->M1_BS_BIG_SELL_Value[0] + ts;
					q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_TOTAL_BIG_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] - ts;  //检查
				}






				//printf("1分钟净买B[%d]主买[%d]主卖[%d]主买比主卖[%.02f]\n",(int)M1_BS_PURE_Value[0],(int)M1_BS_BUY_Value[0],(int)M1_BS_SELL_Value[0],MMBL);
				//M1_BS_PURE_Value[0]=Plus(M1_BS_PURE_Value[0],tick_BS_PURE_Value[0]);
				//M1_BS_BL_Value[0]=Ave(M1_BS_BL_Value[0],M1_BS_BL_Value[1],M1_BS_BL_Value[2],M1_BS_BL_Value[3],M1_BS_BL_Value[4],M1_BS_BL_Value[5],M1_BS_BL_Value[6],M1_BS_BL_Value[7],M1_BS_BL_Value[8],M1_BS_BL_Value[9]);
				




				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3更新

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3更新
				if (q->TickFileWritepaths[0] == '\0')
				{
					q->tick_data[8] = pDepthMarketData->UpperLimitPrice;
					q->tick_data[9] = pDepthMarketData->LowerLimitPrice;

					strcpy(q->TickFileWritepaths, "./TickData/");
					strcat(q->TickFileWritepaths, pDepthMarketData->InstrumentID);
					strcat(q->TickFileWritepaths, "_");
					//恢复strcat_s(q->TickFileWritepaths, TradingDay);
					strcat(q->TickFileWritepaths, ".txt");

					//检查文件是否存在，是否需要新建文本文件
					ifstream inf;
					ofstream ouf;
					inf.open(q->TickFileWritepaths, ios::out);


					/////////////////////////////////////////////////
					//char str[200] = { 0 };
					//strcat_s(str, 200, "SingleData/TradeRecord.txt");

					//检查文件是否存在，是否需要新建文本文件
					//	ifstream inf;
					//ofstream ouf;
					//inf.open(TickFileWritepaths[i], ios::out);
					//inf.open(str, ios::out);
					//}

					//记录TICK数据
					//ofstream o_file(str, ios::app);
					//if ( RunMode && ( check0 || check1 || check2) )

					//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

					//o_file << content << "\t" << endl;

					//else
					//{
					//o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //将内容写入到文本文件中
					//}
					//o_file.close();						//关闭文件




				}

				//记录TICK数据
				ofstream o_file(q->TickFileWritepaths, ios::app);

				if (check1 || check2 || check3 || check4 || check5 || check6 || check7)
				{
					o_file << dbtoch(q->tick_data[1]) << "\t" << q->tick_data[2] << "\t" << pDepthMarketData->UpdateMillisec << "\t" << q->tick_AskPrice1[0] << "\t" << q->tick_AskVolume1[0] << "\t" << q->tick_BidPrice1[0] << "\t" << q->tick_BidVolume1[0] << "\t" << q->tick_data[4] << "\t" << dbtoch(q->tick_Volume[0]) << "\t" << dbtoch(q->tick_OpenInterest[0]) << "\t" << q->Day_open[2] << "\t" << q->Day_high[2] << "\t" << q->Day_low[2] << "\t" << q->Day_close[2] << "\t" << q->Day_open[1] << "\t" << q->Day_high[1] << "\t" << q->Day_low[1] << "\t" << q->Day_close[1] << "\t" << q->tick_data[8] << "\t" << q->tick_data[9] << endl;
				}
				else
				{
					o_file << dbtoch(q->tick_data[1]) << "\t" << q->tick_data[2] << "\t" << pDepthMarketData->UpdateMillisec << "\t" << q->tick_AskPrice1[0] << "\t" << q->tick_AskVolume1[0] << "\t" << q->tick_BidPrice1[0] << "\t" << q->tick_BidVolume1[0] << "\t" << q->tick_data[4] << "\t" << dbtoch(q->tick_Volume[0]) << "\t" << dbtoch(q->tick_OpenInterest[0]) << endl; //将内容写入到文本文件中
				}
				o_file.close();						//关闭文件


				if (q->tick_data[2]>0.145950 && (q->tick_Volume[0] - q->tick_Volume[1])<0.01 && (q->tick_OpenInterest[0] - q->tick_OpenInterest[1])<0.01)// && LastTick[i]==false)
				{
					//LastTick[i]=true;

					q->Day_open[0] = pDepthMarketData->OpenPrice;
					q->Day_high[0] = pDepthMarketData->HighestPrice;
					q->Day_low[0] = pDepthMarketData->LowestPrice;
					q->Day_close[0] = pDepthMarketData->LastPrice;
					//cerr << "--->>> " <<"WriteConfiguration!" << endl;
					//WriteConfiguration("./AutoTrader.dat");
				}

				bool FistCheck1 = (q->TYPEID == TYPE_PP) || (q->TYPEID == TYPE_CS) || (q->TYPEID == TYPE_JD) || (q->TYPEID == TYPE_L) || (q->TYPEID == TYPE_V) || (q->TYPEID == TYPE_C) || (q->TYPEID == TYPE_WH) || (q->TYPEID == TYPE_SM) || (q->TYPEID == TYPE_SF);
				//无夜盘
				bool FistCheck2 = (!FistCheck1) && (!typecheck1) && (!typecheck2);
				//有夜盘
				//老if ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i<=17))
				//    if ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i <= 17) || (tick_data[i][2]<0.0230 && i <= 17))  //修复9

				if (seconds>45 && seconds<55 && q->M1Klinesig == true)
				{
					//printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx转换xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n\n");
					q->M1Klinesig = false;
				}



				//检查修正                                                                                //无夜盘                                               //有夜盘
				if ((q->tick_data[2]>0.0858 && typecheck1) || (q->tick_data[2]>0.0925 && typecheck2) || (q->tick_data[2]>0.0858 && FistCheck1) || ((q->tick_data[2]>0.2058 || q->tick_data[2]<0.1530) && FistCheck2))  //修复9
				{
					q->FristTick = true;
				}

				q->ReceiveTick = false;
				q->tick_data[0] = 1;		//设置标志位

			}


		}

	}
}

void ClearGuestOnlineHash(GuestOnlineHash * q)
{
	delete[]q->M1_BS_CROSS_TOTAL_BIG_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


	delete[]q->M1_BS_CROSS_TOTAL_BIG_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_BIG_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_BIG_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	delete[]q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_MIDDLE_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	delete[]q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_SMALL_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


	delete[]q->M1_BS_CROSS_TOTAL_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_CROSS_TOTAL_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
									   //CROSS的统计

									   //double	tick_data[10];				//TICK基本数据
									   //double	tick_AskPrice1[MAX_TICK_NUM];			//买一价
									   //double	tick_BidPrice1[MAX_TICK_NUM];			//卖一价
									   //double	tick_AskVolume1[MAX_TICK_NUM];		//买一量
									   //double	tick_BidVolume1[MAX_TICK_NUM];		//卖一量
									   //double	tick_Volume[MAX_TICK_NUM];			//成交量
									   //double	tick_OpenInterest[MAX_TICK_NUM];		//持仓量



	delete[]q->tick_BS_BIG_BUY_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->tick_BS_BIG_SELL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->tick_BS_BIG_PURE_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量

	delete[]q->tick_BS_BUY_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->tick_BS_SELL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->tick_BS_PURE_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量


							   // double	tick_BS_BUY_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
							   // double	tick_BS_SELL_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->tick_BS_Time;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量

	delete[]q->tick_BS_BL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M1_BS_TOTAL_BIG_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_BIG_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_BIG_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_BIG_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	delete[]q->M1_BS_TOTAL_MIDDLE_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_MIDDLE_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_MIDDLE_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_MIDDLE_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	delete[]q->M1_BS_TOTAL_SMALL_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_SMALL_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_SMALL_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_SMALL_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


	delete[]q->M1_BS_TOTAL_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	delete[]q->M1_BS_TOTAL_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	delete[]q->M1_open;// = new double[max_pure_m1_num];	//分钟K线开
	delete[]q->M1_high;// = new double[max_pure_m1_num];	//分钟K线高
	delete[]q->M1_low;// = new double[max_pure_m1_num];		//分钟K线低
	delete[]q->M1_close;// = new double[max_pure_m1_num];	//分钟K线收
					 //bool	M1Klinesig;		//1分钟K线标志
	delete[]q->M1_BS_Time;// = new double[max_pure_m1_num];


	delete[]q->M1_BS_BIG_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_BIG_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_BIG_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_BIG_BL_Value; //= new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M1_BS_MIDDLE_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_MIDDLE_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_MIDDLE_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_MIDDLE_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M1_BS_SMALL_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_SMALL_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_SMALL_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_SMALL_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M1_BS_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M1_BS_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例




						   //3分钟
	delete[]q->M3_open;// = new double[max_pure_m3_num];	//分钟K线开
	delete[]q->M3_high;// = new double[max_pure_m3_num];	//分钟K线高
	delete[]q->M3_low;//= new double[max_pure_m3_num];		//分钟K线低
	delete[]q->M3_close;// = new double[max_pure_m3_num];	//分钟K线收
					 //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->M3_BS_Time;// = new double[max_pure_m3_num];
					   // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
					   // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
					   // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
					   // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	delete[]q->M3_BS_BIG_PURE_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M3_BS_BIG_BUY_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M3_BS_BIG_SELL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M3_BS_BIG_BL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M3_BS_BUY_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M3_BS_SELL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M3_BS_PURE_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M3_BS_BL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
						   //3分钟


						   //5分钟
	delete[]q->M5_open;// = new double[max_pure_m5_num];	//分钟K线开
	delete[]q->M5_high;// = new double[max_pure_m5_num];	//分钟K线高
	delete[]q->M5_low;// = new double[max_pure_m5_num];		//分钟K线低
	delete[]q->M5_close;// = new double[max_pure_m5_num];	//分钟K线收
					 //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->M5_BS_Time;// = new double[max_pure_m5_num];
					   // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
					   // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
					   // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
					   // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	delete[]q->M5_BS_BIG_PURE_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M5_BS_BIG_BUY_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M5_BS_BIG_SELL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M5_BS_BIG_BL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M5_BS_BUY_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M5_BS_SELL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M5_BS_PURE_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M5_BS_BL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
						   //5分钟


						   //10分钟
	delete[]q->M10_open;// = ; new double[max_pure_m10_num];	//分钟K线开
	delete[]q->M10_high;// = new double[max_pure_m10_num];	//分钟K线高
	delete[]q->M10_low;// = new double[max_pure_m10_num];		//分钟K线低
	delete[]q->M10_close;// = new double[max_pure_m10_num];	//分钟K线收
					  //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->M10_BS_Time;// = new double[max_pure_m10_num];
						// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	delete[]q->M10_BS_BIG_PURE_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M10_BS_BIG_BUY_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M10_BS_BIG_SELL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M10_BS_BIG_BL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M10_BS_BUY_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M10_BS_SELL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M10_BS_PURE_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M10_BS_BL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
							//10分钟

							//15分钟
	delete[]q->M15_open;// = new double[max_pure_m15_num];	//分钟K线开
	delete[]q->M15_high;// = new double[max_pure_m15_num];	//分钟K线高
	delete[]q->M15_low;// = new double[max_pure_m15_num];		//分钟K线低
	delete[]q->M15_close;// = new double[max_pure_m15_num];	//分钟K线收
					  //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->M15_BS_Time;// = new double[max_pure_m15_num];
						// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	delete[]q->M15_BS_BIG_PURE_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M15_BS_BIG_BUY_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M15_BS_BIG_SELL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M15_BS_BIG_BL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M15_BS_BUY_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M15_BS_SELL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M15_BS_PURE_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M15_BS_BL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
							//15分钟



							//30分钟
	delete[]q->M30_open;// = new double[max_pure_m30_num];	//分钟K线开
	delete[]q->M30_high;// = new double[max_pure_m30_num];	//分钟K线高
	delete[]q->M30_low;// = new double[max_pure_m30_num];		//分钟K线低
	delete[]q->M30_close;// = new double[max_pure_m30_num];	//分钟K线收
					  //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->M30_BS_Time;// = new double[max_pure_m30_num];
						// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	delete[]q->M30_BS_BIG_PURE_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M30_BS_BIG_BUY_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M30_BS_BIG_SELL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M30_BS_BIG_BL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M30_BS_BUY_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M30_BS_SELL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M30_BS_PURE_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M30_BS_BL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
							//30分钟


							//60分钟
	delete[]q->M60_open;// = new double[max_pure_m60_num];	//分钟K线开
	delete[]q->M60_high;// = new double[max_pure_m60_num];	//分钟K线高
	delete[]q->M60_low;// = new double[max_pure_m60_num];		//分钟K线低
	delete[]q->M60_close;// = new double[max_pure_m60_num];	//分钟K线收
					  //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->M60_BS_Time;// = new double[max_pure_m60_num];
						// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
						// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	delete[]q->M60_BS_BIG_PURE_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M60_BS_BIG_BUY_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M60_BS_BIG_SELL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M60_BS_BIG_BL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例


	delete[]q->M60_BS_BUY_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M60_BS_SELL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M60_BS_PURE_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	delete[]q->M60_BS_BL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例								
							//60分钟


							//120分钟
	delete[]q->M120_open;// = new double[max_pure_m120_num];	//分钟K线开
	delete[]q->M120_high;// = new double[max_pure_m120_num];	//分钟K线高
	delete[]q->M120_low;// = new double[max_pure_m120_num];		//分钟K线低
	delete[]q->M120_close;// = new double[max_pure_m120_num];	//分钟K线收
					   //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->M120_BS_Time;// = new double[max_pure_m120_num];
						 // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
						 // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
						 // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
						 // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	delete[]q->M120_BS_BIG_PURE_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M120_BS_BIG_BUY_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M120_BS_BIG_SELL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M120_BS_BIG_BL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例


	delete[]q->M120_BS_BUY_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M120_BS_SELL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M120_BS_PURE_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M120_BS_BL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例								
							 //120分钟

							 //240分钟
	delete[]q->M240_open;// = new double[max_pure_m240_num];	//分钟K线开
	delete[]q->M240_high;// = new double[max_pure_m240_num];	//分钟K线高
	delete[]q->M240_low;// = new double[max_pure_m240_num];		//分钟K线低
	delete[]q->M240_close;// = new double[max_pure_m240_num];	//分钟K线收
					   //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->M240_BS_Time;// = new double[max_pure_m240_num];
						 // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
						 // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
						 // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
						 // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	delete[]q->M240_BS_BIG_PURE_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M240_BS_BIG_BUY_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M240_BS_BIG_SELL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M240_BS_BIG_BL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例


	delete[]q->M240_BS_BUY_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M240_BS_SELL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M240_BS_PURE_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->M240_BS_BL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例								
							 //240分钟


							 //240分钟
	delete[]q->D1_open;// = new double[max_pure_d1_num];	//分钟K线开
	delete[]q->D1_high;// = new double[max_pure_d1_num];	//分钟K线高
	delete[]q->D1_low;// = new double[max_pure_d1_num];		//分钟K线低
	delete[]q->D1_close;// = new double[max_pure_d1_num];	//分钟K线收
					 //	bool	M3Klinesig[20];		//3分钟K线标志
	delete[]q->D1_BS_Time;// = new double[max_pure_d1_num];
					   // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
					   // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
					   // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
					   // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	delete[]q->D1_BS_BIG_PURE_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->D1_BS_BIG_BUY_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->D1_BS_BIG_SELL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->D1_BS_BIG_BL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例


	delete[]q->D1_BS_BUY_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->D1_BS_SELL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->D1_BS_PURE_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	delete[]q->D1_BS_BL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例								
						   //240分钟


}

void InitGuestOnlineHash(GuestOnlineHash * q)
{
	q->key = 0;
	memset(&(q->InstrumentID), 0, strlen(q->InstrumentID));
	q->M1Klinesig = false;
	q->TYPEID = 0;
	//M=0;
	//N=0;
	q->avespace = 0;
	q->tradenum = 0;

	q->ReceiveTick = false;
	q->FristTick = false;
	q->LastTick = false;



	//LastTick = false;
	memset(q->TickFileWritepaths, 0, sizeof(q->TickFileWritepaths));
	q->sort = 0;//space/平均时间排名

			 //dwLastActiveTime=0;			 // 记录此用户的活动时间（服务器使用）
			 //LoginErrorTime=0;
			 //LoginErrorNum=0;
			 //char keystr[MAX_USERNAME];	 // 用户名

	memset(&(q->keystr), 0, MAX_USERNAME);
	//memset(&checkcod,0,sizeof(checkcod));
	//DWORD ip[30];  //6个字符+1位\0
	//u_short port[6];  //6个字符+1位\0

	q->EmaCount = 0;
	//si=0;     //查找长度
	//PEER_RECORD_INFO deallist;
	//ADDR_RADER_INFO addr[MAX_ADDR_NUMBER]; // 由节点的私有终端和公共终端组成的数组
	//u_char AddrNum;					 // addr数组元素数量
	//ADDR_RADER_INFO p2pAddr;				 // P2P通信时应使用的地址（客户方使用）
	//char oncehash[7];
	//memset(tick_data, 0, sizeof(tick_data));
	//enum  entry_status states;

	//PositionAmount[TYPE_NUM]= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //持仓方向，数量
	memset(q->PositionAmount, 0, sizeof(q->PositionAmount));

	memset(q->LastTurn, 0, sizeof(q->LastTurn));

	memset(q->LastTurnTime, 0, sizeof(q->LastTurnTime));

	memset(q->LastCount, 0, sizeof(q->LastCount));

	memset(q->LastStopTime, 0, sizeof(q->LastStopTime));

	// memset(CROSS_SINGLE_TURN, 0, sizeof(CROSS_SINGLE_TURN));
	//memset(CROSS_SINGLE_SELECT, 0, sizeof(CROSS_SINGLE_SELECT));
	// memset(CROSS_SINGLE_TIME, 0, sizeof(CROSS_SINGLE_TIME));

	memset(q->tick_data, 0, sizeof(q->tick_data));
	memset(q->tick_AskPrice1, 0, sizeof(q->tick_AskPrice1));
	memset(q->tick_BidPrice1, 0, sizeof(q->tick_BidPrice1));
	memset(q->tick_AskVolume1, 0, sizeof(q->tick_AskVolume1));
	memset(q->tick_BidVolume1, 0, sizeof(q->tick_BidVolume1));
	memset(q->tick_Volume, 0, sizeof(q->tick_Volume));
	memset(q->tick_OpenInterest, 0, sizeof(q->tick_OpenInterest));

	memset(q->Day_open, 0, sizeof(q->Day_open));
	memset(q->Day_high, 0, sizeof(q->Day_high));
	memset(q->Day_low, 0, sizeof(q->Day_low));
	memset(q->Day_close, 0, sizeof(q->Day_close));



	memset(q->LockState, 0, sizeof(q->LockState));


	memset(q->LastHighPrice, 0, sizeof(q->LastHighPrice));
	memset(q->LastLowPrice, 0, sizeof(q->LastLowPrice));

	memset(q->DynamicStopLossTime, 0, sizeof(q->DynamicStopLossTime));

	memset(q->open, 0, sizeof(q->open));
	memset(q->close, 0, sizeof(q->close));
	memset(q->high, 0, sizeof(q->high));
	memset(q->low, 0, sizeof(q->low));

	memset(q->nottime  , 0, 5 * sizeof(q->nottime));
	memset(q->starttime, 0, 5 * sizeof(q->starttime));
	memset(q->endtime  , 0, 5 * sizeof(q->endtime));

	q->period_M1 = false;
	q->period_M3 = false;
	q->period_M5 = false;
	q->period_M10 = false;
	q->period_M15 = false;
	q->period_M30 = false;
	q->period_M60 = false;
	q->period_M120 = false;
	q->period_M240 = false;
	q->period_D1 = false;




	//int CROSS_SINGLE_SELECT[TYPE_NUM][MAX_CROSSSINGLE_NUM];   //保存信号方向 0空，1空  2多


	//CROSS_SINGLE_TURN[TYPE_NUM][MAX_CROSSSINGLE_NUM]= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //新增


	//CROSS_SINGLE_PRICE[TYPE_NUM][MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //保存信号之间价格
	// memset(CROSS_SINGLE_PRICE, 0, sizeof(CROSS_SINGLE_PRICE));

	//CROSS_SINGLE_SPACE_VALUE[TYPE_NUM][MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //保存信号之间空间
	// memset(CROSS_SINGLE_SPACE_VALUE, 0, sizeof(CROSS_SINGLE_SPACE_VALUE));

	//CROSS_SINGLE_SPACE_RATE[TYPE_NUM][MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //保存信号之间波动空间率，应该大于3才运作
	//memset(CROSS_SINGLE_SPACE_RATE, 0, sizeof(CROSS_SINGLE_SPACE_RATE));


	//CROSS_SINGLE_SPACE_PER_M1[TYPE_NUM][MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //折合成每分钟信号之间平均波动空间率，应该大于3才运作
	//memset(CROSS_SINGLE_SPACE_PER_M1, 0, sizeof(CROSS_SINGLE_SPACE_PER_M1));

	//double CROSS_SINGLE_SPACE_VALUE_M_EMA_L[MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //保存信号之间空间,EMA
	//double CROSS_SINGLE_SPACE_VALUE_M_EMA_R[MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //保存信号之间空间,EMA
	//double CROSS_SINGLE_SPACE_VALUE_N_EMA_L[MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //保存信号之间空间,EMA
	//double CROSS_SINGLE_SPACE_VALUE_N_EMA_R[MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //保存信号之间空间,EMA


	//EMA_SPACE_M[TYPE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // 该M的EMA空间
	// memset(EMA_SPACE_M, 0, sizeof(EMA_SPACE_M));

	//EMA_SPACE_N[TYPE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // 该N的EMA空间
	//memset(EMA_SPACE_N, 0, sizeof(EMA_SPACE_N));

	//  //EMA_SPACE_MN[TYPE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };// 该MN的EMA 平均空间
	//memset(EMA_SPACE_MN, 0, sizeof(EMA_SPACE_MN));
	//PEER_INFO   OrderUserPeerList;  // 订购该操盘手的用户列表;
	//next = NULL;

	q->M1_BS_CROSS_TOTAL_BIG_PURE_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_BIG_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_BIG_PURE_Value));

	q->M1_BS_CROSS_TOTAL_BIG_SELL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_BIG_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_BIG_SELL_Value));

	q->M1_BS_CROSS_TOTAL_BIG_BUY_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_BIG_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_BIG_BUY_Value));

	q->M1_BS_CROSS_TOTAL_BIG_BL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_BIG_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_BIG_BL_Value));

	q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value));

	q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value));


	q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value));


	q->M1_BS_CROSS_TOTAL_MIDDLE_BL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_MIDDLE_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_MIDDLE_BL_Value));

	q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value));


	q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value));

	q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value));


	q->M1_BS_CROSS_TOTAL_SMALL_BL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_SMALL_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_SMALL_BL_Value));

	q->M1_BS_CROSS_TOTAL_PURE_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_PURE_Value));

	q->M1_BS_CROSS_TOTAL_SELL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_SELL_Value));


	q->M1_BS_CROSS_TOTAL_BUY_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_BUY_Value));


	q->M1_BS_CROSS_TOTAL_BL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_CROSS_TOTAL_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_CROSS_TOTAL_BL_Value));
															 //CROSS的统计

															 //double	tick_data[10];				//TICK基本数据
															 //double	tick_AskPrice1[MAX_TICK_NUM];			//买一价
															 //double	tick_BidPrice1[MAX_TICK_NUM];			//卖一价
															 //double	tick_AskVolume1[MAX_TICK_NUM];		//买一量
															 //double	tick_BidVolume1[MAX_TICK_NUM];		//卖一量
															 // double	tick_Volume[MAX_TICK_NUM];			//成交量
															 // double	tick_OpenInterest[MAX_TICK_NUM];		//持仓量



	q->tick_BS_BIG_BUY_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->tick_BS_BIG_BUY_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BIG_BUY_Value));


	q->tick_BS_BIG_SELL_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->tick_BS_BIG_SELL_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BIG_SELL_Value));


	q->tick_BS_BIG_PURE_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->tick_BS_BIG_PURE_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BIG_PURE_Value));


	q->tick_BS_BUY_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->tick_BS_BUY_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BUY_Value));


	q->tick_BS_SELL_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->tick_BS_SELL_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_SELL_Value));


	q->tick_BS_PURE_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->tick_BS_PURE_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_PURE_Value));

														// double	tick_BS_BUY_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
														// double	tick_BS_SELL_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	q->tick_BS_Time = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->tick_BS_Time, 0, max_pure_tick_num * sizeof(q->tick_BS_Time));


	q->tick_BS_BL_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->tick_BS_BL_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BL_Value));



	q->M1_BS_TOTAL_BIG_PURE_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_BIG_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_BIG_PURE_Value));


	q->M1_BS_TOTAL_BIG_SELL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_BIG_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_BIG_SELL_Value));


	q->M1_BS_TOTAL_BIG_BUY_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_BIG_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_BIG_BUY_Value));


	q->M1_BS_TOTAL_BIG_BL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_BIG_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_BIG_BL_Value));


	q->M1_BS_TOTAL_MIDDLE_PURE_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_MIDDLE_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_MIDDLE_PURE_Value));


	q->M1_BS_TOTAL_MIDDLE_SELL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_MIDDLE_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_MIDDLE_SELL_Value));


	q->M1_BS_TOTAL_MIDDLE_BUY_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_MIDDLE_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_MIDDLE_BUY_Value));

	q->M1_BS_TOTAL_MIDDLE_BL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_MIDDLE_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_MIDDLE_BL_Value));


	q->M1_BS_TOTAL_SMALL_PURE_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_SMALL_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_SMALL_PURE_Value));


	q->M1_BS_TOTAL_SMALL_SELL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_SMALL_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_SMALL_SELL_Value));


	q->M1_BS_TOTAL_SMALL_BUY_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_SMALL_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_SMALL_BUY_Value));


	q->M1_BS_TOTAL_SMALL_BL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_SMALL_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_SMALL_BL_Value));

	q->M1_BS_TOTAL_PURE_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_PURE_Value));


	q->M1_BS_TOTAL_SELL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_SELL_Value));


	q->M1_BS_TOTAL_BUY_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_BUY_Value));


	q->M1_BS_TOTAL_BL_Value = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset(q->M1_BS_TOTAL_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_TOTAL_BL_Value));



	q->M1_open = new double[max_pure_m1_num];	//分钟K线开
	memset(q->M1_open, 0, max_pure_m1_num * sizeof(q->M1_open));

	q->M1_high = new double[max_pure_m1_num];	//分钟K线高
	memset(q->M1_high, 0, max_pure_m1_num * sizeof(q->M1_high));

	q->M1_low = new double[max_pure_m1_num];		//分钟K线低
	memset(q->M1_low, 0, max_pure_m1_num * sizeof(q->M1_low));


	q->M1_close = new double[max_pure_m1_num];	//分钟K线收
	memset(q->M1_close, 0, max_pure_m1_num * sizeof(q->M1_close));

											//bool	M1Klinesig;		//1分钟K线标志
	q->M1_BS_Time = new double[max_pure_m1_num];
	memset(q->M1_BS_Time, 0, max_pure_m1_num * sizeof(q->M1_BS_Time));

	q->M1_BS_BIG_PURE_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_BIG_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BIG_PURE_Value));


	q->M1_BS_BIG_BUY_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_BIG_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BIG_BUY_Value));


	q->M1_BS_BIG_SELL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_BIG_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BIG_SELL_Value));


	q->M1_BS_BIG_BL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M1_BS_BIG_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BIG_BL_Value));

	q->M1_BS_MIDDLE_PURE_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_MIDDLE_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_MIDDLE_PURE_Value));


	q->M1_BS_MIDDLE_BUY_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_MIDDLE_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_MIDDLE_BUY_Value));


	q->M1_BS_MIDDLE_SELL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_MIDDLE_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_MIDDLE_SELL_Value));


	q->M1_BS_MIDDLE_BL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M1_BS_MIDDLE_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_MIDDLE_BL_Value));


	q->M1_BS_SMALL_PURE_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_SMALL_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SMALL_PURE_Value));


	q->M1_BS_SMALL_BUY_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_SMALL_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SMALL_BUY_Value));


	q->M1_BS_SMALL_SELL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_SMALL_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SMALL_SELL_Value));



	q->M1_BS_SMALL_BL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M1_BS_SMALL_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SMALL_BL_Value));

	q->M1_BS_PURE_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_PURE_Value));


	q->M1_BS_BUY_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BUY_Value));


	q->M1_BS_SELL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M1_BS_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SELL_Value));


	q->M1_BS_BL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M1_BS_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BL_Value));



													//3分钟
	q->M3_open = new double[max_pure_m3_num];	//分钟K线开
	memset(q->M3_open, 0, max_pure_m3_num * sizeof(q->M3_open));


	q->M3_high = new double[max_pure_m3_num];	//分钟K线高
	memset(q->M3_high, 0, max_pure_m3_num * sizeof(q->M3_high));


	q->M3_low = new double[max_pure_m3_num];		//分钟K线低
	memset(q->M3_low, 0, max_pure_m3_num * sizeof(q->M3_low));

	q->M3_close = new double[max_pure_m3_num];	//分钟K线收
	memset(q->M3_close, 0, max_pure_m3_num * sizeof(q->M3_close));


											//	bool	M3Klinesig[20];		//3分钟K线标志
	q->M3_BS_Time = new double[max_pure_m3_num];
	memset(q->M3_BS_Time, 0, max_pure_m3_num * sizeof(q->M3_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	q->M3_BS_BIG_PURE_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M3_BS_BIG_PURE_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BIG_PURE_Value));


	q->M3_BS_BIG_BUY_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M3_BS_BIG_BUY_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BIG_BUY_Value));


	q->M3_BS_BIG_SELL_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M3_BS_BIG_SELL_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BIG_SELL_Value));


	q->M3_BS_BIG_BL_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M3_BS_BIG_BL_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BIG_BL_Value));

	q->M3_BS_BUY_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M3_BS_BUY_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BUY_Value));


	q->M3_BS_SELL_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M3_BS_SELL_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_SELL_Value));



	q->M3_BS_PURE_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M3_BS_PURE_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_PURE_Value));


	q->M3_BS_BL_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M3_BS_BL_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BL_Value));												//3分钟


													//5分钟
	q->M5_open = new double[max_pure_m5_num];	//分钟K线开
	memset(q->M5_open, 0, max_pure_m5_num * sizeof(q->M5_open));

	q->M5_high = new double[max_pure_m5_num];	//分钟K线高
	memset(q->M5_high, 0, max_pure_m5_num * sizeof(q->M5_high));


	q->M5_low = new double[max_pure_m5_num];		//分钟K线低
	memset(q->M5_low, 0, max_pure_m5_num * sizeof(q->M5_low));

	q->M5_close = new double[max_pure_m5_num];	//分钟K线收
	memset(q->M5_close, 0, max_pure_m5_num * sizeof(q->M5_close));

											//	bool	M3Klinesig[20];		//3分钟K线标志
	q->M5_BS_Time = new double[max_pure_m5_num];
	memset(q->M5_BS_Time, 0, max_pure_m5_num * sizeof(q->M5_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	q->M5_BS_BIG_PURE_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M5_BS_BIG_PURE_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BIG_PURE_Value));

	q->M5_BS_BIG_BUY_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M5_BS_BIG_BUY_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BIG_BUY_Value));

	q->M5_BS_BIG_SELL_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M5_BS_BIG_SELL_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BIG_SELL_Value));

	q->M5_BS_BIG_BL_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M5_BS_BIG_BL_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BIG_BL_Value));

	q->M5_BS_BUY_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M5_BS_BUY_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BUY_Value));


	q->M5_BS_SELL_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M5_BS_SELL_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_SELL_Value));

	q->M5_BS_PURE_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M5_BS_PURE_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_PURE_Value));


	q->M5_BS_BL_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M5_BS_BL_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BL_Value));

													//10分钟
	q->M10_open = new double[max_pure_m10_num];	//分钟K线开
	memset(q->M10_open, 0, max_pure_m10_num * sizeof(q->M10_open));

	q->M10_high = new double[max_pure_m10_num];	//分钟K线高
	memset(q->M10_high, 0, max_pure_m10_num * sizeof(q->M10_high));


	q->M10_low = new double[max_pure_m10_num];		//分钟K线低
	memset(q->M10_low, 0, max_pure_m10_num * sizeof(q->M10_low));

	q->M10_close = new double[max_pure_m10_num];	//分钟K线收
	memset(q->M10_close, 0, max_pure_m10_num * sizeof(q->M10_close));
												//	bool	M3Klinesig[20];		//3分钟K线标志
	q->M10_BS_Time = new double[max_pure_m10_num];
	memset(q->M10_BS_Time, 0, max_pure_m10_num * sizeof(q->M10_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	q->M10_BS_BIG_PURE_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M10_BS_BIG_PURE_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BIG_PURE_Value));

	q->M10_BS_BIG_BUY_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M10_BS_BIG_BUY_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BIG_BUY_Value));


	q->M10_BS_BIG_SELL_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M10_BS_BIG_SELL_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BIG_SELL_Value));


	q->M10_BS_BIG_BL_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M10_BS_BIG_BL_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BIG_BL_Value));

	q->M10_BS_BUY_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M10_BS_BUY_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BUY_Value));

	q->M10_BS_SELL_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M10_BS_SELL_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_SELL_Value));

	q->M10_BS_PURE_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M10_BS_PURE_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_PURE_Value));


	q->M10_BS_BL_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M10_BS_BL_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BL_Value));
													//10分钟

													//15分钟
	q->M15_open = new double[max_pure_m15_num];	//分钟K线开
	memset(q->M15_open, 0, max_pure_m15_num * sizeof(q->M15_open));

	q->M15_high = new double[max_pure_m15_num];	//分钟K线高
	memset(q->M15_high, 0, max_pure_m15_num * sizeof(q->M15_high));

	q->M15_low = new double[max_pure_m15_num];		//分钟K线低
	memset(q->M15_low, 0, max_pure_m15_num * sizeof(q->M15_low));

	q->M15_close = new double[max_pure_m15_num];	//分钟K线收
	memset(q->M15_close, 0, max_pure_m15_num * sizeof(q->M15_close));
												//	bool	M3Klinesig[20];		//3分钟K线标志
	q->M15_BS_Time = new double[max_pure_m15_num];
	memset(q->M15_BS_Time, 0, max_pure_m15_num * sizeof(q->M15_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	q->M15_BS_BIG_PURE_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M15_BS_BIG_PURE_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BIG_PURE_Value));

	q->M15_BS_BIG_BUY_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M15_BS_BIG_BUY_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BIG_BUY_Value));


	q->M15_BS_BIG_SELL_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M15_BS_BIG_SELL_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BIG_SELL_Value));

	q->M15_BS_BIG_BL_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M15_BS_BIG_BL_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BIG_BL_Value));

	q->M15_BS_BUY_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M15_BS_BUY_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BUY_Value));


	q->M15_BS_SELL_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M15_BS_SELL_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_SELL_Value));


	q->M15_BS_PURE_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M15_BS_PURE_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_PURE_Value));

	q->M15_BS_BL_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M15_BS_BL_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BL_Value));												//15分钟



													//30分钟
	q->M30_open = new double[max_pure_m30_num];	//分钟K线开
	memset(q->M30_open, 0, max_pure_m30_num * sizeof(q->M30_open));

	q->M30_high = new double[max_pure_m30_num];	//分钟K线高
	memset(q->M30_high, 0, max_pure_m30_num * sizeof(q->M30_high));


	q->M30_low = new double[max_pure_m30_num];		//分钟K线低
	memset(q->M30_low, 0, max_pure_m30_num * sizeof(q->M30_low));


	q->M30_close = new double[max_pure_m30_num];	//分钟K线收
	memset(q->M30_close, 0, max_pure_m30_num * sizeof(q->M30_close));

												//	bool	M3Klinesig[20];		//3分钟K线标志
	q->M30_BS_Time = new double[max_pure_m30_num];
	memset(q->M30_BS_Time, 0, max_pure_m30_num * sizeof(q->M30_BS_Time));

	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	q->M30_BS_BIG_PURE_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M30_BS_BIG_PURE_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BIG_PURE_Value));


	q->M30_BS_BIG_BUY_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M30_BS_BIG_BUY_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BIG_BUY_Value));

	q->M30_BS_BIG_SELL_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M30_BS_BIG_SELL_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BIG_SELL_Value));

	q->M30_BS_BIG_BL_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M30_BS_BIG_BL_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BIG_BL_Value));

	q->M30_BS_BUY_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M30_BS_BUY_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BUY_Value));

	q->M30_BS_SELL_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M30_BS_SELL_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_SELL_Value));


	q->M30_BS_PURE_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M30_BS_PURE_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_PURE_Value));


	q->M30_BS_BL_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M30_BS_BL_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BL_Value));											//30分钟


													//60分钟
	q->M60_open = new double[max_pure_m60_num];	//分钟K线开
	memset(q->M60_open, 0, max_pure_m60_num * sizeof(q->M60_open));


	q->M60_high = new double[max_pure_m60_num];	//分钟K线高
	memset(q->M60_high, 0, max_pure_m60_num * sizeof(q->M60_high));


	q->M60_low = new double[max_pure_m60_num];		//分钟K线低
	memset(q->M60_low, 0, max_pure_m60_num * sizeof(q->M60_low));


	q->M60_close = new double[max_pure_m60_num];	//分钟K线收
	memset(q->M60_close, 0, max_pure_m60_num * sizeof(q->M60_close));

												//	bool	M3Klinesig[20];		//3分钟K线标志
	q->M60_BS_Time = new double[max_pure_m60_num];
	memset(q->M60_BS_Time, 0, max_pure_m60_num * sizeof(q->M60_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	q->M60_BS_BIG_PURE_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M60_BS_BIG_PURE_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BIG_PURE_Value));

	q->M60_BS_BIG_BUY_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M60_BS_BIG_BUY_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BIG_BUY_Value));

	q->M60_BS_BIG_SELL_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M60_BS_BIG_SELL_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BIG_SELL_Value));


	q->M60_BS_BIG_BL_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例
	memset(q->M60_BS_BIG_BL_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BIG_BL_Value));

	q->M60_BS_BUY_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M60_BS_BUY_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BUY_Value));

	q->M60_BS_SELL_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M60_BS_SELL_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_SELL_Value));


	q->M60_BS_PURE_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖净量
	memset(q->M60_BS_PURE_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_PURE_Value));

	q->M60_BS_BL_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据,主动买卖拉升压力比例								
	memset(q->M60_BS_BL_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BL_Value));


													//120分钟
	q->M120_open = new double[max_pure_m120_num];	//分钟K线开
	memset(q->M120_open, 0, max_pure_m120_num * sizeof(q->M120_open));

	q->M120_high = new double[max_pure_m120_num];	//分钟K线高
	memset(q->M120_high, 0, max_pure_m120_num * sizeof(q->M120_high));

	q->M120_low = new double[max_pure_m120_num];		//分钟K线低
	memset(q->M120_low, 0, max_pure_m120_num * sizeof(q->M120_low));

	q->M120_close = new double[max_pure_m120_num];	//分钟K线收
	memset(q->M120_close, 0, max_pure_m120_num * sizeof(q->M120_close));
												//	bool	M3Klinesig[20];		//3分钟K线标志
	q->M120_BS_Time = new double[max_pure_m120_num];
	memset(q->M120_BS_Time, 0, max_pure_m120_num * sizeof(q->M120_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	q->M120_BS_BIG_PURE_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M120_BS_BIG_PURE_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BIG_PURE_Value));

	q->M120_BS_BIG_BUY_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M120_BS_BIG_BUY_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BIG_BUY_Value));

	q->M120_BS_BIG_SELL_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M120_BS_BIG_SELL_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BIG_SELL_Value));

	q->M120_BS_BIG_BL_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	memset(q->M120_BS_BIG_BL_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BIG_BL_Value));

	q->M120_BS_BUY_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M120_BS_BUY_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BUY_Value));

	q->M120_BS_SELL_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M120_BS_SELL_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_SELL_Value));

	q->M120_BS_PURE_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M120_BS_PURE_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_PURE_Value));

	q->M120_BS_BL_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例								
	memset(q->M120_BS_BL_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BL_Value));												//120分钟

														//240分钟
	q->M240_open = new double[max_pure_m240_num];	//分钟K线开
	memset(q->M240_open, 0, max_pure_m240_num * sizeof(q->M240_open));

	q->M240_high = new double[max_pure_m240_num];	//分钟K线高
	memset(q->M240_high, 0, max_pure_m240_num * sizeof(q->M240_high));

	q->M240_low = new double[max_pure_m240_num];		//分钟K线低
	memset(q->M240_low, 0, max_pure_m240_num * sizeof(q->M240_low));

	q->M240_close = new double[max_pure_m240_num];	//分钟K线收
	memset(q->M240_close, 0, max_pure_m240_num * sizeof(q->M240_close));
												//	bool	M3Klinesig[20];		//3分钟K线标志
	q->M240_BS_Time = new double[max_pure_m240_num];
	memset(q->M240_BS_Time, 0, max_pure_m240_num * sizeof(q->M240_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	q->M240_BS_BIG_PURE_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M240_BS_BIG_PURE_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BIG_PURE_Value));

	q->M240_BS_BIG_BUY_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M240_BS_BIG_BUY_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BIG_BUY_Value));

	q->M240_BS_BIG_SELL_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M240_BS_BIG_SELL_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BIG_SELL_Value));

	q->M240_BS_BIG_BL_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	memset(q->M240_BS_BIG_BL_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BIG_BL_Value));

	q->M240_BS_BUY_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M240_BS_BUY_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BUY_Value));

	q->M240_BS_SELL_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M240_BS_SELL_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_SELL_Value));

	q->M240_BS_PURE_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->M240_BS_PURE_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_PURE_Value));

	q->M240_BS_BL_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例								
	memset(q->M240_BS_BL_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BL_Value));												//240分钟


														//240分钟
	q->D1_open = new double[max_pure_d1_num];	//分钟K线开
	memset(q->D1_open, 0, max_pure_d1_num * sizeof(q->D1_open));

	q->D1_high = new double[max_pure_d1_num];	//分钟K线高
	memset(q->D1_high, 0, max_pure_d1_num * sizeof(q->D1_high));

	q->D1_low = new double[max_pure_d1_num];		//分钟K线低
	memset(q->D1_low, 0, max_pure_d1_num * sizeof(q->D1_low));

	q->D1_close = new double[max_pure_d1_num];	//分钟K线收
	memset(q->D1_close, 0, max_pure_d1_num * sizeof(q->D1_close));
											//	bool	M3Klinesig[20];		//3分钟K线标志
	q->D1_BS_Time = new double[max_pure_d1_num];
	memset(q->D1_BS_Time, 0, max_pure_d1_num * sizeof(q->D1_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	q->D1_BS_BIG_PURE_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->D1_BS_BIG_PURE_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BIG_PURE_Value));

	q->D1_BS_BIG_BUY_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->D1_BS_BIG_BUY_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BIG_BUY_Value));

	q->D1_BS_BIG_SELL_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->D1_BS_BIG_SELL_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BIG_SELL_Value));

	q->D1_BS_BIG_BL_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例
	memset(q->D1_BS_BIG_BL_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BIG_BL_Value));

	q->D1_BS_BUY_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->D1_BS_BUY_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BUY_Value));

	q->D1_BS_SELL_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->D1_BS_SELL_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_SELL_Value));

	q->D1_BS_PURE_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖净量
	memset(q->D1_BS_PURE_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_PURE_Value));

	q->D1_BS_BL_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存个TICK数据,主动买卖拉升压力比例								
	memset(q->D1_BS_BL_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BL_Value));											//240分钟



}
void UpdatePriceHash(const char * InstrumentID, CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	//double pDepthMarketData->LastPrice = pDepthMarketData->LastPrice;

	if (pDepthMarketData->LastPrice < 1) //恢复
										 //if (NewPrice < 5 || thistime<-1e-10) //小于5元，错误价格，不做处理
	{
		return;// false;
	}

	//int  Millisecs = pDepthMarketData->UpdateMillisec;
	//时间

	
	/*
	//时间
	GuestOnlineHash  * q = new GuestOnlineHash;
	 q->key = 8;
	//memset(&r,0,sizeof(GuestOnlineHash));

	string temp = InstrumentID;
	//pair<string, GuestOnlineHash>value(temp, r);

	pair<string, GuestOnlineHash>value(temp, *q);
	/////////////////////////////bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb

	//map


	QS_Data_Map::iterator it = mapData.find(InstrumentID);

	if (it == mapData.end())
	{
		//printf("*************没找到该合约的%s\n", InstrumentID);
		mapData.insert(value);
	}
	else
	{
		printf("xxxxxxxxxxxxxxx找到该合约的%s\n", InstrumentID);
		//GuestOnlineHash  * r 

	}

		q = &(it->second);

		printf("q->key : %d\n", q->key);
	*/
	//EnterCriticalSection(&g_csdata);
	//cmdlist.push_back(tn);
	//LeaveCriticalSection(&g_csdata);
	


	std::hash_map<string, GuestOnlineHash>::iterator it;
	it = mapData.find(InstrumentID);
	if (it == mapData.end())
	{	  // printf("*************没找到该合约的%s\n", InstrumentID);
		    GuestOnlineHash value;
		    InitGuestOnlineHash(&value);
			mapData.insert(std::make_pair(InstrumentID, value));
			//ClearGuestOnlineHash(&value);
	        it = mapData.find(InstrumentID);
	}


	if (it != mapData.end())
	{

		//printf("xxxxxxxxxxxxxxx找到该合约的%s\n", InstrumentID);
		GuestOnlineHash * q = &(it->second);
		//it->second = value;
		//	it->second.D1_BS_BIG_BL_Value
		char names[10] = { 0 };
		char times[10] = { 0 };
		//memset()

		// hours;// = atoi(str1.c_str());
		// minutes;// = atoi(str2.c_str());
		// seconds;// = atoi(str3.c_str());
		// Millisecs;// = pDepthMarketData->UpdateMillisec;
		// NewPrice;
		// NewVolume;
		////try
		//{


		strcpy_s(times, sizeof(times), (char*)(&pDepthMarketData->UpdateTime));


		//InstrumentID_name = names;
		string str0 = names;
		string str1 = times;
		string str2 = times;
		string str3 = times;

		str1 = str1.substr(0, 2);		//
		str2 = str2.substr(3, 2);		//
		str3 = str3.substr(6, 2);		//
		int  hours = atoi(str1.c_str());
		int  minutes = atoi(str2.c_str());
		int  seconds = atoi(str3.c_str());
		//int  Millisecs = pDepthMarketData->UpdateMillisec;
		double thistime = 0.01* hours + 0.0001* minutes + 0.000001*seconds;
		//if (q)

		{
			//printf("thistimechar:[%s]\n", thistimechar);
			//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX这个应该检查是否恢复ReceiveTick[i] = true;  //这个需要检查，导致12点以后不运行的原因


			int Q_BarTime_1 = hours * 60 * 60 + minutes * 60 + seconds;						//时间采用秒计
			double Q_BarTime_2 = (1 / 10e1)*hours + (1 / 10e3)*minutes + (1 / 10e5)*seconds;	//时间格式0.145100 = 14：51：00	

																								//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX这个应该见识是否恢复Q_BarTime_1n[i] = Q_BarTime_1;

			q->tick_data[0] = 2;		//设置标志位
			q->tick_data[1] = atof(pDepthMarketData->TradingDay);
			q->tick_data[2] = Q_BarTime_2;//pDepthMarketData->UpdateTime;
			q->tick_data[3] = pDepthMarketData->UpdateMillisec;

			q->tick_data[4] = pDepthMarketData->LastPrice;
			q->tick_data[5] = pDepthMarketData->AveragePrice;
			q->tick_data[6] = pDepthMarketData->HighestPrice;
			q->tick_data[7] = pDepthMarketData->LowestPrice;




			//printf("thistime:[%0.06f]\n", thistime);
			//return;
			//q->LastHighPrice = z_max(q->LastHighPrice, NewPrice);
			//q->LastLowPrice = z_min(q->LastLowPrice, NewPrice);

			bool check1, check2, check3, check4, check5, check6, check7;

			bool typecheck1 = (q->TYPEID == TYPE_T) || (q->TYPEID == TYPE_TF);
			bool typecheck2 = (q->TYPEID == TYPE_IC) || (q->TYPEID == TYPE_IF) || (q->TYPEID == TYPE_IH);
			bool typecheck3 = (!typecheck1) && (!typecheck2); //(q->TYPEID == TYPE_PP) || (q->TYPEID == TYPE_CS) || (q->TYPEID == TYPE_JD) || (q->TYPEID == TYPE_L) || (q->TYPEID == TYPE_V) || (q->TYPEID == TYPE_C) || (q->TYPEID == TYPE_WH) || (q->TYPEID == TYPE_SM) || (q->TYPEID == TYPE_SF);
			bool typecheck4 = (q->TYPEID == TYPE_I) || (q->TYPEID == TYPE_TA) || (q->TYPEID == TYPE_A) || (q->TYPEID == TYPE_M) || (q->TYPEID == TYPE_Y) || (q->TYPEID == TYPE_P) || (q->TYPEID == TYPE_MA) || (q->TYPEID == TYPE_FG);
			bool typecheck5 = (q->TYPEID == TYPE_RU) || (q->TYPEID == TYPE_BU) || (q->TYPEID == TYPE_RB);
			bool typecheck6 = (q->TYPEID == TYPE_NI) || (q->TYPEID == TYPE_ZN) || (q->TYPEID == TYPE_AL) || (q->TYPEID == TYPE_CU);  // (!typecheck2) && (!typecheck3);
			bool typecheck7 = (q->TYPEID == TYPE_AU) || (q->TYPEID == TYPE_AG);




			check1 = (q->tick_data[2]>0.0910 && q->tick_data[2]<0.0915 && typecheck1) || (q->tick_data[2]<0.1518 && q->tick_data[2]>0.0914 && !q->FristTick && typecheck1);  //国债
			check2 = (q->tick_data[2]>0.0925 && q->tick_data[2]<0.0930 && typecheck2) || (q->tick_data[2]<0.1505 && q->tick_data[2]>0.0929 && !q->FristTick && typecheck2); //股指
			check3 = (q->tick_data[2]>0.0856 && q->tick_data[2]<0.0900 && typecheck3) || (q->tick_data[2]<0.1505 && q->tick_data[2]>0.0859 && !q->FristTick && typecheck3);  //白天
			check4 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck4) || ((q->tick_data[2]<0.2335 && q->tick_data[2]>0.2059) && !q->FristTick && typecheck4); //有夜盘  23:30结束
			check5 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck5) || ((q->tick_data[2]<0.2305 && q->tick_data[2]>0.2059) && !q->FristTick && typecheck5); //有夜盘  23:00结束
			check6 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck6) || ((q->tick_data[2]<0.0105 || q->tick_data[2]>0.2059) && !q->FristTick && typecheck6); //有夜盘  1:00结束
			check7 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck7) || ((q->tick_data[2]<0.0235 || q->tick_data[2]>0.2059) && !q->FristTick && typecheck7); //有夜盘  2:30结束  


		    //if(1)
			if ((check1 || check2 || check3 || check4 || check5 || check6 || check7))
			{
				//printf("change......\n");

				q->M1Klinesig = true;
				q->tick_data[8] = pDepthMarketData->UpperLimitPrice;	//涨停价
				q->tick_data[9] = pDepthMarketData->LowerLimitPrice;	//跌停价					


				if ((pDepthMarketData->OpenPrice<1e-10) || _isnan(pDepthMarketData->OpenPrice))
				{
					//printf("pDepthMarketData->OpenPrice <1e-10\n");

					char chr[150] = { 0 };
					_snprintf_s(chr, sizeof(chr), sizeof(chr), "OpenPrice<1e-10或OpenPrice不是有效的数字[%0.06f][%0.02f]", Q_BarTime_2, pDepthMarketData->LastPrice);
					WirteSingleRecordToFile8(0, chr);
					return;
				}

				if (pDepthMarketData->OpenPrice>((pDepthMarketData->UpperLimitPrice) + 1) || pDepthMarketData->OpenPrice<((pDepthMarketData->LowerLimitPrice) - 1))
				{
					//printf("pDepthMarketData->OpenPrice超出涨跌停价\n");

					char chr[150] = { 0 };
					_snprintf_s(chr, sizeof(chr), sizeof(chr), "pDepthMarketData->OpenPrice超出涨跌停价[%0.06f][%0.02f] 涨停[%0.02f] 跌停[%0.02f]", Q_BarTime_2, pDepthMarketData->LastPrice, pDepthMarketData->UpperLimitPrice, pDepthMarketData->LowerLimitPrice);
					WirteSingleRecordToFile8(0, chr);
					return;
				}

				q->Day_open[0] = pDepthMarketData->OpenPrice;

				q->M1_open[1] = q->Day_open[0];
				q->M1_high[1] = q->Day_open[0];
				q->M1_low[1] = q->Day_open[0];
				q->M1_close[1] = q->Day_open[0];

				q->M1_open[0] = q->Day_open[0];
				q->M1_high[0] = q->Day_open[0];		    //为什么不影响刷新？是否有必要存在？-----------------------------------------------------------------------------------
				q->M1_low[0] = q->Day_open[0];
				q->M1_close[0] = q->Day_open[0];
				//为什么不影响刷新？是否有必要存在？-----------------------------------------------------------------------------------

				//恢复 printf("\nPPPPPPPPPPPPPPPPPPPPPPPPP__________________________多次运行了____________________________pppppppppppppppppppppppppp");
				q->M1_BS_Time[0] = q->tick_BS_Time[0];	   //为什么每次都刷新

				q->M3_open[1] = q->Day_open[0];
				q->M3_high[1] = q->Day_open[0];
				q->M3_low[1] = q->Day_open[0];
				q->M3_close[1] = q->Day_open[0];
				q->M3_open[0] = q->Day_open[0];
				q->M3_high[0] = q->Day_open[0];
				q->M3_low[0] = q->Day_open[0];
				q->M3_close[0] = q->Day_open[0];
				q->M3_BS_Time[0] = q->tick_BS_Time[0];	   //为什么每次都刷新


				q->M5_open[1] = q->Day_open[0];
				q->M5_high[1] = q->Day_open[0];
				q->M5_low[1] = q->Day_open[0];
				q->M5_close[1] = q->Day_open[0];
				q->M5_open[0] = q->Day_open[0];
				q->M5_high[0] = q->Day_open[0];
				q->M5_low[0] = q->Day_open[0];
				q->M5_close[0] = q->Day_open[0];
				q->M5_BS_Time[0] = q->tick_BS_Time[0];	   //为什么每次都刷新


				q->M10_open[1] = q->Day_open[0];
				q->M10_high[1] = q->Day_open[0];
				q->M10_low[1] = q->Day_open[0];
				q->M10_close[1] = q->Day_open[0];
				q->M10_open[0] = q->Day_open[0];
				q->M10_high[0] = q->Day_open[0];
				q->M10_low[0] = q->Day_open[0];
				q->M10_close[0] = q->Day_open[0];
				q->M10_BS_Time[0] = q->tick_BS_Time[0];	   //为什么每次都刷新

				q->M15_open[1] = q->Day_open[0];
				q->M15_high[1] = q->Day_open[0];
				q->M15_low[1] = q->Day_open[0];
				q->M15_close[1] = q->Day_open[0];
				q->M15_open[0] = q->Day_open[0];
				q->M15_high[0] = q->Day_open[0];
				q->M15_low[0] = q->Day_open[0];
				q->M15_close[0] = q->Day_open[0];
				q->M15_BS_Time[0] = q->tick_BS_Time[0];	   //为什么每次都刷新


				q->M30_open[1] = q->Day_open[0];
				q->M30_high[1] = q->Day_open[0];
				q->M30_low[1] = q->Day_open[0];
				q->M30_close[1] = q->Day_open[0];
				q->M30_open[0] = q->Day_open[0];
				q->M30_high[0] = q->Day_open[0];
				q->M30_low[0] = q->Day_open[0];
				q->M30_close[0] = q->Day_open[0];
				q->M30_BS_Time[0] = q->tick_BS_Time[0];	   //为什么每次都刷新


				q->M60_open[1] = q->Day_open[0];
				q->M60_high[1] = q->Day_open[0];
				q->M60_low[1] = q->Day_open[0];
				q->M60_close[1] = q->Day_open[0];
				q->M60_open[0] = q->Day_open[0];
				q->M60_high[0] = q->Day_open[0];
				q->M60_low[0] = q->Day_open[0];
				q->M60_close[0] = q->Day_open[0];
				q->M60_BS_Time[0] = q->tick_BS_Time[0];	   //为什么每次都刷新

														   //?????????????有啥用
														   //q->Day_CloseProfit = 0;
														   //q->Day_CloseProfitA = 0;
														   //q->Day_CloseProfitB = 0;

														   //q->Day_TradeNumb = 0;

			}






			//接上面60-129
			for (int j = max_pure_tick_move_num; j >MAX_TICK_MOVE_NUM; j--) //注意和下面数字如何接循环的
			{

				// printf("____X:%d\n",j);
				
				q->tick_BS_BUY_Value[j] = q->tick_BS_BUY_Value[j - 1];
				//tick_BS_BUY_Time[j] = tick_BS_BUY_Time[j-1];
				q->tick_BS_SELL_Value[j] = q->tick_BS_SELL_Value[j - 1];
				//tick_BS_SELL_Time[j] = tick_BS_SELL_Time[j-1];
				q->tick_BS_PURE_Value[j] = q->tick_BS_PURE_Value[j - 1];
				q->tick_BS_Time[j] = q->tick_BS_Time[j - 1];
				q->tick_BS_BL_Value[j] = q->tick_BS_BL_Value[j - 1];
				//			tick_BS_BL_Time[j] = tick_BS_BL_Time[j-1];
				
			}

			//1-59
			for (int j = MAX_TICK_MOVE_NUM; j >0; j--)
			{
				//printf("\nX:%d",j);
				q->tick_AskPrice1[j] = q->tick_AskPrice1[j - 1];
				q->tick_BidPrice1[j] = q->tick_BidPrice1[j - 1];
				q->tick_AskVolume1[j] = q->tick_AskVolume1[j - 1];
				q->tick_BidVolume1[j] = q->tick_BidVolume1[j - 1];
				q->tick_Volume[j] = q->tick_Volume[j - 1];
				q->tick_OpenInterest[j] = q->tick_OpenInterest[j - 1]; //持仓量

																	   
				q->tick_BS_BUY_Value[j] = q->tick_BS_BUY_Value[j - 1];
				//tick_BS_BUY_Time[j] = tick_BS_BUY_Time[j-1];
				q->tick_BS_SELL_Value[j] = q->tick_BS_SELL_Value[j - 1];
				//tick_BS_SELL_Time[j] = tick_BS_SELL_Time[j-1];
				q->tick_BS_PURE_Value[j] = q->tick_BS_PURE_Value[j - 1];
				q->tick_BS_Time[j] = q->tick_BS_Time[j - 1];
				q->tick_BS_BL_Value[j] = q->tick_BS_BL_Value[j - 1];
				//			tick_BS_BL_Time[j] = tick_BS_BL_Time[j-1];
				
			}



			//实盘开盘前登录，交易所此处插入的最大值，需屏蔽处理
			//bool sstate;
			// if (serverselect) //实盘时间
			//{
			//老的sstate=(tick_data[2]>0.0250 && tick_data[2]<0.0850) || (tick_data[2]>0.1550 && tick_data[2]<0.2050); 
			bool sstate = (q->tick_data[2]>0.0250 && q->tick_data[2]<0.0850) || (q->tick_data[2]>0.1550 && q->tick_data[2]<0.2050);
			//}
			//else            //24小时服务器
			//{
			//  sstate = false;//(tick_data[2]>0.0250 && tick_data[2]<0.0850) || (tick_data[2]>0.1550 && tick_data[2]<0.2050);
			//}

			if (sstate)
				//if (AskPrice1t>99999 || BidPrice1t>99999)
			{
				q->tick_AskPrice1[0] = 0;
				q->tick_BidPrice1[0] = 0;
			}
			else
			{
				q->tick_AskPrice1[0] = pDepthMarketData->AskPrice1;
				q->tick_BidPrice1[0] = pDepthMarketData->BidPrice1;
			}

			q->tick_AskVolume1[0] = pDepthMarketData->AskVolume1;
			q->tick_BidVolume1[0] = pDepthMarketData->BidVolume1;
			q->tick_Volume[0] = pDepthMarketData->Volume;  //总成交量   单位时间成交量tick_Volume[0]-tick_Volume[1]
			q->tick_OpenInterest[0] = pDepthMarketData->OpenInterest;
			


			//printf("\n问价 [%.02f] 卖一价[%.02f] 买一价[%.02f]\n",tick_data[4],tick_AskPrice1[0],tick_BidPrice1[0]);
			//恢复a  		printf("\n 卖一[%.02f] 买一[%.02f] 毫秒[%.02f]时间[%.08f]\n",tick_AskVolume1[0]-tick_BidVolume1[1],tick_OpenInterest[0],tick_data[3],Q_BarTime_2); //2次毫秒数都一样，说明没更新

			//输出每笔成交量	

			//char DD[200];
			//string ttt=DD;


			//为何重复2次
			double thisvol = 0;
			//检查
			if (q->tick_Volume[1]>0)
			{
				thisvol = q->tick_Volume[0] - q->tick_Volume[1];
			}
			//检查

			if (q->tick_data[4] == q->tick_AskPrice1[0])  //卖一价成交，应该是主动买入
			{
				q->tick_BS_BUY_Value[0] = thisvol;
				q->tick_BS_SELL_Value[0] = 0;
				q->tick_BS_PURE_Value[0] = thisvol;
				// tick_BS_BUY_Time[0]=  tick_data[2];
				// tick_BS_SELL_Time[0]= tick_data[2];
				q->tick_BS_Time[0] = q->tick_data[2];
				//printf("\n————量 [%.00f] 卖一量[%.00f] 买一量[%.00f]\n",tick_Volume[0]-tick_Volume[1],tick_AskVolume1[0],tick_BidVolume1[0]);
				// printf("\n————量 [%.00f]\n",tick_Volume[0]-tick_Volume[1]);
			}
			else //买一价成交，应该是主动卖入
			{
				q->tick_BS_BUY_Value[0] = 0;
				q->tick_BS_SELL_Value[0] = thisvol;//pDepthMarketData->Volume;
				q->tick_BS_PURE_Value[0] = (-1)*thisvol;//(-1)*pDepthMarketData->Volume;
														// tick_BS_BUY_Time[0]=  tick_data[2];
														// tick_BS_SELL_Time[0]= tick_data[2];
				q->tick_BS_Time[0] = q->tick_data[2];
				// printf("\n————量 [%.00f]\n",tick_Volume[1]-tick_Volume[0]);

			}

					

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////



			//printf("%s 检查是否更新数据\n", InstrumentID);
			bool Timemore0 = false, Timemore1 = false, Timemore2 = false;

			//printf("TYPE: %d\n",q->TYPEID);

			switch (q->TYPEID)
			{
				////////////////////////////////////////////////////
			case TYPE_PP:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_CS:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_JD:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_L:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_V:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_C:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_WH:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_SM:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;
			case TYPE_SF:
			{
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2300);
			}
			break;

			///////////////////////////////////////
			case TYPE_T:  //注意国债时间没更正
			{
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
			}
			break;
			case TYPE_TF:
			{
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
			}
			break;
			///////////////////////////////////////
			case TYPE_IC:
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
				break;
			case TYPE_IF:
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
				break;
			case TYPE_IH:
				Timemore0 = thistime != 0.0925 && thistime != 0.0930 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = true;// thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime > 0.0900 && thistime<0.1500);// || (thistime>0.2100 && thistime < 0.2330);
				break;
				////////////////////////////////////////
			case TYPE_I:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_TA:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_A:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_M:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_Y:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_P:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
			case TYPE_MA:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;


			case TYPE_FG:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2330;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2330);
				break;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case TYPE_RU:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2300);
				break;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case TYPE_NI:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;
				break;
			case TYPE_ZN:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;
				break;
			case TYPE_AL:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;
				break;
			case TYPE_CU:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;
				break;
			case TYPE_BU:
				//Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				//Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				//Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;

				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2300);
				break;
			case TYPE_RB:
				//Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				//Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0100;
				//Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0100;

				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.2300;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || (thistime>0.2100 && thistime<0.2300);
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////
			case TYPE_AU:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0230;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0230;
				break;
			case TYPE_AG:
				Timemore0 = thistime != 0.0859 && thistime != 0.0900 && thistime != 0.1015 && thistime != 0.1130 && thistime != 0.1500;
				Timemore1 = thistime != 0.2059 && thistime != 0.2100 && thistime != 0.0230;
				Timemore2 = (thistime>0.0900 && thistime<0.1500) || thistime>0.2100 || thistime<0.0230;
				break;
			case TYPE_NEW1:

				break;
			case TYPE_NEW2:

				break;
			case TYPE_NEW3:

				break;
			case TYPE_NEW4:

				break;
			case TYPE_NEW5:

				break;
			case TYPE_NEW6:

				break;
			case TYPE_NEW7:

				break;
			case TYPE_NEW8:

				break;
			case TYPE_NEW9:

				break;
			case TYPE_NEW10:

				break;
			case TYPE_NEW11:

				break;
			case TYPE_NEW12:

				break;
			case TYPE_NEW13:

				break;
			case TYPE_NEW14:

				break;
			case TYPE_NEW15:

				break;
			case TYPE_NEW16:

				break;
			case TYPE_NEW17:

				break;
			case TYPE_NEW18:

				break;
			case TYPE_NEW19:

				break;
			case TYPE_NEW20:

				break;

				///////////////////////////////////////////////////////////////////////////////////////////////////////////
			}

			//printf("start......\n");


			//it->instructment
			//if (Timemore0)
			//printf ("pass 1\n");
			//if (Timemore1)
			//printf ("pass 2\n");
			//if (Timemore2)
			//printf("pass 3\n");
			//if ((seconds >= 0 && seconds < 40))
			//printf ("pass 4\n");
			//if ((pDepthMarketData->Volume > q->tick_VolumeLast))
			//printf ("pass 5\n");
			//if (q->M1Klinesig == false)
			//printf ("pass 6\n");

			if (Timemore0 && Timemore1  && Timemore2 && seconds >= 0 && seconds<40 && pDepthMarketData->Volume>q->tick_VolumeLast && q->M1Klinesig == false)
			{
				//printf("************************更新**********************\n\n");

				//char th[20] = { 0 };
				//sprintf_s(th,sizeof(th),"%d   %0.06f", q->period_M1, Q_BarTime_2);
				//WirteSingleRecordToFile8(0, th);


				q->tick_VolumeLast = pDepthMarketData->Volume;

				q->M1Klinesig = true;



				if (q->period_M1)
				{

					//printf("%s 更新M1\n", InstrumentID);

					for (int j = max_pure_m1_move_num; j >0; j--) //1~59执行了
					{


						//printf("M1 pDepthMarketData->LastPrice[%d][%0.02f][%0.02f][%0.02f][%0.02f]\n",j, q->M1_open[j], q->M1_high[j], q->M1_low[j], q->M1_close[j]);

						
						//if (M1_open[j - 1] > 1e-10 && M1_high[j - 1] > 1e-10 && M1_low[j - 1] > 1e-10  && M1_close[j - 1] > 1e-10   && M1_BS_Time[j - 1] > 1e-10 && M1_BS_BUY_Value[j - 1] > 1e-10 && M1_BS_SELL_Value[j - 1] > 1e-10)
						if (CheckData(q->M1_open[j - 1]) && CheckData(q->M1_high[j - 1]) && CheckData(q->M1_low[j - 1]) && CheckData(q->M1_close[j - 1]) && CheckDataOnlyNum(q->M1_BS_Time[j - 1]) && q->M1_BS_Time[j - 1]>-0.5
							&& CheckDataOnlyNum(q->M1_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M1_BS_SELL_Value[j - 1])
							)

						{

							q->M1_open[j] = q->M1_open[j - 1];
							q->M1_high[j] = q->M1_high[j - 1];
							q->M1_low[j] = q->M1_low[j - 1];
							q->M1_close[j] = q->M1_close[j - 1];
							q->M1_BS_Time[j] = q->M1_BS_Time[j - 1];

							q->M1_BS_PURE_Value[j] = q->M1_BS_PURE_Value[j - 1];
							q->M1_BS_BUY_Value[j] = q->M1_BS_BUY_Value[j - 1];
							q->M1_BS_SELL_Value[j] = q->M1_BS_SELL_Value[j - 1];
							q->M1_BS_BL_Value[j] = q->M1_BS_BL_Value[j - 1];

							q->M1_BS_SMALL_PURE_Value[j] = q->M1_BS_SMALL_PURE_Value[j - 1];
							q->M1_BS_SMALL_BUY_Value[j] = q->M1_BS_SMALL_BUY_Value[j - 1];
							q->M1_BS_SMALL_SELL_Value[j] = q->M1_BS_SMALL_SELL_Value[j - 1];
							q->M1_BS_SMALL_BL_Value[j] = q->M1_BS_SMALL_BL_Value[j - 1];

							q->M1_BS_MIDDLE_PURE_Value[j] = q->M1_BS_MIDDLE_PURE_Value[j - 1];
							q->M1_BS_MIDDLE_BUY_Value[j] = q->M1_BS_MIDDLE_BUY_Value[j - 1];
							q->M1_BS_MIDDLE_SELL_Value[j] = q->M1_BS_MIDDLE_SELL_Value[j - 1];
							q->M1_BS_MIDDLE_BL_Value[j] = q->M1_BS_MIDDLE_BL_Value[j - 1];

							q->M1_BS_BIG_PURE_Value[j] = q->M1_BS_BIG_PURE_Value[j - 1];
							q->M1_BS_BIG_BUY_Value[j] = q->M1_BS_BIG_BUY_Value[j - 1];
							q->M1_BS_BIG_SELL_Value[j] = q->M1_BS_BIG_SELL_Value[j - 1];
							q->M1_BS_BIG_BL_Value[j] = q->M1_BS_BIG_BL_Value[j - 1];

							q->M1_BS_TOTAL_PURE_Value[j] = q->M1_BS_TOTAL_PURE_Value[j - 1];
							q->M1_BS_TOTAL_BUY_Value[j] = q->M1_BS_TOTAL_BUY_Value[j - 1];
							q->M1_BS_TOTAL_SELL_Value[j] = q->M1_BS_TOTAL_SELL_Value[j - 1];
							q->M1_BS_TOTAL_BL_Value[j] = q->M1_BS_TOTAL_BL_Value[j - 1];

							q->M1_BS_TOTAL_SMALL_PURE_Value[j] = q->M1_BS_TOTAL_SMALL_PURE_Value[j - 1];
							q->M1_BS_TOTAL_SMALL_BUY_Value[j] = q->M1_BS_TOTAL_SMALL_BUY_Value[j - 1];
							q->M1_BS_TOTAL_SMALL_SELL_Value[j] = q->M1_BS_TOTAL_SMALL_SELL_Value[j - 1];
							q->M1_BS_TOTAL_SMALL_BL_Value[j] = q->M1_BS_TOTAL_SMALL_BL_Value[j - 1];


							q->M1_BS_TOTAL_MIDDLE_PURE_Value[j] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[j - 1];
							q->M1_BS_TOTAL_MIDDLE_BUY_Value[j] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[j - 1];
							q->M1_BS_TOTAL_MIDDLE_SELL_Value[j] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[j - 1];
							q->M1_BS_TOTAL_MIDDLE_BL_Value[j] = q->M1_BS_TOTAL_MIDDLE_BL_Value[j - 1];

							q->M1_BS_TOTAL_BIG_PURE_Value[j] = q->M1_BS_TOTAL_BIG_PURE_Value[j - 1];
							q->M1_BS_TOTAL_BIG_BUY_Value[j] = q->M1_BS_TOTAL_BIG_BUY_Value[j - 1];
							q->M1_BS_TOTAL_BIG_SELL_Value[j] = q->M1_BS_TOTAL_BIG_SELL_Value[j - 1];
							q->M1_BS_TOTAL_BIG_BL_Value[j] = q->M1_BS_TOTAL_BIG_BL_Value[j - 1];

							q->M1_BS_CROSS_TOTAL_PURE_Value[j] = q->M1_BS_CROSS_TOTAL_PURE_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BUY_Value[j] = q->M1_BS_CROSS_TOTAL_BUY_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_SELL_Value[j] = q->M1_BS_CROSS_TOTAL_SELL_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BL_Value[j] = q->M1_BS_CROSS_TOTAL_BL_Value[j - 1];

							q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[j] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[j] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[j] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_SMALL_BL_Value[j] = q->M1_BS_CROSS_TOTAL_SMALL_BL_Value[j - 1];

							q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[j] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[j] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[j] = q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_MIDDLE_BL_Value[j] = q->M1_BS_CROSS_TOTAL_MIDDLE_BL_Value[j - 1];

							q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[j] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[j] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[j] = q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[j - 1];
							q->M1_BS_CROSS_TOTAL_BIG_BL_Value[j] = q->M1_BS_CROSS_TOTAL_BIG_BL_Value[j - 1];

							bool tempdata = true; //临时声明，应该删除声明
						}
						else if (_isnan(q->M1_open[j - 1]) || _isnan(q->M1_high[j - 1]) || _isnan(q->M1_low[j - 1]) || _isnan(q->M1_close[j - 1]) || _isnan(q->M1_BS_Time[j - 1]) || _isnan(q->M1_BS_BUY_Value[j - 1]) || _isnan(q->M1_BS_SELL_Value[j - 1]))
						{

							if (j - 1 < 4)
							{
								//printf("错误(不是数字) [%d][%0.06f] M1_open[%0.02f] M1_high[%0.02f] M1_low[%0.02f] M1_close[%0.02f] M1_BS_Time[%0.02f] M1_BS_BUY_Value[%0.02f] M1_BS_SELL_Value[%0.02f]\n", j - 1, Q_BarTime_2, M1_open[j - 1], M1_high[j - 1], M1_low[j - 1], M1_close[j - 1], M1_BS_Time[j - 1], M1_BS_BUY_Value[j - 1], M1_BS_SELL_Value[j - 1]);

								//char chr[150] = { 0 };
								//sprintf_s(chr, sizeof(chr), "错误(不是数字) [%d][%s]M1_open[%0.02f] M1_high[%0.02f] M1_low[%0.02f] M1_close[%0.02f] M1_BS_Time[%0.02f] M1_BS_BUY_Value[%0.02f] M1_BS_SELL_Value[%0.02f]", j - 1, thistime, q->M1_open[j - 1], q->M1_high[j - 1], q->M1_low[j - 1], q->M1_close[j - 1], q->M1_BS_Time[j - 1], q->M1_BS_BUY_Value[j - 1], q->M1_BS_SELL_Value[j - 1]);
								//WirteTradeRecordToFileMainThreadB(0, chr);

							}

						}


						
					}


					char tttt[50] = { 0 };
					_snprintf_s(tttt, sizeof(tttt), sizeof(tttt), "%s", InstrumentID);
					WirteSingleRecordToFile8(0, tttt);

					for (int i = 1; i < 20; i++)
					{
						//if(q->M1_open[i]>1e-10 || q->M1_close[i]>1e-10 || q->M1_high[i]>1e-10 || q->M1_low[i]>1e-10)
						if (q->M1_open[i]>1e-10)
						{
							printf("M1:[%d]开盘[%0.02f]收盘[%0.02f]最高[%0.02f]最低[%0.02f]\n", i, q->M1_open[i], q->M1_close[i], q->M1_high[i], q->M1_low[i]);

							char th[100] = { 0 };
							_snprintf_s(th, sizeof(th), sizeof(th), "[%d]%0.06f M1 open[%0.02f]close[%0.02f]high[%0.02f]low[%0.02f]", i, q->M1_BS_Time[i], q->M1_open[i], q->M1_close[i], q->M1_high[i], q->M1_low[i]);
							WirteSingleRecordToFile8(0, th);
						}
					}
					printf("\n");
					WirteSingleRecordToFile8(0, "");
					for (int i = 1; i < 5; i++)
					{
						//if (q->M3_open[i] > 1e-10 || q->M3_close[i] > 1e-10 || q->M3_high[i] > 1e-10 || q->M3_low[i] > 1e-10) 
						if (q->M3_open[i]>1e-10)
						{
							printf("M3:[%d]开盘[%0.02f]收盘[%0.02f]最高[%0.02f]最低[%0.02f]\n", i, q->M3_open[i], q->M3_close[i], q->M3_high[i], q->M3_low[i]);

							char th[100] = { 0 };
							_snprintf_s(th, sizeof(th), sizeof(th), "[%d]%0.06f M3 open[%0.02f]close[%0.02f]high[%0.02f]low[%0.02f]", i, q->M3_BS_Time[i], q->M3_open[i], q->M3_close[i], q->M3_high[i], q->M3_low[i]);
							WirteSingleRecordToFile8(0, th);
						}



					}
					printf("\n");
					WirteSingleRecordToFile8(0, "");

					for (int i = 1; i < 5; i++)
					{
						//if (q->M5_open[i]>1e-10 || q->M5_close[i]>1e-10 || q->M5_high[i]>1e-10 || q->M5_low[i]>1e-10)
						if (q->M5_open[i]>1e-10)
						{
							printf("M5:[%d]开盘[%0.02f]收盘[%0.02f]最高[%0.02f]最低[%0.02f]\n", i, q->M5_open[i], q->M5_close[i], q->M5_high[i], q->M5_low[i]);

							char th[100] = { 0 };
							_snprintf_s(th, sizeof(th), sizeof(th), "[%d]%0.06f M5 open[%0.02f]close[%0.02f]high[%0.02f]low[%0.02f]", i, q->M5_BS_Time[i], q->M5_open[i], q->M5_close[i], q->M5_high[i], q->M5_low[i]);
							WirteSingleRecordToFile8(0, th);
						}



					}
















				}

				int tempm = 0;
				if (q->period_M3)
				{
					//printf("%s 更新M3\n", InstrumentID);
					//M3更新
					int tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M3_BS_Time[1]);
					if (NotFirst_3M)
					{

						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M3_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M3_BS_Time[1]));
						//if (tempm >= 3-1 || (tempm<0 && tempm >= -1437-1))
						if (tempm >= 3 || (tempm<0 && tempm >= -1437))
						{
							for (int j = max_pure_m3_move_num; j >1; j--) //2~59执行了
							{
								
								//if (M3_open[j - 1] > 1e-10 && M3_high[j - 1] > 1e-10 && M3_low[j - 1] > 1e-10  &&  M3_close[j - 1] > 1e-10 && M3_BS_Time[j - 1] > 1e-10 && M3_BS_BUY_Value[j - 1] > 1e-10 && M3_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M3_open[j - 1]) && CheckData(q->M3_high[j - 1]) && CheckData(q->M3_low[j - 1]) && CheckData(q->M3_close[j - 1]) && CheckDataOnlyNum(q->M3_BS_Time[j - 1]) && q->M3_BS_Time[j - 1]>-0.5

									&& CheckDataOnlyNum(q->M3_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M3_BS_SELL_Value[j - 1])
									)

								{
									q->M3_open[j] = q->M3_open[j - 1];
									q->M3_high[j] = q->M3_high[j - 1];
									q->M3_low[j] = q->M3_low[j - 1];
									q->M3_close[j] = q->M3_close[j - 1];
									q->M3_BS_Time[j] = q->M3_BS_Time[j - 1];

									q->M3_BS_PURE_Value[j] = q->M3_BS_PURE_Value[j - 1];
									q->M3_BS_BUY_Value[j] = q->M3_BS_BUY_Value[j - 1];
									q->M3_BS_SELL_Value[j] = q->M3_BS_SELL_Value[j - 1];
									q->M3_BS_BL_Value[j] = q->M3_BS_BL_Value[j - 1];

									q->M3_BS_BIG_PURE_Value[j] = q->M3_BS_BIG_PURE_Value[j - 1];
									q->M3_BS_BIG_BUY_Value[j] = q->M3_BS_BIG_BUY_Value[j - 1];
									q->M3_BS_BIG_SELL_Value[j] = q->M3_BS_BIG_SELL_Value[j - 1];
									q->M3_BS_BIG_BL_Value[j] = q->M3_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}
								
							}

							//比M1增加的处理部分
							q->M3_open[1] = q->M1_open[3];//M3_open[0];
							q->M3_high[1] = zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1])); //M3_high[j - 1];
							q->M3_low[1] = zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])); //M3_low[j - 1];
							q->M3_close[1] = q->M1_close[1]; //M3_close[j - 1];
							q->M3_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //M1_BS_Time[0];//M3_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M3_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// M3_BS_PURE_Value[j - 1];
							q->M3_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// M3_BS_BUY_Value[j - 1];
							q->M3_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//M3_BS_SELL_Value[j - 1];
							q->M3_BS_BL_Value[1] = q->M3_BS_BUY_Value[1] / q->M3_BS_SELL_Value[1];   //M3_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M3_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// M3_BS_BIG_PURE_Value[j - 1];
							q->M3_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// M3_BS_BIG_BUY_Value[j - 1];
							q->M3_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// M3_BS_BIG_SELL_Value[j - 1];
							q->M3_BS_BIG_BL_Value[1] = q->M3_BS_BIG_BUY_Value[1] / q->M3_BS_BIG_SELL_Value[1];// M3_BS_BIG_BL_Value[j - 1];
																											  //比M1增加的处理部分
																											  // int id = i;
																											  //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]M3_BS_Time[%0.02f] M3_close[%0.02f] M3_BS_PURE_Value[%0.02f] M3_BS_BUY_Value[%0.02f] M3_BS_SELL_Value[%0.02f] \n",
																											  //  tick_data[2], InstrumentID_n[id], i, M3_BS_Time[id][1], M3_close[id][1], M3_BS_PURE_Value[id][1], M3_BS_BUY_Value[id][1], M3_BS_SELL_Value[id][1]);
						}


					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 3 || (tempm<0 && tempm >= -1437)) && CheckFirst(q->M1_BS_Time[1], 3))
						{

							NotFirst_3M = true;

						}
					}
					//M3更新		
				}


				if (q->period_M5)
				{
					//printf("%s 更新M5\n", InstrumentID);
					//M5更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M5_BS_Time[1]);
					if (NotFirst_5M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M5_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M5_BS_Time[1]));
						//if (tempm >= 5-1 || (tempm < 0 && tempm >= -1435-1))

						if (tempm >= 5 || (tempm < 0 && tempm >= -1435))
						{
							for (int j = max_pure_m5_move_num; j > 1; j--) //2~59执行了
							{
								
								//if (M5_open[j - 1] > 1e-10 && M5_high[j - 1] > 1e-10 && M5_low[j - 1] > 1e-10  &&  M5_close[j - 1] > 1e-10 && M5_BS_Time[j - 1] > 1e-10 && M5_BS_BUY_Value[j - 1] > 1e-10 && M5_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M5_open[j - 1]) && CheckData(q->M5_high[j - 1]) && CheckData(q->M5_low[j - 1]) && CheckData(q->M5_close[j - 1]) && CheckDataOnlyNum(q->M5_BS_Time[j - 1]) && q->M5_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M5_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M5_BS_SELL_Value[j - 1])
									)

								{
									q->M5_open[j] = q->M5_open[j - 1];
									q->M5_high[j] = q->M5_high[j - 1];
									q->M5_low[j] = q->M5_low[j - 1];
									q->M5_close[j] = q->M5_close[j - 1];
									q->M5_BS_Time[j] = q->M5_BS_Time[j - 1];

									q->M5_BS_PURE_Value[j] = q->M5_BS_PURE_Value[j - 1];
									q->M5_BS_BUY_Value[j] = q->M5_BS_BUY_Value[j - 1];
									q->M5_BS_SELL_Value[j] = q->M5_BS_SELL_Value[j - 1];
									q->M5_BS_BL_Value[j] = q->M5_BS_BL_Value[j - 1];

									q->M5_BS_BIG_PURE_Value[j] = q->M5_BS_BIG_PURE_Value[j - 1];
									q->M5_BS_BIG_BUY_Value[j] = q->M5_BS_BIG_BUY_Value[j - 1];
									q->M5_BS_BIG_SELL_Value[j] = q->M5_BS_BIG_SELL_Value[j - 1];
									q->M5_BS_BIG_BL_Value[j] = q->M5_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}
								
							}

							//比M1增加的处理部分
							q->M5_open[1] = q->M1_open[5];//M5_open[0];
							q->M5_high[1] = zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))));
							q->M5_low[1] = zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))));
							q->M5_close[1] = q->M1_close[1]; //M5_close[j - 1];
							q->M5_BS_Time[1] = q->M1_BS_Time[1]; //5个周期的最后1个（结束时间）   //M1_BS_Time[0];//M5_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确


																 //未修正
							q->M5_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// M5_BS_PURE_Value[j - 1];
							q->M5_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// M5_BS_BUY_Value[j - 1];
							q->M5_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//M5_BS_SELL_Value[j - 1];
							q->M5_BS_BL_Value[1] = q->M5_BS_BUY_Value[1] / q->M5_BS_SELL_Value[1];   //M5_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M5_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// M5_BS_BIG_PURE_Value[j - 1];
							q->M5_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// M5_BS_BIG_BUY_Value[j - 1];
							q->M5_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// M5_BS_BIG_SELL_Value[j - 1];
							q->M5_BS_BIG_BL_Value[1] = q->M5_BS_BIG_BUY_Value[1] / q->M5_BS_BIG_SELL_Value[1];// M5_BS_BIG_BL_Value[j - 1];

																											  //未修正
																											  //比M1增加的处理部分
																											  // int id = i;
																											  //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]M5_BS_Time[%0.02f] M5_close[%0.02f] M5_BS_PURE_Value[%0.02f] M5_BS_BUY_Value[%0.02f] M5_BS_SELL_Value[%0.02f] \n",
																											  //  tick_data[2], InstrumentID_n[id], i, M5_BS_Time[id][1], M5_close[id][1], M5_BS_PURE_Value[id][1], M5_BS_BUY_Value[id][1], M5_BS_SELL_Value[id][1]);
						}
					}
					else
					{

						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 5 || (tempm<0 && tempm >= -1435)) && CheckFirst(q->M1_BS_Time[1], 5))
						{

							NotFirst_5M = true;

						}


					}
					//M5更新		

				}


				if (q->period_M10)
				{
					//printf("%s 更新M10\n", InstrumentID);
					//M10更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M10_BS_Time[1]);
					if (NotFirst_10M)
					{

						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M10_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M10_BS_Time[1]));
						if (tempm >= 10 || (tempm < 0 && tempm >= -1430))
						{
							for (int j = max_pure_m10_move_num; j > 1; j--) //2~59执行了
							{
								
								//if (M10_open[j - 1] > 1e-10 && M10_high[j - 1] > 1e-10 && M10_low[j - 1] > 1e-10  &&  M10_close[j - 1] > 1e-10 && M10_BS_Time[j - 1] > 1e-10 && M10_BS_BUY_Value[j - 1] > 1e-10 && M10_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M10_open[j - 1]) && CheckData(q->M10_high[j - 1]) && CheckData(q->M10_low[j - 1]) && CheckData(q->M10_close[j - 1]) && CheckDataOnlyNum(q->M10_BS_Time[j - 1]) && q->M10_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M10_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M10_BS_SELL_Value[j - 1])
									)

								{
									q->M10_open[j] = q->M10_open[j - 1];
									q->M10_high[j] = q->M10_high[j - 1];
									q->M10_low[j] = q->M10_low[j - 1];
									q->M10_close[j] = q->M10_close[j - 1];
									q->M10_BS_Time[j] = q->M10_BS_Time[j - 1];

									q->M10_BS_PURE_Value[j] = q->M10_BS_PURE_Value[j - 1];
									q->M10_BS_BUY_Value[j] = q->M10_BS_BUY_Value[j - 1];
									q->M10_BS_SELL_Value[j] = q->M10_BS_SELL_Value[j - 1];
									q->M10_BS_BL_Value[j] = q->M10_BS_BL_Value[j - 1];

									q->M10_BS_BIG_PURE_Value[j] = q->M10_BS_BIG_PURE_Value[j - 1];
									q->M10_BS_BIG_BUY_Value[j] = q->M10_BS_BIG_BUY_Value[j - 1];
									q->M10_BS_BIG_SELL_Value[j] = q->M10_BS_BIG_SELL_Value[j - 1];
									q->M10_BS_BIG_BL_Value[j] = q->M10_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}
								
							}

							//比M1增加的处理部分
							q->M10_open[1] = q->M1_open[10];//q->M10_open[0];
							q->M10_high[1] = zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))))))))); //q->M10_high[j - 1];
							q->M10_low[1] = zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))))))))); //q->M10_low[j - 1];
							q->M10_close[1] = q->M1_close[1]; //q->M10_close[j - 1];
							q->M10_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //M1_BS_Time[0];//q->M10_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M10_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M10_BS_PURE_Value[j - 1];
							q->M10_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M10_BS_BUY_Value[j - 1];
							q->M10_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M10_BS_SELL_Value[j - 1];
							q->M10_BS_BL_Value[1] = q->M10_BS_BUY_Value[1] / q->M10_BS_SELL_Value[1];   //q->M10_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M10_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M10_BS_BIG_PURE_Value[j - 1];
							q->M10_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M10_BS_BIG_BUY_Value[j - 1];
							q->M10_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M10_BS_BIG_SELL_Value[j - 1];
							q->M10_BS_BIG_BL_Value[1] = q->M10_BS_BIG_BUY_Value[1] / q->M10_BS_BIG_SELL_Value[1];// q->M10_BS_BIG_BL_Value[j - 1];
																												 //比M1增加的处理部分
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M10_BS_Time[%0.02f] q->M10_close[%0.02f] q->M10_BS_PURE_Value[%0.02f] q->M10_BS_BUY_Value[%0.02f] q->M10_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M10_BS_Time[id][1], q->M10_close[id][1], q->M10_BS_PURE_Value[id][1], q->M10_BS_BUY_Value[id][1], q->M10_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 10 || (tempm<0 && tempm >= -1430)) && CheckFirst(q->M1_BS_Time[1], 10))
						{

							NotFirst_10M = true;

						}
					}
					//M10更新


				}

				if (q->period_M15)
				{
					//printf("%s 更新M15\n", InstrumentID);
					//M15更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M15_BS_Time[1]);
					if (NotFirst_15M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M15_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M15_BS_Time[1]));
						if (tempm >= 15 || (tempm < 0 && tempm >= -1425))
						{
							for (int j = max_pure_m15_move_num; j > 1; j--) //2~59执行了
							{
								
								//if (M15_open[j - 1] > 1e-10 && M15_high[j - 1] > 1e-10 && M15_low[j - 1] > 1e-10  &&  M15_close[j - 1] > 1e-10 && M15_BS_Time[j - 1] > 1e-10 && M15_BS_BUY_Value[j - 1] > 1e-10 && M15_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M15_open[j - 1]) && CheckData(q->M15_high[j - 1]) && CheckData(q->M15_low[j - 1]) && CheckData(q->M15_close[j - 1]) && CheckDataOnlyNum(q->M15_BS_Time[j - 1]) && q->M15_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M15_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M15_BS_SELL_Value[j - 1])
									)

								{
									q->M15_open[j] = q->M15_open[j - 1];
									q->M15_high[j] = q->M15_high[j - 1];
									q->M15_low[j] = q->M15_low[j - 1];
									q->M15_close[j] = q->M15_close[j - 1];
									q->M15_BS_Time[j] = q->M15_BS_Time[j - 1];

									q->M15_BS_PURE_Value[j] = q->M15_BS_PURE_Value[j - 1];
									q->M15_BS_BUY_Value[j] = q->M15_BS_BUY_Value[j - 1];
									q->M15_BS_SELL_Value[j] = q->M15_BS_SELL_Value[j - 1];
									q->M15_BS_BL_Value[j] = q->M15_BS_BL_Value[j - 1];

									q->M15_BS_BIG_PURE_Value[j] = q->M15_BS_BIG_PURE_Value[j - 1];
									q->M15_BS_BIG_BUY_Value[j] = q->M15_BS_BIG_BUY_Value[j - 1];
									q->M15_BS_BIG_SELL_Value[j] = q->M15_BS_BIG_SELL_Value[j - 1];
									q->M15_BS_BIG_BL_Value[j] = q->M15_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}
								
							}

							//比M1增加的处理部分
							q->M15_open[1] = q->M1_open[15];//M15_open[0];

							q->M15_high[1] = zero_max(q->M1_high[15], zero_max(q->M1_high[14], zero_max(q->M1_high[13], zero_max(q->M1_high[12], zero_max(q->M1_high[11], zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1])))))))))))))); //q->M10_high[j - 1];
							q->M15_low[1] = zero_min(q->M1_low[15], zero_min(q->M1_low[14], zero_min(q->M1_low[13], zero_min(q->M1_low[12], zero_min(q->M1_low[11], zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])))))))))))))); //M15_low[j - 1];
							q->M15_close[1] = q->M1_close[1]; //q->M15_close[j - 1];
							q->M15_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //q->M1_BS_Time[0];//q->M15_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M15_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M15_BS_PURE_Value[j - 1];
							q->M15_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M15_BS_BUY_Value[j - 1];
							q->M15_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M15_BS_SELL_Value[j - 1];
							q->M15_BS_BL_Value[1] = q->M15_BS_BUY_Value[1] / q->M15_BS_SELL_Value[1];   //q->M15_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M15_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M15_BS_BIG_PURE_Value[j - 1];
							q->M15_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M15_BS_BIG_BUY_Value[j - 1];
							q->M15_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M15_BS_BIG_SELL_Value[j - 1];
							q->M15_BS_BIG_BL_Value[1] = q->M15_BS_BIG_BUY_Value[1] / q->M15_BS_BIG_SELL_Value[1];// q->M15_BS_BIG_BL_Value[j - 1];
																												 //比M1增加的处理部分
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M15_BS_Time[%0.02f] q->M15_close[%0.02f] q->M15_BS_PURE_Value[%0.02f] q->M15_BS_BUY_Value[%0.02f] q->M15_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M15_BS_Time[id][1], q->M15_close[id][1], q->M15_BS_PURE_Value[id][1], q->M15_BS_BUY_Value[id][1], q->M15_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 15 || (tempm<0 && tempm >= -1425)) && CheckFirst(q->M1_BS_Time[1], 15))
						{

							NotFirst_15M = true;

						}
					}
					//M15更新			

				}


				if (q->period_M30)
				{

					//M30更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M30_BS_Time[1]);
					if (NotFirst_30M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] q->M1_BS_Time[%0.06f] M30_BS_Time[%0.06f]\n", tempm, (q->M1_BS_Time[1]), (M30_BS_Time[1]));
						if (tempm >= 30 || (tempm < 0 && tempm >= -1410))
						{
							for (int j = max_pure_m30_move_num; j > 1; j--) //2~59执行了
							{
								
								//if (M30_open[j - 1] > 1e-10 && M30_high[j - 1] > 1e-10 && M30_low[j - 1] > 1e-10  &&  M30_close[j - 1] > 1e-10 && M30_BS_Time[j - 1] > 1e-10 && M30_BS_BUY_Value[j - 1] > 1e-10 && M30_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M30_open[j - 1]) && CheckData(q->M30_high[j - 1]) && CheckData(q->M30_low[j - 1]) && CheckData(q->M30_close[j - 1]) && CheckDataOnlyNum(q->M30_BS_Time[j - 1]) && q->M30_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M30_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M30_BS_SELL_Value[j - 1])
									)

								{
									q->M30_open[j] = q->M30_open[j - 1];
									q->M30_high[j] = q->M30_high[j - 1];
									q->M30_low[j] = q->M30_low[j - 1];
									q->M30_close[j] = q->M30_close[j - 1];
									q->M30_BS_Time[j] = q->M30_BS_Time[j - 1];

									q->M30_BS_PURE_Value[j] = q->M30_BS_PURE_Value[j - 1];
									q->M30_BS_BUY_Value[j] = q->M30_BS_BUY_Value[j - 1];
									q->M30_BS_SELL_Value[j] = q->M30_BS_SELL_Value[j - 1];
									q->M30_BS_BL_Value[j] = q->M30_BS_BL_Value[j - 1];

									q->M30_BS_BIG_PURE_Value[j] = q->M30_BS_BIG_PURE_Value[j - 1];
									q->M30_BS_BIG_BUY_Value[j] = q->M30_BS_BIG_BUY_Value[j - 1];
									q->M30_BS_BIG_SELL_Value[j] = q->M30_BS_BIG_SELL_Value[j - 1];
									q->M30_BS_BIG_BL_Value[j] = q->M30_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}
								
							}

							//比M1增加的处理部分
							q->M30_open[1] = q->M1_open[30];//M30_open[0];
															//M30_high[1] = max(q->M1_high[3], max(q->M1_high[2], q->M1_high[1])); //M30_high[j - 1];
															//M30_low[1] = zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])); //M30_low[j - 1];

							q->M30_high[1] = zero_max(q->M1_high[30], zero_max(q->M1_high[29], zero_max(q->M1_high[28], zero_max(q->M1_high[27], zero_max(q->M1_high[26], zero_max(q->M1_high[25], zero_max(q->M1_high[24], zero_max(q->M1_high[23], zero_max(q->M1_high[22], zero_max(q->M1_high[21], zero_max(q->M1_high[20], zero_max(q->M1_high[19], zero_max(q->M1_high[18], zero_max(q->M1_high[17], zero_max(q->M1_high[16], zero_max(q->M1_high[15], zero_max(q->M1_high[14], zero_max(q->M1_high[13], zero_max(q->M1_high[12], zero_max(q->M1_high[11], zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))))))))))))))))))))))))))))); //q->M15_high[j - 1];
							q->M30_low[1] = zero_min(q->M1_low[30], zero_min(q->M1_low[29], zero_min(q->M1_low[28], zero_min(q->M1_low[27], zero_min(q->M1_low[26], zero_min(q->M1_low[25], zero_min(q->M1_low[24], zero_min(q->M1_low[23], zero_min(q->M1_low[22], zero_min(q->M1_low[21], zero_min(q->M1_low[20], zero_min(q->M1_low[19], zero_min(q->M1_low[18], zero_min(q->M1_low[17], zero_min(q->M1_low[16], zero_min(q->M1_low[15], zero_min(q->M1_low[14], zero_min(q->M1_low[13], zero_min(q->M1_low[12], zero_min(q->M1_low[11], zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))))))))))))))))))))))))))))); //q->M15_low[j - 1];

							q->M30_close[1] = q->M1_close[1]; //q->M30_close[j - 1];
							q->M30_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //q->M1_BS_Time[0];//q->M30_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M30_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M30_BS_PURE_Value[j - 1];
							q->M30_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M30_BS_BUY_Value[j - 1];
							q->M30_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M30_BS_SELL_Value[j - 1];
							q->M30_BS_BL_Value[1] = q->M30_BS_BUY_Value[1] / q->M30_BS_SELL_Value[1];   //q->M30_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M30_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M30_BS_BIG_PURE_Value[j - 1];
							q->M30_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M30_BS_BIG_BUY_Value[j - 1];
							q->M30_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M30_BS_BIG_SELL_Value[j - 1];
							q->M30_BS_BIG_BL_Value[1] = q->M30_BS_BIG_BUY_Value[1] / q->M30_BS_BIG_SELL_Value[1];// q->M30_BS_BIG_BL_Value[j - 1];
																												 //比M1增加的处理部分
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M30_BS_Time[%0.02f] q->M30_close[%0.02f] q->M30_BS_PURE_Value[%0.02f] q->M30_BS_BUY_Value[%0.02f] q->M30_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M30_BS_Time[id][1], q->M30_close[id][1], q->M30_BS_PURE_Value[id][1], q->M30_BS_BUY_Value[id][1], q->M30_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 30 || (tempm<0 && tempm >= -1410)) && CheckFirst(q->M1_BS_Time[1], 30))
						{
							NotFirst_30M = true;
						}
					}
					//M30更新			

				}

				if (q->period_M60)
				{

					//M60更新
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M60_BS_Time[1]);
					if (NotFirst_60M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] q->M1_BS_Time[%0.06f] q->M60_BS_Time[%0.06f]\n", tempm, (q->M1_BS_Time[1]), (q->M60_BS_Time[1]));
						if (tempm >= 60 || (tempm < 0 && tempm >= -1380))
						{
							for (int j = max_pure_m60_move_num; j > 1; j--) //2~59执行了
							{
								
								//if (M60_open[j - 1] > 1e-10 && M60_high[j - 1] > 1e-10 && M60_low[j - 1] > 1e-10  &&  M60_close[j - 1] > 1e-10 && M60_BS_Time[j - 1] > 1e-10 && M60_BS_BUY_Value[j - 1] > 1e-10 && M60_BS_SELL_Value[j - 1] > 1e-10)

								if (CheckData(q->M60_open[j - 1]) && CheckData(q->M60_high[j - 1]) && CheckData(q->M60_low[j - 1]) && CheckData(q->M60_close[j - 1]) && CheckDataOnlyNum(q->M60_BS_Time[j - 1]) && q->M60_BS_Time[j - 1] > -0.5

									&& CheckDataOnlyNum(q->M60_BS_BUY_Value[j - 1]) && CheckDataOnlyNum(q->M60_BS_SELL_Value[j - 1])
									)

								{
									q->M60_open[j] = q->M60_open[j - 1];
									q->M60_high[j] = q->M60_high[j - 1];
									q->M60_low[j] = q->M60_low[j - 1];
									q->M60_close[j] = q->M60_close[j - 1];
									q->M60_BS_Time[j] = q->M60_BS_Time[j - 1];

									q->M60_BS_PURE_Value[j] = q->M60_BS_PURE_Value[j - 1];
									q->M60_BS_BUY_Value[j] = q->M60_BS_BUY_Value[j - 1];
									q->M60_BS_SELL_Value[j] = q->M60_BS_SELL_Value[j - 1];
									q->M60_BS_BL_Value[j] = q->M60_BS_BL_Value[j - 1];

									q->M60_BS_BIG_PURE_Value[j] = q->M60_BS_BIG_PURE_Value[j - 1];
									q->M60_BS_BIG_BUY_Value[j] = q->M60_BS_BIG_BUY_Value[j - 1];
									q->M60_BS_BIG_SELL_Value[j] = q->M60_BS_BIG_SELL_Value[j - 1];
									q->M60_BS_BIG_BL_Value[j] = q->M60_BS_BIG_BL_Value[j - 1];
									//tempdata = true;
								}
								
							}

							//比M1增加的处理部分
							q->M60_open[1] = q->M1_open[60];//q->M60_open[0];	
							q->M60_high[1] = zero_max(q->M1_high[60],
								zero_max(q->M1_high[59],
									zero_max(q->M1_high[58],
										zero_max(q->M1_high[57],
											zero_max(q->M1_high[56],
												zero_max(q->M1_high[55],
													zero_max(q->M1_high[54],
														zero_max(q->M1_high[53],
															zero_max(q->M1_high[52],
																zero_max(q->M1_high[51],
																	zero_max(q->M1_high[50],
																		zero_max(q->M1_high[49],
																			zero_max(q->M1_high[48],
																				zero_max(q->M1_high[47],
																					zero_max(q->M1_high[46],
																						zero_max(q->M1_high[45],
																							zero_max(q->M1_high[44],
																								zero_max(q->M1_high[43],
																									zero_max(q->M1_high[42],
																										zero_max(q->M1_high[41],
																											zero_max(q->M1_high[40],
																												zero_max(q->M1_high[39],
																													zero_max(q->M1_high[38],
																														zero_max(q->M1_high[37],
																															zero_max(q->M1_high[36],
																																zero_max(q->M1_high[35],
																																	zero_max(q->M1_high[34],
																																		zero_max(q->M1_high[33],
																																			zero_max(q->M1_high[32],
																																				zero_max(q->M1_high[31],
																																					q->M30_high[1]))))))))))))))))))))))))))))));


							q->M60_low[1] = zero_min(q->M1_low[60], zero_min(q->M1_low[59], zero_min(q->M1_low[58], zero_min(q->M1_low[57], zero_min(q->M1_low[56], zero_min(q->M1_low[55], zero_min(q->M1_low[54], zero_min(q->M1_low[53], zero_min(q->M1_low[52], zero_min(q->M1_low[51], zero_min(q->M1_low[50], zero_min(q->M1_low[49], zero_min(q->M1_low[48], zero_min(q->M1_low[47], zero_min(q->M1_low[46], zero_min(q->M1_low[45], zero_min(q->M1_low[44], zero_min(q->M1_low[43], zero_min(q->M1_low[42], zero_min(q->M1_low[41], zero_min(q->M1_low[40], zero_min(q->M1_low[39], zero_min(q->M1_low[38], zero_min(q->M1_low[37], zero_min(q->M1_low[36], zero_min(q->M1_low[35], zero_min(q->M1_low[34], zero_min(q->M1_low[33], zero_min(q->M1_low[32], zero_min(q->M1_low[31], q->M30_high[1])))))))))))))))))))))))))))))); //M15_low[j - 1];



							q->M60_close[1] = q->M1_close[1];
							q->M60_BS_Time[1] = q->M1_BS_Time[1]; //3个周期的最后1个（结束时间）   //q->M1_BS_Time[0];//q->M60_BS_Time[j - 1]; ?????????????????????????????????????????????是否正确

							q->M60_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M60_BS_PURE_Value[j - 1];
							q->M60_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M60_BS_BUY_Value[j - 1];
							q->M60_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M60_BS_SELL_Value[j - 1];
							q->M60_BS_BL_Value[1] = q->M60_BS_BUY_Value[1] / q->M60_BS_SELL_Value[1];   //q->M60_BS_BL_Value[j - 1]; ?????????????????????????是否正确

							q->M60_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M60_BS_BIG_PURE_Value[j - 1];
							q->M60_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M60_BS_BIG_BUY_Value[j - 1];
							q->M60_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M60_BS_BIG_SELL_Value[j - 1];
							q->M60_BS_BIG_BL_Value[1] = q->M60_BS_BIG_BUY_Value[1] / q->M60_BS_BIG_SELL_Value[1];// q->M60_BS_BIG_BL_Value[j - 1];
																												 //比M1增加的处理部分
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M60_BS_Time[%0.02f] q->M60_close[%0.02f] q->M60_BS_PURE_Value[%0.02f] q->M60_BS_BUY_Value[%0.02f] q->M60_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M60_BS_Time[id][1], q->M60_close[id][1], q->M60_BS_PURE_Value[id][1], q->M60_BS_BUY_Value[id][1], q->M60_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//检查分钟数/周期分钟数为0作为起始数据
						if ((tempm >= 60 || (tempm<0 && tempm >= -1380)) && CheckFirst(q->M1_BS_Time[1], 60))
						{
							NotFirst_60M = true;
						}
					}
					//M60更新		

				}






				//计算1M走势相关性,建议开一个新线程
				//if (tempdata == true)
				//{
				//恢复printf("\n计算相关性\n");
				//for (int k = 0;k<TYPE_NUM;k++)
				//	{
				//	CalRelativity(i, k);
				//	}

				//}

				//计算1M走势相关性，建议开一个新线程    

				//这个是否在挪位之前更新

				
				//printf("M1 pDepthMarketData->LastPrice[%0.02f][%0.02f][%0.02f][%0.02f]\n",q->M1_open[0],q->M1_high[0],q->M1_low[0], q->M1_close[0]);
				q->M1_open[0] = pDepthMarketData->LastPrice;
				q->M1_high[0] = pDepthMarketData->LastPrice;
				q->M1_low[0] = pDepthMarketData->LastPrice;
				q->M1_close[0] = pDepthMarketData->LastPrice;	//是否和前面每次TICK都=0重复了	，是否可以删除前面	=0赋值
																


																//TOTAL部分只累计，不清空置0


				q->M1_BS_Time[0] = q->tick_BS_Time[0];
				//每分钟之更新1次

				for (int j = 0; j < MAX_PURE_TICK_PERMIN_NUM; j++)
				{


					//printf("A3\n");
					//double temptime=tick_BS_Time[j];
					//double temptime2=tick_BS_Time[j+1];
					//四舍五入
					//cout.setf(ios::fixed);
					// cout.setf(ios::showpoint);
					//cout.precision(4);

					//cout<<temptime;
					//cout<<temptime2;
					//四舍五入
					//if(tick_BS_Time[j]>0)
					////printf("时间啊B%d[%.06f][%.06f]\n",j,tick_BS_Time[j], ((double)(tick_BS_Time[j])).precision(2));
					//printf("时间啊XXXX______________   %d[%.06f]_[%.06f]\n",j,tick_BS_Time[j],temptime);

					//if(temptime-temptime2>=0.0001 && (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //一定有时间数，但是要排除00:00点
					// if(  (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //一定有时间数，但是要排除00:00点
					// {

					q->M1_BS_PURE_Value[0] = q->M1_BS_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_BUY_Value[0] = q->M1_BS_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//逻辑是否有问题
					q->M1_BS_SELL_Value[0] = q->M1_BS_SELL_Value[0] + q->tick_BS_SELL_Value[j];

					//bl,middle在哪里？
					q->M1_BS_TOTAL_PURE_Value[0] = q->M1_BS_TOTAL_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_TOTAL_BUY_Value[0] = q->M1_BS_TOTAL_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//逻辑是否有问题
					q->M1_BS_TOTAL_SELL_Value[0] = q->M1_BS_TOTAL_SELL_Value[0] + q->tick_BS_SELL_Value[j];

					q->M1_BS_CROSS_TOTAL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_CROSS_TOTAL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//逻辑是否有问题
					q->M1_BS_CROSS_TOTAL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SELL_Value[0] + q->tick_BS_SELL_Value[j];


					//TOTAL 还是CROSS?
					//小单





					double tb = q->tick_BS_BUY_Value[j];
					double ts = q->tick_BS_SELL_Value[j];


					if (tb < CAL_MIDDLE_AMOUNT)
					{
						q->M1_BS_SMALL_BUY_Value[0] = q->M1_BS_SMALL_BUY_Value[0] + tb;
						q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_TOTAL_SMALL_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] + tb;  //检查


						q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] + tb;  //检查
					}

					//中单
					else if (tb >= CAL_MIDDLE_AMOUNT && tb < CAL_BIG_AMOUNT)
					{
						// printf("[%d]中单 %0.02f\n", j, tb);
						q->M1_BS_MIDDLE_BUY_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //检查

						q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //检查
					}

					else// if (tb >= CAL_BIG_AMOUNT)
					{
						//printf("[%d]大单 %0.02f\n", j, tb);
						q->M1_BS_BIG_BUY_Value[0] = q->M1_BS_BIG_BUY_Value[0] + tb;
						q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_TOTAL_BIG_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] + tb;  //检查

						q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] + tb;  //检查
					}



					if (ts < CAL_MIDDLE_AMOUNT)
					{
						q->M1_BS_SMALL_SELL_Value[0] = q->M1_BS_SMALL_SELL_Value[0] + ts;
						q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_TOTAL_SMALL_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] - ts;  //检查

						q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] - ts;  //检查
					}
					else if (ts >= CAL_MIDDLE_AMOUNT && ts < CAL_BIG_AMOUNT)
					{
						q->M1_BS_MIDDLE_SELL_Value[0] = q->M1_BS_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //检查

						q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //检查
					}
					else// if (ts >= CAL_BIG_AMOUNT)
					{
						q->M1_BS_BIG_SELL_Value[0] = q->M1_BS_BIG_SELL_Value[0] + ts;
						q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_TOTAL_BIG_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] - ts;  //检查

						q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] - ts;  //检查
					}


					if (q->M1_BS_SELL_Value[0] > 0)
					{
						q->M1_BS_BL_Value[0] = q->M1_BS_BUY_Value[0] / q->M1_BS_SELL_Value[0];
					}
					else
					{
						q->M1_BS_BL_Value[0] = 0;
					}

					if (q->M1_BS_SMALL_SELL_Value[0] > 0)
					{
						q->M1_BS_SMALL_BL_Value[0] = q->M1_BS_SMALL_BUY_Value[0] / q->M1_BS_SMALL_SELL_Value[0];
					}
					else
					{
						q->M1_BS_SMALL_BL_Value[0] = 0;
					}


					if (q->M1_BS_MIDDLE_SELL_Value[0] > 0)
					{
						q->M1_BS_MIDDLE_BL_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] / q->M1_BS_MIDDLE_SELL_Value[0];
					}
					else
					{
						q->M1_BS_MIDDLE_BL_Value[0] = 0;
					}


					if (q->M1_BS_BIG_SELL_Value[0] > 0)
					{
						q->M1_BS_BIG_BL_Value[0] = q->M1_BS_BIG_BUY_Value[0] / q->M1_BS_BIG_SELL_Value[0];
					}
					else
					{
						q->M1_BS_BIG_BL_Value[0] = 0;
					}


					//for (int k = 0; k<5; k++)
					//{
					//int id = i;
					//if(q->M1_BS_BUY_Value[id][k]>1e-10)
					// {
					//printf("当前时间：[%.06f]\n", tick_data[2]);
					//printf("\n__%d时间：[%.06f][%d][%s]tb[%0.02f]ts[%0.02f]净买[%0.02f]买[%0.02f]卖[%0.02f]比例[%0.02f]小净[%0.02f]小买[%0.02f]小卖[%0.02f]小单比例[%0.02f]中净[%0.02f]中买[%0.02f]中卖[%0.02f]中单比例[%0.02f]大净[%0.02f]大买[%0.02f]大卖[%0.02f]大单比例[%0.02f]\n",


					//}
					//printf("------------------\n");




				}






				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3更新


				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3更新

				//printf("A6\n");



				//打印K线线数据
				//	 cerr << "--->>> K:" << InstrumentID_n << "_" <<dbtoch(tick_data[1])<< "_" << tick_data[2]<< "_" << q->M1_open[1]<< "_" << q->M1_high[1]<< "_" << q->M1_low[1]<< "_" << q->M1_close[1] << endl;
			}
			else
			{
				//printf("A7\n");
				// printf("\n\n\n_______________________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx__________________________kaishi \n\n\n");
				//更新最后一根K线？自己添加理解
				if (q->M1_open[0] == 0) { q->M1_open[0] = pDepthMarketData->LastPrice; }

				q->M1_high[0] = max(q->M1_high[0], pDepthMarketData->LastPrice);
				q->M1_low[0] = zero_min(q->M1_low[0], pDepthMarketData->LastPrice);
				q->M1_close[0] = pDepthMarketData->LastPrice;


				
				//q->M1_BS_PURE_Value[1]=q->M1_BS_PURE_Value[0];
				//q->M1_BS_PURE_Value[0]=NewVolume;

				//q->M1_BS_PURE_Value[0]=tick_BS_PURE_Value[0]+tick_BS_PURE_Value[1]+tick_BS_PURE_Value[2];
				//q->M1_BS_BUY_Value[0]=tick_BS_BUY_Value[0]+tick_BS_BUY_Value[1]+tick_BS_BUY_Value[2];
				//q->M1_BS_SELL_Value[0]=tick_BS_SELL_Value[0]+tick_BS_SELL_Value[1]+tick_BS_SELL_Value[2];

				//TICK过滤后，每秒1次TICK，1分钟应该改为60比TICK。每秒2次TICK，应该位120比
				//q->M1_BS_PURE_Value[0]=0;
				//q->M1_BS_BUY_Value[0]=0;
				//q->M1_BS_SELL_Value[0]=0;
				//printf("BBBBB \n");
				// printf("下一分钟\n");
				//for(int j=0;j<MAX_PURE_TICK_PERMIN_NUM;j++)
				//{



				// cout<<temptime;
				//cout<<temptime2;
				//四舍五入
				//if(tick_BS_Time[j]>0)
				////printf("时间啊B%d[%.06f][%.06f]\n",j,tick_BS_Time[j], ((double)(tick_BS_Time[j])).precision(2));
				//printf("时间啊B   %d[%.06f]_[%.06f]\n",j,tick_BS_Time[j],temptime);
				//printf("CCCCC \n");

				//  if(temptime-temptime2>=0.0001 && (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //一定有时间数，但是要排除00:00点
				// if((tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //一定有时间数，但是要排除00:00点

				//if(tick_BS_Time[j]>0)
				// printf("时间差[%.06f][%.06f]_[%.06f]\n",tick_BS_Time[j],tick_BS_Time[j+1],temptime-temptime2);
				//if(tick_BS_Time[j]>0)
				//printf("时间差TICK[%.06f]M1[%.06f]_M2[%.06f]\n",tick_BS_Time[j],tempq->M1_A,tempq->M1_B);

				//if(tick_BS_Time[j]-tempq->M1_A>=0.000001 && (tempq->M1_A>0 || tempq->M1_B>0))
				//if()
				//{
				//if(j==0)
				double tb = q->tick_BS_BUY_Value[0];
				double ts = q->tick_BS_SELL_Value[0];



				q->M1_BS_PURE_Value[0] = q->M1_BS_PURE_Value[0] + q->tick_BS_PURE_Value[0];
				q->M1_BS_BUY_Value[0] = q->M1_BS_BUY_Value[0] + tb;
				q->M1_BS_SELL_Value[0] = q->M1_BS_SELL_Value[0] + ts;


				if (tb < CAL_MIDDLE_AMOUNT)
				{
					q->M1_BS_SMALL_BUY_Value[0] = q->M1_BS_SMALL_BUY_Value[0] + tb;
					q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_TOTAL_SMALL_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] + tb;  //检查

					q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] + tb;  //检查
				}

				//中单
				else if (tb >= CAL_MIDDLE_AMOUNT && tb < CAL_BIG_AMOUNT)
				{
					// printf("[%d]中单 %0.02f\n", j, tb);
					q->M1_BS_MIDDLE_BUY_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //检查

					q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //检查
				}

				else// if (tb >= CAL_BIG_AMOUNT)
				{
					//printf("[%d]大单 %0.02f\n", j, tb);
					q->M1_BS_BIG_BUY_Value[0] = q->M1_BS_BIG_BUY_Value[0] + tb;
					q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_TOTAL_BIG_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] + tb;  //检查

					q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] + tb;  //检查
				}

				if (ts < CAL_MIDDLE_AMOUNT)
				{
					q->M1_BS_SMALL_SELL_Value[0] = q->M1_BS_SMALL_SELL_Value[0] + ts;
					q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_TOTAL_SMALL_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] - ts;  //检查

					q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] + ts;
					q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] - ts;  //检查
				}
				else if (ts >= CAL_MIDDLE_AMOUNT && ts < CAL_BIG_AMOUNT)
				{
					q->M1_BS_MIDDLE_SELL_Value[0] = q->M1_BS_MIDDLE_SELL_Value[0] + ts;
					q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //检查
				}
				else// if (ts >= CAL_BIG_AMOUNT)
				{
					q->M1_BS_BIG_SELL_Value[0] = q->M1_BS_BIG_SELL_Value[0] + ts;
					q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_TOTAL_BIG_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] - ts;  //检查
				}

				///////////////////////////////////////////////////////////////////////////////////////////////////////临时
				//无必要，可删除

				//无必要


				////////////////////////////////////////////////////////////////////////////////////////////////////////////临时







				//printf("1分钟净买B[%d]主买[%d]主卖[%d]主买比主卖[%.02f]\n",(int)M1_BS_PURE_Value[0],(int)M1_BS_BUY_Value[0],(int)M1_BS_SELL_Value[0],MMBL);
				//M1_BS_PURE_Value[0]=Plus(M1_BS_PURE_Value[0],tick_BS_PURE_Value[0]);
				//M1_BS_BL_Value[0]=Ave(M1_BS_BL_Value[0],M1_BS_BL_Value[1],M1_BS_BL_Value[2],M1_BS_BL_Value[3],M1_BS_BL_Value[4],M1_BS_BL_Value[5],M1_BS_BL_Value[6],M1_BS_BL_Value[7],M1_BS_BL_Value[8],M1_BS_BL_Value[9]);
				




				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3更新

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3更新
				if (q->TickFileWritepaths[0] == '\0')
				{
					q->tick_data[8] = pDepthMarketData->UpperLimitPrice;
					q->tick_data[9] = pDepthMarketData->LowerLimitPrice;

					strcpy(q->TickFileWritepaths, "./TickData/");
					strcat(q->TickFileWritepaths, pDepthMarketData->InstrumentID);
					strcat(q->TickFileWritepaths, "_");
					//恢复strcat_s(q->TickFileWritepaths, TradingDay);
					strcat(q->TickFileWritepaths, ".txt");

					//检查文件是否存在，是否需要新建文本文件
					ifstream inf;
					ofstream ouf;
					inf.open(q->TickFileWritepaths, ios::out);


					/////////////////////////////////////////////////
					//char str[200] = { 0 };
					//strcat_s(str, 200, "SingleData/TradeRecord.txt");

					//检查文件是否存在，是否需要新建文本文件
					//	ifstream inf;
					//ofstream ouf;
					//inf.open(TickFileWritepaths[i], ios::out);
					//inf.open(str, ios::out);
					//}

					//记录TICK数据
					//ofstream o_file(str, ios::app);
					//if ( RunMode && ( check0 || check1 || check2) )

					//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

					//o_file << content << "\t" << endl;

					//else
					//{
					//o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //将内容写入到文本文件中
					//}
					//o_file.close();						//关闭文件




				}

				//记录TICK数据
				ofstream o_file(q->TickFileWritepaths, ios::app);

				if (check1 || check2 || check3 || check4 || check5 || check6 || check7)
				{
					o_file << dbtoch(q->tick_data[1]) << "\t" << q->tick_data[2] << "\t" << pDepthMarketData->UpdateMillisec << "\t" << q->tick_AskPrice1[0] << "\t" << q->tick_AskVolume1[0] << "\t" << q->tick_BidPrice1[0] << "\t" << q->tick_BidVolume1[0] << "\t" << q->tick_data[4] << "\t" << dbtoch(q->tick_Volume[0]) << "\t" << dbtoch(q->tick_OpenInterest[0]) << "\t" << q->Day_open[2] << "\t" << q->Day_high[2] << "\t" << q->Day_low[2] << "\t" << q->Day_close[2] << "\t" << q->Day_open[1] << "\t" << q->Day_high[1] << "\t" << q->Day_low[1] << "\t" << q->Day_close[1] << "\t" << q->tick_data[8] << "\t" << q->tick_data[9] << endl;
				}
				else
				{
					o_file << dbtoch(q->tick_data[1]) << "\t" << q->tick_data[2] << "\t" << pDepthMarketData->UpdateMillisec << "\t" << q->tick_AskPrice1[0] << "\t" << q->tick_AskVolume1[0] << "\t" << q->tick_BidPrice1[0] << "\t" << q->tick_BidVolume1[0] << "\t" << q->tick_data[4] << "\t" << dbtoch(q->tick_Volume[0]) << "\t" << dbtoch(q->tick_OpenInterest[0]) << endl; //将内容写入到文本文件中
				}
				o_file.close();						//关闭文件


				if (q->tick_data[2]>0.145950 && (q->tick_Volume[0] - q->tick_Volume[1])<0.01 && (q->tick_OpenInterest[0] - q->tick_OpenInterest[1])<0.01)// && LastTick[i]==false)
				{
					//LastTick[i]=true;

					q->Day_open[0] = pDepthMarketData->OpenPrice;
					q->Day_high[0] = pDepthMarketData->HighestPrice;
					q->Day_low[0] = pDepthMarketData->LowestPrice;
					q->Day_close[0] = pDepthMarketData->LastPrice;
					//cerr << "--->>> " <<"WriteConfiguration!" << endl;
					//WriteConfiguration("./AutoTrader.dat");
				}

				bool FistCheck1 = (q->TYPEID == TYPE_PP) || (q->TYPEID == TYPE_CS) || (q->TYPEID == TYPE_JD) || (q->TYPEID == TYPE_L) || (q->TYPEID == TYPE_V) || (q->TYPEID == TYPE_C) || (q->TYPEID == TYPE_WH) || (q->TYPEID == TYPE_SM) || (q->TYPEID == TYPE_SF);
				//无夜盘
				bool FistCheck2 = (!FistCheck1) && (!typecheck1) && (!typecheck2);
				//有夜盘
				//老if ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i<=17))
				//    if ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i <= 17) || (tick_data[i][2]<0.0230 && i <= 17))  //修复9

				if (seconds>45 && seconds<55 && q->M1Klinesig == true)
				{
					//printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx转换xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n\n");
					q->M1Klinesig = false;
				}



				//检查修正                                                                                //无夜盘                                               //有夜盘
				if ((q->tick_data[2]>0.0858 && typecheck1) || (q->tick_data[2]>0.0925 && typecheck2) || (q->tick_data[2]>0.0858 && FistCheck1) || ((q->tick_data[2]>0.2058 || q->tick_data[2]<0.1530) && FistCheck2))  //修复9
				{
					q->FristTick = true;
				}

				q->ReceiveTick = false;
				q->tick_data[0] = 1;		//设置标志位

			}


		}


	}




}





CMdSpi::CMdSpi()
{
	  begintime1=-1;
	  begintime2=-1;
	  begintime3=-1;
	  begintime4=-1;

	  endtime1=-1;
	  endtime2=-1;
	  endtime3=-1;
	  endtime4=-1;



	mInitOK = FALSE;
	//mpUserApi = NULL;
	mhSyncObj = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	RequestID = 0;
}

CMdSpi::~CMdSpi()
{
	::CloseHandle(mhSyncObj);
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;
	IsErrorRspInfo(pRspInfo);
}



void CMdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;
 
    
	//if ((*OnFrontDisconnectedCallback ))
	    	OnFrontDisconnectedCallback(&nReason);
 
	::Beep(800, 10000);
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	//cerr << "--->>> " << __FUNCTION__ << std::endl;
}

BOOL CMdSpi::Init()
{
	return TRUE;
}


void CMdSpi::OnFrontConnected()
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;


	if (*OnFrontConnectedCallback)
		   OnFrontConnectedCallback();
    ///用户登录请求
	ReqUserLogin();	  
 
}


char *CMdSpi::GetApiVersion()
{
	_snprintf_s(ver,sizeof(ver),sizeof(ver),"%s", (char*)(mpUserApi->GetApiVersion()));
	return ver;
}

char *CMdSpi::GetTradingDay()
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;
	_snprintf_s(tradingday, sizeof(tradingday), sizeof(tradingday), "%s", (char*)(mpUserApi->GetTradingDay()));
	return tradingday;
}

void   CMdSpi::RegisterFront(char *pszFrontAddress)
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;
	mpUserApi->RegisterFront(pszFrontAddress);

}

void   CMdSpi::RegisterNameServer(char *pszNsAddress)
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;
	mpUserApi->RegisterNameServer(pszNsAddress);
}


int CMdSpi::ReqUserLogin()
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID.c_str());
	strcpy(req.UserID, gInvestorID.c_str());
	strcpy(req.Password, gPassword.c_str());
	printf("cpp2: %s,%s,%s \n",req.BrokerID, req.UserID, req.Password);

	return mpUserApi->ReqUserLogin(&req, ++RequestID);
}

int CMdSpi::ReqUserLogout()
{
	cerr << "--->>> " << __FUNCTION__ << std::endl;
 	CThostFtdcUserLogoutField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID.c_str());
	strcpy(req.UserID, gInvestorID.c_str());
	//strcpy(req.Password, gPassword.c_str());
	return mpUserApi->ReqUserLogout(&req, ++RequestID);
}



void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!pRspInfo)
	{
		return;
	}
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
		cerr << "TradeingDay: " << mpUserApi->GetTradingDay() << endl;
		mInitOK = TRUE;

 
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
			printf("登录失败,ErrorID=0x%04x, ErrMsg=%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);

			CThostFtdcRspUserLoginField tn;
			memset(&tn, 0, sizeof(CThostFtdcRspUserLoginField));
			memcpy_s(&tn, sizeof(CThostFtdcRspUserLoginField), pRspUserLogin, sizeof(CThostFtdcRspUserLoginField));

	}
	else
	{
		if (*OnRspUserLoginCallback)
			OnRspUserLoginCallback();


			printf("登录成功\n");
			Sleep(3000);
	}

	}

}

int CMdSpi::SubscribeForQuoteRsp(char * InstrumentID)
{
	return mpUserApi->SubscribeForQuoteRsp(&InstrumentID,1);
}

int CMdSpi::SubscribeMarketData(char * InstrumentID)
{
	return mpUserApi->SubscribeMarketData(&(ppInstrumentID[amount - 1]), 1);
}

int CMdSpi::SubscribeMarketData()
{
	return mpUserApi->SubscribeMarketData( &(ppInstrumentID[amount - 1]), 1);
}


int CMdSpi::UnSubscribeMarketData()
{
	if (amount >= 1)
	{
		int iResult = mpUserApi->UnSubscribeMarketData(&(ppInstrumentID[amount - 1]), 1);
		if (iResult != 0)
			cerr << "Failer:" << __FUNCTION__ << ppInstrumentID[amount - 1] << endl;
		else
			cerr << "Scuess:" << __FUNCTION__ << ppInstrumentID[amount - 1] << endl;
	}
	return 0;
}





/*
void CMdSpi::WirteTick_Simple(const char * InstrumentID,const char * ticktime, double price)
{
	char str[200] = { 0 };
	strcat_s(str, 200, "TickData_Simple\\TickData_");
	strcat_s(str, 200, InstrumentID);
	strcat_s(str, 200, ".csv");

	//检查文件是否存在，是否需要新建文本文件
	ifstream inf;
	ofstream ouf;
	//inf.open(TickFileWritepaths[i], ios::out);
	inf.open(str, ios::out);
 

	//记录TICK数据
	ofstream o_file(str, ios::app);
 

	o_file << ticktime<<","<<price << "," << endl;

 
	o_file.close();						//关闭文件

}


void CMdSpi::WirteTick_Detail(const char * InstrumentID,const CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	char str[200] = { 0 };
	strcat_s(str, 200, "TickData_Detail\\TickData_");
	strcat_s(str, 200, InstrumentID);
	strcat_s(str, 200, ".csv");

	//检查文件是否存在，是否需要新建文本文件
	ifstream inf;
	ofstream ouf;
	inf.open(str, ios::out);
	//}
	if (!inf)
	{
		cout << "文件不能打开" << endl;
		ofstream o_file(str, ios::app);

		o_file << "交易日" << ","
			<< "合约" << ","
			<< "时间" << ","
			<< "毫秒" << ","
			<< "现价" << ","
			<< "持仓" << ","  //持仓量
			<< "成交量" << ","
			<< "买一" << ","
			//<< "买二"<< ","
			//<< "买三" << ","
			//<< "买四" << ","
			//<< "买五" << ","
			<< "买一量" << ","
			//<< "买二量" << ","
			//<< "买三量" << ","
			//<< "买四量" << ","
			//<< "买五量" << ","
			<< "卖一" << ","
			//<< "卖二" << ","
			//<< "卖三" << ","
			//<< "卖四" << ","
			//<< "卖五" << ","
			<< "卖一量" << ","
		    //<< "卖二量" << ","
			//<< "卖三量" << ","
			//<< "卖四量" << ","
			//<< "卖五量" << ","
			<< "涨停价" << ","
			<< "跌停价" << endl;
	}
	else
	{
		ofstream o_file(str, ios::app);
	
	
	}
	//记录TICK数据
	ofstream o_file(str, ios::app);
	//if ( RunMode && ( check0 || check1 || check2) )

	//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

 //<<
		//ticktime << "," 
		//<< price 
		
		
	o_file<< pDepthMarketData->TradingDay << ","
		<< pDepthMarketData->InstrumentID << ","
		<< pDepthMarketData->UpdateTime << ","
		<< pDepthMarketData->UpdateMillisec << ","
		<< pDepthMarketData->LastPrice<< ","
		<< pDepthMarketData->OpenInterest << ","  //持仓量
		<< pDepthMarketData->Volume << ","
		<< pDepthMarketData->AskPrice1 << ","
		//<< pDepthMarketData->AskPrice2 << ","
		//<< pDepthMarketData->AskPrice3 << ","
		//<< pDepthMarketData->AskPrice4 << ","
		//<< pDepthMarketData->AskPrice5 << ","
		<< pDepthMarketData->AskVolume1 << ","
		//<< pDepthMarketData->AskVolume2 << ","
		//<< pDepthMarketData->AskVolume3 << ","
		//<< pDepthMarketData->AskVolume4 << ","
		//<< pDepthMarketData->AskVolume5 << ","
		<< pDepthMarketData->BidPrice1 << ","
		//<< pDepthMarketData->BidPrice2 << ","
		//<< pDepthMarketData->BidPrice3 << ","
		//<< pDepthMarketData->BidPrice4 << ","
		//<< pDepthMarketData->BidPrice5 << ","
		<< pDepthMarketData->BidVolume1 << ","
		//<< pDepthMarketData->BidVolume2 << ","
		//<< pDepthMarketData->BidVolume3 << ","
		//<< pDepthMarketData->BidVolume4 << ","
		//<< pDepthMarketData->BidVolume5 << ","
		<< pDepthMarketData->UpperLimitPrice << ","
		<< pDepthMarketData->LowerLimitPrice << endl;

 
 
	o_file.close();						//关闭文件

}
*/

//PYTHON
/*
#include "include_Python/Python.h"
void strategy2(const char * filename)
{

	Py_Initialize();	          //使用python之前，要调用Py_Initialize();这个函数进行初始化
	PyObject * pModule = NULL;	  //声明变量
	PyObject * pFunc = NULL;	  //声明变量


	pModule = PyImport_ImportModule(filename);	            //这里是要调用的Python文件名
	//char tempstr[300] = { 0 };
	//_snprintf_s(tempstr,sizeof(tempstr),sizeof(tempstr),"run %s", filename);
	pFunc = PyObject_GetAttrString(pModule, "main_strategy");	//这里是要调用的函数名
	PyEval_CallObject(pFunc, NULL);	                        //调用函数,NULL表示参数为空
															//Py_Finalize();	              //调用Py_Finalize,这个和Py_Initialize相对应的.
															//调用会错误

}


void strategy3(const char * filename,const char * instrument, double price)
{
	Py_Initialize();

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	pModule = PyImport_ImportModule(filename);	    //Test001:Python文件名
	pFunc = PyObject_GetAttrString(pModule, "main_strategy");	//Add:Python文件中的函数名
													//创建参数:
	PyObject *pArgs = PyTuple_New(2);               //函数调用的参数传递均是以元组的形式打包的,2表示参数个数
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 5));//0---序号  i表示创建int型变量
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 5));//1---序号
	//PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", price));//1---序号
													 //返回值
	PyObject *pReturn = NULL;
	pReturn = PyEval_CallObject(pFunc, pArgs);	    //调用函数
													//将返回值转换为int类型
	int result;
	PyArg_Parse(pReturn, "i", &result);    //i表示转换成int型变量
	//cout << "5+7 = " << result << endl;
	if (result==1)
	{
		printf("%s触发了[%s]交易信号", instrument, filename);
	
	}
	//Py_Finalize();
}
*/


	/*

void CMdSpi::strategy()
{

	
	
	
	//strategy1();




	Py_Initialize();	          //使用python之前，要调用Py_Initialize();这个函数进行初始化
	PyObject * pModule = NULL;	  //声明变量
	PyObject * pFunc = NULL;	  //声明变量


	pModule = PyImport_ImportModule("Test001");	            //这里是要调用的Python文件名
	pFunc = PyObject_GetAttrString(pModule, "HelloWorld");	//这里是要调用的函数名
	PyEval_CallObject(pFunc, NULL);	                        //调用函数,NULL表示参数为空
															//Py_Finalize();	              //调用Py_Finalize,这个和Py_Initialize相对应的.
															//调用会错误
	//TestTransferDict();

	
}*/
//参数传递的类型为字典
/*
void TestTransferDict()
{
	Py_Initialize();

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	pModule = PyImport_ImportModule("Test001");	    //Test001:Python文件名
	pFunc = PyObject_GetAttrString(pModule, "TestDict");	//Add:Python文件中的函数名
															//创建参数:
	PyObject *pArgs = PyTuple_New(1);
	PyObject *pDict = PyDict_New();   //创建字典类型变量
	PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "WangYao")); //往字典类型变量中填充数据
	PyDict_SetItemString(pDict, "Age", Py_BuildValue("i", 25));         //往字典类型变量中填充数据
	PyTuple_SetItem(pArgs, 0, pDict);//0---序号  将字典类型变量添加到参数元组中
									 //返回值
	PyObject *pReturn = NULL;
	pReturn = PyEval_CallObject(pFunc, pArgs);	    //调用函数
													//处理返回值:
	int size = PyDict_Size(pReturn);
	cout << "返回字典的大小为: " << size << endl;
	PyObject *pNewAge = PyDict_GetItemString(pReturn, "Age");
	int newAge;
	PyArg_Parse(pNewAge, "i", &newAge);
	cout << "True Age: " << newAge << endl;

	Py_Finalize();
}
*/
bool firststate = true;


//加锁
HANDLE hThread[12];
unsigned int uiThreadId[12];
//加锁
/*
DWORD WINAPI strategy(const LPVOID lpParam)
//UINT strategy(LPVOID pParam)
// void CMdSpi::strategy()
{
	Py_Initialize();

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	pModule = PyImport_ImportModule("Test001");	    //Test001:Python文件名
	pFunc = PyObject_GetAttrString(pModule, "TestDict");	//Add:Python文件中的函数名
															//创建参数:
	PyObject *pArgs = PyTuple_New(1);
	PyObject *pDict = PyDict_New();   //创建字典类型变量
	PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "WangYao")); //往字典类型变量中填充数据
	PyDict_SetItemString(pDict, "Age", Py_BuildValue("i", 25));         //往字典类型变量中填充数据
	PyTuple_SetItem(pArgs, 0, pDict);//0---序号  将字典类型变量添加到参数元组中
									 //返回值
	PyObject *pReturn = NULL;
	pReturn = PyEval_CallObject(pFunc, pArgs);	    //调用函数
													//处理返回值:
	Py_ssize_t size = PyDict_Size(pReturn);
	cout << "返回字典的大小为: " << size << endl;
	PyObject *pNewAge = PyDict_GetItemString(pReturn, "Age");
	int newAge;
	PyArg_Parse(pNewAge, "i", &newAge);
	cout << "True Age: " << newAge << endl;

	Py_Finalize();
	return 1;
};



unsigned int __stdcall SniffAndTradingThread(void *lpParam)
{
	if (lpParam)
	{
		idrand* ir = (idrand*)lpParam;//获取参数结构体
		while (true)
		{
			showcalnum[ir->threadid]++;
			if (showcalnum[ir->threadid] > 200)
			{
				showcalnum[ir->threadid] = 0;
				printf("[%d]  %d~%d\n", ir->threadid, ir->beginid, ir->endid);
			}


			// WaitForSingleObject(semaphorehandle[ir->threadid], INFINITE);
			Sniffer(ir->beginid, ir->endid);
			Trading(ir->beginid, ir->endid);	//冲突

			::Sleep(100);
			//  ReleaseSemaphore(semaphorehandle[ir->threadid], 1, NULL);
		}
		delete lpParam;
		lpParam = NULL;
	}
	return 0;
}
*/
double GetLocalTimeSec2()
{
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	double system_times;
	system_times = (double)((sys_time.wHour) / 10e1) + (double)((sys_time.wMinute) / 10e3) + (double)((sys_time.wSecond) / 10e5);	//格式时间0.145100
	return system_times;
}

 
void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{	
	if (!pDepthMarketData)
	{
		return;
	}
 
	if (*OnRtnDepthMarketDataCallback)
		 OnRtnDepthMarketDataCallback(pDepthMarketData);
	return;
	//本机时间过滤
	double Nowtime = GetLocalTimeSec2();
	if (begintime1 > -0.1 && endtime1 > -0.1)
	{
		if (Nowtime<begintime1 && Nowtime>endtime1)
	  {
		  
		printf("不在交易时间内，行情数据被拒绝[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
		return;
	  }
	
	}
	if (begintime2 > -0.1 && endtime2 > -0.1)
	{
		if (Nowtime > begintime2 && Nowtime < endtime2)
		{
			printf("不在交易时间内，行情数据被拒绝[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			return;
		}
	}
	if (begintime3 > -0.1 && endtime3 > -0.1)
	{
		if (Nowtime > begintime3 && Nowtime < endtime3)
		{
			printf("不在交易时间内，行情数据被拒绝[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			return;
		}
	}
	if (begintime4 > -0.1 && endtime4 > -0.1)
	{
		if (Nowtime > begintime4 && Nowtime < endtime4)
		{
			printf("不在交易时间内，行情数据被拒绝[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			return;
		}
	}
	//本机时间过滤
	if (gMarket.find(pDepthMarketData->InstrumentID) != gMarket.end())
	{
		memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		UpdatePriceHash((char*)(&pDepthMarketData->InstrumentID), pDepthMarketData);

		/*
		printf("savetickstate:[%d]\n", savetickstate);
		if (savetickstate==2)
		{   
			printf("保存数据(详细)中...\r");
			WirteTick_Detail((char*)pDepthMarketData->InstrumentID, pDepthMarketData);
		}
		else if (savetickstate == 1)
		{
			printf("保存数据(少量)中...\r");
			WirteTick_Simple((char*)pDepthMarketData->InstrumentID, (char*)pDepthMarketData->UpdateTime, (double)pDepthMarketData->LastPrice);
		}
		*/

		//TThostFtdcInstrumentIDTypeStruct tn;
		//memset(&tn, 0, sizeof(TThostFtdcInstrumentIDTypeStruct));

		//strncpy_s(tn.Instrument, sizeof(tn.Instrument), pDepthMarketData->InstrumentID, sizeof(pDepthMarketData->InstrumentID));

		//EnterCriticalSection(&g_csdata);
		//ticknamelist.push_back(tn);
		//LeaveCriticalSection(&g_csdata);
		//时间驱动
		//SetEvent(hEvent[EID_OnRtnDepthMarketData]);
		//return;
		//下面的暂时屏蔽，对多策略
		/*
       //生成周期价格数据
		string instrumentstr = pDepthMarketData->InstrumentID;
		QS_Strategy_Map::iterator it = mapStrategy.find(instrumentstr);
		if (it == mapStrategy.end())
		{
				printf("没找到该合约的策略%s\n", pDepthMarketData->InstrumentID); 
		}
		else
		{
			for (int k = 0; k < it->second.strategyfilenum; k++)
			{
				if (true) 
				{ 
					printf("调用[%s]的策略文件：[%s] 仓位[%d]\n", pDepthMarketData->InstrumentID, (it->second.strategyfile[k]).c_str(), it->second.position[k]);
					//strategy((it->second.strategyfile[k]).c_str(), pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);
					
					if (firststate)
					{
						firststate = false;
						//strategy();
					//	hThread[id] = (HANDLE)
					//	_beginthreadex(NULL, 0, &strategy, (void *)pDepthMarketData, 0, &uiThreadId[0]);

						//HANDLE hThread2 = 
							::CreateThread(NULL, 0, strategy, (LPVOID)(it->second.strategyfile[k]).c_str(), 0, NULL);
						//m_pCStrategyThread = (CStrategy*)
						//AfxBeginThread(RUNTIME_CLASS(CStrategy));
						//int m_ctrlD = 1;
					//	CWinThread * m_pCStrategy = AfxBeginThread(strategy,&m_ctrlD,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
					//	m_pCStrategyThread->ResumeThread();
						//m_pCStrategyThread->PostThreadMessage(WM_STRATEGY, 5, NULL);

						//TestTransferDict();
					}
				
				}
			}
		}
		*/
	}
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
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


///订阅行情应答
void  CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (*OnRspSubMarketDataCallback)
		OnRspSubMarketDataCallback(0);

};

///取消订阅行情应答
void  CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{ 
	if (*OnRspUnSubMarketDataCallback)
		OnRspUnSubMarketDataCallback();
};

///询价通知
void  CMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{

};

///登出请求响应
void CMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (*OnRspUserLogoutCallback)
		OnRspUserLogoutCallback();
}