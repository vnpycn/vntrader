/*
1.���ļ�ΪVNTrader �ڻ�CTP���׿�ײ����
2.VNTrader����C++�⿪ԴЭ��MITЭ��

��VNPY��Դ���������׵ģ����õ�ԭʼ���߿϶��ģ���������http://www.vnpy.cn��վ�ϣ�
������ڡ���Դ˵���͸�л.txt�����������ļ����ϸ��·���ÿһ���°汾��vnpy���

�ٷ���վ��http://www.vnpy.cn
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

 int max_pure_tick_num = 130;  //����120TICK�������� ������
 int max_pure_tick_move_num = 129; //����120TICK�������� ������

 int max_pure_m1_num = 480;  //ÿ1���� 120TICK
 int max_pure_m1_move_num = 479;  //ÿ1���� 120TICK

 int max_pure_m3_num = 361;  //160  //ÿ3���� 360TICK
 int max_pure_m3_move_num = 360;  //159  //ÿ3���� 360TICK

 int max_pure_m5_num = 96;  //ÿ3���� 360TICK
 int max_pure_m5_move_num = 95; //ÿ3���� 360TICK

 int max_pure_m10_num = 96;  //ÿ3���� 360TICK
 int max_pure_m10_move_num = 95; //ÿ3���� 360TICK

 int  max_pure_m15_num = 96;  //ÿ3���� 360TICK
 int  max_pure_m15_move_num = 95; //ÿ3���� 360TICK

 int  max_pure_m30_num = 96;  //ÿ3���� 360TICK
 int  max_pure_m30_move_num = 95; //ÿ3���� 360TICK

 int  max_pure_m60_num = 96; //ÿ3���� 360TICK
 int  max_pure_m60_move_num = 95; //ÿ3���� 360TICK

 int  max_pure_m120_num = 96;  //ÿ3���� 360TICK
 int  max_pure_m120_move_num = 95; //ÿ3���� 360TICK

 int  max_pure_m240_num = 96;  //ÿ3���� 360TICK
 int  max_pure_m240_move_num = 95;//ÿ3���� 360TICK

 int  max_pure_d1_num = 30;  //ÿ3���� 360TICK
 int  max_pure_d1_move_num = 29; //ÿ3���� 360TICK

 int  max_pure_m480_num = 100;  //��ÿ��8Сʱ����Ϊ480����
 int  max_pure_m480_move_num = 99;  //��ÿ��8Сʱ����Ϊ480����
// ���ò���
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
//������

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
double CAL_BIG_AMOUNT = 50;// = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 ,100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };        //��Ʒ�ִ󵥵Ķ���
bool CAL_AMOUNT_STATE = true;// = { true, true, true, true, true, true, true, true, true, true ,true, true, true, true, true, true, true, true, true, true };        //true��Ҫ����BIG_AMOUNT
int  GetMin(double thistime)
{
	int h = (int)(thistime * 100); //  21
	int m = (int)(thistime * 10000) - 100 * h; //  2130-2100=30
	return  (h * 60 + m);
}
string dbtoch(double nums)		//��double ת��Ϊ string ���б���
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

	//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
	ifstream inf;
	ofstream ouf;
	//inf.open(TickFileWritepaths[i], ios::out);
	inf.open(str, ios::out);
	//}

	//��¼TICK����
	ofstream o_file(str, ios::app);
	//if ( RunMode && ( check0 || check1 || check2) )

	//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

	o_file << content << "\t" << endl;

	//else
	//{
	//o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //������д�뵽�ı��ļ���
	//}
	o_file.close();						//�ر��ļ�

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
			


			int Q_BarTime_1 = hours * 60 * 60 + minutes * 60 + seconds;						//ʱ��������
			double Q_BarTime_2 = (1 / 10e1)*hours + (1 / 10e3)*minutes + (1 / 10e5)*seconds;	//ʱ���ʽ0.145100 = 14��51��00	

																								//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX���Ӧ�ü�ʶ�Ƿ�ָ�Q_BarTime_1n[i] = Q_BarTime_1;

			q->tick_data[0] = 2;		//���ñ�־λ
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




			check1 = (q->tick_data[2]>0.0910 && q->tick_data[2]<0.0915 && typecheck1) || (q->tick_data[2]<0.1518 && q->tick_data[2]>0.0914 && !q->FristTick && typecheck1);  //��ծ
			check2 = (q->tick_data[2]>0.0925 && q->tick_data[2]<0.0930 && typecheck2) || (q->tick_data[2]<0.1505 && q->tick_data[2]>0.0929 && !q->FristTick && typecheck2); //��ָ
			check3 = (q->tick_data[2]>0.0856 && q->tick_data[2]<0.0900 && typecheck3) || (q->tick_data[2]<0.1505 && q->tick_data[2]>0.0859 && !q->FristTick && typecheck3);  //����
			check4 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck4) || ((q->tick_data[2]<0.2335 && q->tick_data[2]>0.2059) && !q->FristTick && typecheck4); //��ҹ��  23:30����
			check5 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck5) || ((q->tick_data[2]<0.2305 && q->tick_data[2]>0.2059) && !q->FristTick && typecheck5); //��ҹ��  23:00����
			check6 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck6) || ((q->tick_data[2]<0.0105 || q->tick_data[2]>0.2059) && !q->FristTick && typecheck6); //��ҹ��  1:00����
			check7 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck7) || ((q->tick_data[2]<0.0235 || q->tick_data[2]>0.2059) && !q->FristTick && typecheck7); //��ҹ��  2:30����  


			if ((check1 || check2 || check3 || check4 || check5 || check6 || check7))
			{
				q->M1Klinesig = true;
				q->tick_data[8] = pDepthMarketData->UpperLimitPrice;	//��ͣ��
				q->tick_data[9] = pDepthMarketData->LowerLimitPrice;	//��ͣ��					


				if ((pDepthMarketData->OpenPrice<1e-10) || _isnan(pDepthMarketData->OpenPrice))
				{
					//printf("pDepthMarketData->OpenPrice <1e-10\n");

					char chr[150] = { 0 };
					_snprintf_s(chr, sizeof(chr), sizeof(chr), "OpenPrice<1e-10��OpenPrice������Ч������[%0.06f][%0.02f]", Q_BarTime_2, pDepthMarketData->LastPrice);
					WirteSingleRecordToFile8(0, chr);
					return;
				}

				if (pDepthMarketData->OpenPrice>((pDepthMarketData->UpperLimitPrice) + 1) || pDepthMarketData->OpenPrice<((pDepthMarketData->LowerLimitPrice) - 1))
				{
					//printf("pDepthMarketData->OpenPrice�����ǵ�ͣ��\n");

					char chr[150] = { 0 };
					_snprintf_s(chr, sizeof(chr), sizeof(chr), "pDepthMarketData->OpenPrice�����ǵ�ͣ��[%0.06f][%0.02f] ��ͣ[%0.02f] ��ͣ[%0.02f]", Q_BarTime_2, pDepthMarketData->LastPrice, pDepthMarketData->UpperLimitPrice, pDepthMarketData->LowerLimitPrice);
					WirteSingleRecordToFile8(0, chr);
					return;
				}

				q->Day_open[0] = pDepthMarketData->OpenPrice;

				q->M1_open[1] = q->Day_open[0];
				q->M1_high[1] = q->Day_open[0];
				q->M1_low[1] = q->Day_open[0];
				q->M1_close[1] = q->Day_open[0];

				q->M1_open[0] = q->Day_open[0];
				q->M1_high[0] = q->Day_open[0];		    //Ϊʲô��Ӱ��ˢ�£��Ƿ��б�Ҫ���ڣ�-----------------------------------------------------------------------------------
				q->M1_low[0] = q->Day_open[0];
				q->M1_close[0] = q->Day_open[0];
				//Ϊʲô��Ӱ��ˢ�£��Ƿ��б�Ҫ���ڣ�-----------------------------------------------------------------------------------

				//�ָ� printf("\nPPPPPPPPPPPPPPPPPPPPPPPPP__________________________���������____________________________pppppppppppppppppppppppppp");
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
				q->M5_BS_Time[0] = q->tick_BS_Time[0];	   //Ϊʲôÿ�ζ�ˢ��


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

														   //?????????????��ɶ��
														   //q->Day_CloseProfit = 0;
														   //q->Day_CloseProfitA = 0;
														   //q->Day_CloseProfitB = 0;

														   //q->Day_TradeNumb = 0;

			}






			//������60-129
			for (int j = max_pure_tick_move_num; j >MAX_TICK_MOVE_NUM; j--) //ע�������������ν�ѭ����
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
				q->tick_OpenInterest[j] = q->tick_OpenInterest[j - 1]; //�ֲ���

																	   
				q->tick_BS_BUY_Value[j] = q->tick_BS_BUY_Value[j - 1];
				//tick_BS_BUY_Time[j] = tick_BS_BUY_Time[j-1];
				q->tick_BS_SELL_Value[j] = q->tick_BS_SELL_Value[j - 1];
				//tick_BS_SELL_Time[j] = tick_BS_SELL_Time[j-1];
				q->tick_BS_PURE_Value[j] = q->tick_BS_PURE_Value[j - 1];
				q->tick_BS_Time[j] = q->tick_BS_Time[j - 1];
				q->tick_BS_BL_Value[j] = q->tick_BS_BL_Value[j - 1];
				//			tick_BS_BL_Time[j] = tick_BS_BL_Time[j-1];

			}



			//ʵ�̿���ǰ��¼���������˴���������ֵ�������δ���
			//bool sstate;
			// if (serverselect) //ʵ��ʱ��
			//{
			//�ϵ�sstate=(tick_data[2]>0.0250 && tick_data[2]<0.0850) || (tick_data[2]>0.1550 && tick_data[2]<0.2050); 
			bool sstate = (q->tick_data[2]>0.0250 && q->tick_data[2]<0.0850) || (q->tick_data[2]>0.1550 && q->tick_data[2]<0.2050);
			//}
			//else            //24Сʱ������
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
			q->tick_Volume[0] = pDepthMarketData->Volume;  //�ܳɽ���   ��λʱ��ɽ���tick_Volume[0]-tick_Volume[1]
			q->tick_OpenInterest[0] = pDepthMarketData->OpenInterest;



			//printf("\n�ʼ� [%.02f] ��һ��[%.02f] ��һ��[%.02f]\n",tick_data[4],tick_AskPrice1[0],tick_BidPrice1[0]);
			//�ָ�a  		printf("\n ��һ[%.02f] ��һ[%.02f] ����[%.02f]ʱ��[%.08f]\n",tick_AskVolume1[0]-tick_BidVolume1[1],tick_OpenInterest[0],tick_data[3],Q_BarTime_2); //2�κ�������һ����˵��û����

			//���ÿ�ʳɽ���	

			//char DD[200];
			//string ttt=DD;


			//Ϊ���ظ�2��
			double thisvol = 0;
			//���
			if (q->tick_Volume[1]>0)
			{
				thisvol = q->tick_Volume[0] - q->tick_Volume[1];
			}
			//���

			if (q->tick_data[4] == q->tick_AskPrice1[0])  //��һ�۳ɽ���Ӧ������������
			{
				q->tick_BS_BUY_Value[0] = thisvol;
				q->tick_BS_SELL_Value[0] = 0;
				q->tick_BS_PURE_Value[0] = thisvol;
				// tick_BS_BUY_Time[0]=  tick_data[2];
				// tick_BS_SELL_Time[0]= tick_data[2];
				q->tick_BS_Time[0] = q->tick_data[2];
				//printf("\n���������� [%.00f] ��һ��[%.00f] ��һ��[%.00f]\n",tick_Volume[0]-tick_Volume[1],tick_AskVolume1[0],tick_BidVolume1[0]);
				// printf("\n���������� [%.00f]\n",tick_Volume[0]-tick_Volume[1]);
			}
			else //��һ�۳ɽ���Ӧ������������
			{
				q->tick_BS_BUY_Value[0] = 0;
				q->tick_BS_SELL_Value[0] = thisvol;//pDepthMarketData->Volume;
				q->tick_BS_PURE_Value[0] = (-1)*thisvol;//(-1)*pDepthMarketData->Volume;
														// tick_BS_BUY_Time[0]=  tick_data[2];
														// tick_BS_SELL_Time[0]= tick_data[2];
				q->tick_BS_Time[0] = q->tick_data[2];
				// printf("\n���������� [%.00f]\n",tick_Volume[1]-tick_Volume[0]);

			}

		

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////



			//printf("%s ����Ƿ��������\n", InstrumentID);
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
			case TYPE_T:  //ע���ծʱ��û����
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
				//printf("************************����**********************\n\n");

				//char th[20] = { 0 };
				//sprintf_s(th,sizeof(th),"%d   %0.06f", q->period_M1, Q_BarTime_2);
				//WirteSingleRecordToFile8(0, th);


				q->tick_VolumeLast = pDepthMarketData->Volume;

				q->M1Klinesig = true;



				if (q->period_M1)
				{

					//printf("%s ����M1\n", InstrumentID);

					for (int j = max_pure_m1_move_num; j >0; j--) //1~59ִ����
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

							bool tempdata = true; //��ʱ������Ӧ��ɾ������
						}
						else if (_isnan(q->M1_open[j - 1]) || _isnan(q->M1_high[j - 1]) || _isnan(q->M1_low[j - 1]) || _isnan(q->M1_close[j - 1]) || _isnan(q->M1_BS_Time[j - 1]) || _isnan(q->M1_BS_BUY_Value[j - 1]) || _isnan(q->M1_BS_SELL_Value[j - 1]))
						{

							if (j - 1 < 4)
							{
								//printf("����(��������) [%d][%0.06f] M1_open[%0.02f] M1_high[%0.02f] M1_low[%0.02f] M1_close[%0.02f] M1_BS_Time[%0.02f] M1_BS_BUY_Value[%0.02f] M1_BS_SELL_Value[%0.02f]\n", j - 1, Q_BarTime_2, M1_open[j - 1], M1_high[j - 1], M1_low[j - 1], M1_close[j - 1], M1_BS_Time[j - 1], M1_BS_BUY_Value[j - 1], M1_BS_SELL_Value[j - 1]);

								//char chr[150] = { 0 };
								//sprintf_s(chr, sizeof(chr), "����(��������) [%d][%s]M1_open[%0.02f] M1_high[%0.02f] M1_low[%0.02f] M1_close[%0.02f] M1_BS_Time[%0.02f] M1_BS_BUY_Value[%0.02f] M1_BS_SELL_Value[%0.02f]", j - 1, thistime, q->M1_open[j - 1], q->M1_high[j - 1], q->M1_low[j - 1], q->M1_close[j - 1], q->M1_BS_Time[j - 1], q->M1_BS_BUY_Value[j - 1], q->M1_BS_SELL_Value[j - 1]);
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
							printf("M1:[%d]����[%0.02f]����[%0.02f]���[%0.02f]���[%0.02f]\n", i, q->M1_open[i], q->M1_close[i], q->M1_high[i], q->M1_low[i]);

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
							printf("M3:[%d]����[%0.02f]����[%0.02f]���[%0.02f]���[%0.02f]\n", i, q->M3_open[i], q->M3_close[i], q->M3_high[i], q->M3_low[i]);

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
							printf("M5:[%d]����[%0.02f]����[%0.02f]���[%0.02f]���[%0.02f]\n", i, q->M5_open[i], q->M5_close[i], q->M5_high[i], q->M5_low[i]);

							char th[100] = { 0 };
							_snprintf_s(th, sizeof(th), sizeof(th), "[%d]%0.06f M5 open[%0.02f]close[%0.02f]high[%0.02f]low[%0.02f]", i, q->M5_BS_Time[i], q->M5_open[i], q->M5_close[i], q->M5_high[i], q->M5_low[i]);
							WirteSingleRecordToFile8(0, th);
						}



					}
















				}

				int tempm = 0;
				if (q->period_M3)
				{
					//printf("%s ����M3\n", InstrumentID);
					//M3����
					int tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M3_BS_Time[1]);
					if (NotFirst_3M)
					{

						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M3_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M3_BS_Time[1]));
						//if (tempm >= 3-1 || (tempm<0 && tempm >= -1437-1))
						if (tempm >= 3 || (tempm<0 && tempm >= -1437))
						{
							for (int j = max_pure_m3_move_num; j >1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M3_open[1] = q->M1_open[3];//M3_open[0];
							q->M3_high[1] = zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1])); //M3_high[j - 1];
							q->M3_low[1] = zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])); //M3_low[j - 1];
							q->M3_close[1] = q->M1_close[1]; //M3_close[j - 1];
							q->M3_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //M1_BS_Time[0];//M3_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M3_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// M3_BS_PURE_Value[j - 1];
							q->M3_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// M3_BS_BUY_Value[j - 1];
							q->M3_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//M3_BS_SELL_Value[j - 1];
							q->M3_BS_BL_Value[1] = q->M3_BS_BUY_Value[1] / q->M3_BS_SELL_Value[1];   //M3_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M3_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// M3_BS_BIG_PURE_Value[j - 1];
							q->M3_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// M3_BS_BIG_BUY_Value[j - 1];
							q->M3_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// M3_BS_BIG_SELL_Value[j - 1];
							q->M3_BS_BIG_BL_Value[1] = q->M3_BS_BIG_BUY_Value[1] / q->M3_BS_BIG_SELL_Value[1];// M3_BS_BIG_BL_Value[j - 1];
																											  //��M1���ӵĴ�����
																											  // int id = i;
																											  //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]M3_BS_Time[%0.02f] M3_close[%0.02f] M3_BS_PURE_Value[%0.02f] M3_BS_BUY_Value[%0.02f] M3_BS_SELL_Value[%0.02f] \n",
																											  //  tick_data[2], InstrumentID_n[id], i, M3_BS_Time[id][1], M3_close[id][1], M3_BS_PURE_Value[id][1], M3_BS_BUY_Value[id][1], M3_BS_SELL_Value[id][1]);
						}


					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 3 || (tempm<0 && tempm >= -1437)) && CheckFirst(q->M1_BS_Time[1], 3))
						{

							NotFirst_3M = true;

						}
					}
					//M3����		
				}


				if (q->period_M5)
				{
					//printf("%s ����M5\n", InstrumentID);
					//M5����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M5_BS_Time[1]);
					if (NotFirst_5M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M5_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M5_BS_Time[1]));
						//if (tempm >= 5-1 || (tempm < 0 && tempm >= -1435-1))

						if (tempm >= 5 || (tempm < 0 && tempm >= -1435))
						{
							for (int j = max_pure_m5_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M5_open[1] = q->M1_open[5];//M5_open[0];
							q->M5_high[1] = zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))));
							q->M5_low[1] = zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))));
							q->M5_close[1] = q->M1_close[1]; //M5_close[j - 1];
							q->M5_BS_Time[1] = q->M1_BS_Time[1]; //5�����ڵ����1��������ʱ�䣩   //M1_BS_Time[0];//M5_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ


																 //δ����
							q->M5_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// M5_BS_PURE_Value[j - 1];
							q->M5_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// M5_BS_BUY_Value[j - 1];
							q->M5_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//M5_BS_SELL_Value[j - 1];
							q->M5_BS_BL_Value[1] = q->M5_BS_BUY_Value[1] / q->M5_BS_SELL_Value[1];   //M5_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M5_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// M5_BS_BIG_PURE_Value[j - 1];
							q->M5_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// M5_BS_BIG_BUY_Value[j - 1];
							q->M5_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// M5_BS_BIG_SELL_Value[j - 1];
							q->M5_BS_BIG_BL_Value[1] = q->M5_BS_BIG_BUY_Value[1] / q->M5_BS_BIG_SELL_Value[1];// M5_BS_BIG_BL_Value[j - 1];

																											  //δ����
																											  //��M1���ӵĴ�����
																											  // int id = i;
																											  //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]M5_BS_Time[%0.02f] M5_close[%0.02f] M5_BS_PURE_Value[%0.02f] M5_BS_BUY_Value[%0.02f] M5_BS_SELL_Value[%0.02f] \n",
																											  //  tick_data[2], InstrumentID_n[id], i, M5_BS_Time[id][1], M5_close[id][1], M5_BS_PURE_Value[id][1], M5_BS_BUY_Value[id][1], M5_BS_SELL_Value[id][1]);
						}
					}
					else
					{

						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 5 || (tempm<0 && tempm >= -1435)) && CheckFirst(q->M1_BS_Time[1], 5))
						{

							NotFirst_5M = true;

						}


					}
					//M5����		

				}


				if (q->period_M10)
				{
					//printf("%s ����M10\n", InstrumentID);
					//M10����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M10_BS_Time[1]);
					if (NotFirst_10M)
					{

						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M10_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M10_BS_Time[1]));
						if (tempm >= 10 || (tempm < 0 && tempm >= -1430))
						{
							for (int j = max_pure_m10_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M10_open[1] = q->M1_open[10];//q->M10_open[0];
							q->M10_high[1] = zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))))))))); //q->M10_high[j - 1];
							q->M10_low[1] = zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))))))))); //q->M10_low[j - 1];
							q->M10_close[1] = q->M1_close[1]; //q->M10_close[j - 1];
							q->M10_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //M1_BS_Time[0];//q->M10_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M10_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M10_BS_PURE_Value[j - 1];
							q->M10_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M10_BS_BUY_Value[j - 1];
							q->M10_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M10_BS_SELL_Value[j - 1];
							q->M10_BS_BL_Value[1] = q->M10_BS_BUY_Value[1] / q->M10_BS_SELL_Value[1];   //q->M10_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M10_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M10_BS_BIG_PURE_Value[j - 1];
							q->M10_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M10_BS_BIG_BUY_Value[j - 1];
							q->M10_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M10_BS_BIG_SELL_Value[j - 1];
							q->M10_BS_BIG_BL_Value[1] = q->M10_BS_BIG_BUY_Value[1] / q->M10_BS_BIG_SELL_Value[1];// q->M10_BS_BIG_BL_Value[j - 1];
																												 //��M1���ӵĴ�����
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M10_BS_Time[%0.02f] q->M10_close[%0.02f] q->M10_BS_PURE_Value[%0.02f] q->M10_BS_BUY_Value[%0.02f] q->M10_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M10_BS_Time[id][1], q->M10_close[id][1], q->M10_BS_PURE_Value[id][1], q->M10_BS_BUY_Value[id][1], q->M10_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 10 || (tempm<0 && tempm >= -1430)) && CheckFirst(q->M1_BS_Time[1], 10))
						{

							NotFirst_10M = true;

						}
					}
					//M10����


				}

				if (q->period_M15)
				{
					//printf("%s ����M15\n", InstrumentID);
					//M15����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M15_BS_Time[1]);
					if (NotFirst_15M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M15_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M15_BS_Time[1]));
						if (tempm >= 15 || (tempm < 0 && tempm >= -1425))
						{
							for (int j = max_pure_m15_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M15_open[1] = q->M1_open[15];//M15_open[0];

							q->M15_high[1] = zero_max(q->M1_high[15], zero_max(q->M1_high[14], zero_max(q->M1_high[13], zero_max(q->M1_high[12], zero_max(q->M1_high[11], zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1])))))))))))))); //q->M10_high[j - 1];
							q->M15_low[1] = zero_min(q->M1_low[15], zero_min(q->M1_low[14], zero_min(q->M1_low[13], zero_min(q->M1_low[12], zero_min(q->M1_low[11], zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])))))))))))))); //M15_low[j - 1];
							q->M15_close[1] = q->M1_close[1]; //q->M15_close[j - 1];
							q->M15_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //q->M1_BS_Time[0];//q->M15_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M15_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M15_BS_PURE_Value[j - 1];
							q->M15_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M15_BS_BUY_Value[j - 1];
							q->M15_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M15_BS_SELL_Value[j - 1];
							q->M15_BS_BL_Value[1] = q->M15_BS_BUY_Value[1] / q->M15_BS_SELL_Value[1];   //q->M15_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M15_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M15_BS_BIG_PURE_Value[j - 1];
							q->M15_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M15_BS_BIG_BUY_Value[j - 1];
							q->M15_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M15_BS_BIG_SELL_Value[j - 1];
							q->M15_BS_BIG_BL_Value[1] = q->M15_BS_BIG_BUY_Value[1] / q->M15_BS_BIG_SELL_Value[1];// q->M15_BS_BIG_BL_Value[j - 1];
																												 //��M1���ӵĴ�����
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M15_BS_Time[%0.02f] q->M15_close[%0.02f] q->M15_BS_PURE_Value[%0.02f] q->M15_BS_BUY_Value[%0.02f] q->M15_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M15_BS_Time[id][1], q->M15_close[id][1], q->M15_BS_PURE_Value[id][1], q->M15_BS_BUY_Value[id][1], q->M15_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 15 || (tempm<0 && tempm >= -1425)) && CheckFirst(q->M1_BS_Time[1], 15))
						{

							NotFirst_15M = true;

						}
					}
					//M15����			

				}


				if (q->period_M30)
				{

					//M30����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M30_BS_Time[1]);
					if (NotFirst_30M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] q->M1_BS_Time[%0.06f] M30_BS_Time[%0.06f]\n", tempm, (q->M1_BS_Time[1]), (M30_BS_Time[1]));
						if (tempm >= 30 || (tempm < 0 && tempm >= -1410))
						{
							for (int j = max_pure_m30_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M30_open[1] = q->M1_open[30];//M30_open[0];
															//M30_high[1] = max(q->M1_high[3], max(q->M1_high[2], q->M1_high[1])); //M30_high[j - 1];
															//M30_low[1] = zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])); //M30_low[j - 1];

							q->M30_high[1] = zero_max(q->M1_high[30], zero_max(q->M1_high[29], zero_max(q->M1_high[28], zero_max(q->M1_high[27], zero_max(q->M1_high[26], zero_max(q->M1_high[25], zero_max(q->M1_high[24], zero_max(q->M1_high[23], zero_max(q->M1_high[22], zero_max(q->M1_high[21], zero_max(q->M1_high[20], zero_max(q->M1_high[19], zero_max(q->M1_high[18], zero_max(q->M1_high[17], zero_max(q->M1_high[16], zero_max(q->M1_high[15], zero_max(q->M1_high[14], zero_max(q->M1_high[13], zero_max(q->M1_high[12], zero_max(q->M1_high[11], zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))))))))))))))))))))))))))))); //q->M15_high[j - 1];
							q->M30_low[1] = zero_min(q->M1_low[30], zero_min(q->M1_low[29], zero_min(q->M1_low[28], zero_min(q->M1_low[27], zero_min(q->M1_low[26], zero_min(q->M1_low[25], zero_min(q->M1_low[24], zero_min(q->M1_low[23], zero_min(q->M1_low[22], zero_min(q->M1_low[21], zero_min(q->M1_low[20], zero_min(q->M1_low[19], zero_min(q->M1_low[18], zero_min(q->M1_low[17], zero_min(q->M1_low[16], zero_min(q->M1_low[15], zero_min(q->M1_low[14], zero_min(q->M1_low[13], zero_min(q->M1_low[12], zero_min(q->M1_low[11], zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))))))))))))))))))))))))))))); //q->M15_low[j - 1];

							q->M30_close[1] = q->M1_close[1]; //q->M30_close[j - 1];
							q->M30_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //q->M1_BS_Time[0];//q->M30_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M30_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M30_BS_PURE_Value[j - 1];
							q->M30_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M30_BS_BUY_Value[j - 1];
							q->M30_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M30_BS_SELL_Value[j - 1];
							q->M30_BS_BL_Value[1] = q->M30_BS_BUY_Value[1] / q->M30_BS_SELL_Value[1];   //q->M30_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M30_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M30_BS_BIG_PURE_Value[j - 1];
							q->M30_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M30_BS_BIG_BUY_Value[j - 1];
							q->M30_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M30_BS_BIG_SELL_Value[j - 1];
							q->M30_BS_BIG_BL_Value[1] = q->M30_BS_BIG_BUY_Value[1] / q->M30_BS_BIG_SELL_Value[1];// q->M30_BS_BIG_BL_Value[j - 1];
																												 //��M1���ӵĴ�����
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M30_BS_Time[%0.02f] q->M30_close[%0.02f] q->M30_BS_PURE_Value[%0.02f] q->M30_BS_BUY_Value[%0.02f] q->M30_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M30_BS_Time[id][1], q->M30_close[id][1], q->M30_BS_PURE_Value[id][1], q->M30_BS_BUY_Value[id][1], q->M30_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 30 || (tempm<0 && tempm >= -1410)) && CheckFirst(q->M1_BS_Time[1], 30))
						{
							NotFirst_30M = true;
						}
					}
					//M30����			

				}

				if (q->period_M60)
				{

					//M60����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M60_BS_Time[1]);
					if (NotFirst_60M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] q->M1_BS_Time[%0.06f] q->M60_BS_Time[%0.06f]\n", tempm, (q->M1_BS_Time[1]), (q->M60_BS_Time[1]));
						if (tempm >= 60 || (tempm < 0 && tempm >= -1380))
						{
							for (int j = max_pure_m60_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
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
							q->M60_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //q->M1_BS_Time[0];//q->M60_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M60_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M60_BS_PURE_Value[j - 1];
							q->M60_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M60_BS_BUY_Value[j - 1];
							q->M60_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M60_BS_SELL_Value[j - 1];
							q->M60_BS_BL_Value[1] = q->M60_BS_BUY_Value[1] / q->M60_BS_SELL_Value[1];   //q->M60_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M60_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M60_BS_BIG_PURE_Value[j - 1];
							q->M60_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M60_BS_BIG_BUY_Value[j - 1];
							q->M60_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M60_BS_BIG_SELL_Value[j - 1];
							q->M60_BS_BIG_BL_Value[1] = q->M60_BS_BIG_BUY_Value[1] / q->M60_BS_BIG_SELL_Value[1];// q->M60_BS_BIG_BL_Value[j - 1];
																												 //��M1���ӵĴ�����
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M60_BS_Time[%0.02f] q->M60_close[%0.02f] q->M60_BS_PURE_Value[%0.02f] q->M60_BS_BUY_Value[%0.02f] q->M60_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M60_BS_Time[id][1], q->M60_close[id][1], q->M60_BS_PURE_Value[id][1], q->M60_BS_BUY_Value[id][1], q->M60_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 60 || (tempm<0 && tempm >= -1380)) && CheckFirst(q->M1_BS_Time[1], 60))
						{
							NotFirst_60M = true;
						}
					}
					//M60����		

				}






				//����1M���������,���鿪һ�����߳�
				//if (tempdata == true)
				//{
				//�ָ�printf("\n���������\n");
				//for (int k = 0;k<TYPE_NUM;k++)
				//	{
				//	CalRelativity(i, k);
				//	}

				//}

				//����1M��������ԣ����鿪һ�����߳�    

				//����Ƿ���Ųλ֮ǰ����

				
				//printf("M1 pDepthMarketData->LastPrice[%0.02f][%0.02f][%0.02f][%0.02f]\n",q->M1_open[0],q->M1_high[0],q->M1_low[0], q->M1_close[0]);
				q->M1_open[0] = pDepthMarketData->LastPrice;
				q->M1_high[0] = pDepthMarketData->LastPrice;
				q->M1_low[0] = pDepthMarketData->LastPrice;
				q->M1_close[0] = pDepthMarketData->LastPrice;	//�Ƿ��ǰ��ÿ��TICK��=0�ظ���	���Ƿ����ɾ��ǰ��	=0��ֵ
																


																//TOTAL����ֻ�ۼƣ��������0


				q->M1_BS_Time[0] = q->tick_BS_Time[0];
				//ÿ����֮����1��

				for (int j = 0; j < MAX_PURE_TICK_PERMIN_NUM; j++)
				{


					//printf("A3\n");
					//double temptime=tick_BS_Time[j];
					//double temptime2=tick_BS_Time[j+1];
					//��������
					//cout.setf(ios::fixed);
					// cout.setf(ios::showpoint);
					//cout.precision(4);

					//cout<<temptime;
					//cout<<temptime2;
					//��������
					//if(tick_BS_Time[j]>0)
					////printf("ʱ�䰡B%d[%.06f][%.06f]\n",j,tick_BS_Time[j], ((double)(tick_BS_Time[j])).precision(2));
					//printf("ʱ�䰡XXXX______________   %d[%.06f]_[%.06f]\n",j,tick_BS_Time[j],temptime);

					//if(temptime-temptime2>=0.0001 && (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //һ����ʱ����������Ҫ�ų�00:00��
					// if(  (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //һ����ʱ����������Ҫ�ų�00:00��
					// {

					q->M1_BS_PURE_Value[0] = q->M1_BS_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_BUY_Value[0] = q->M1_BS_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//�߼��Ƿ�������
					q->M1_BS_SELL_Value[0] = q->M1_BS_SELL_Value[0] + q->tick_BS_SELL_Value[j];

					//bl,middle�����
					q->M1_BS_TOTAL_PURE_Value[0] = q->M1_BS_TOTAL_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_TOTAL_BUY_Value[0] = q->M1_BS_TOTAL_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//�߼��Ƿ�������
					q->M1_BS_TOTAL_SELL_Value[0] = q->M1_BS_TOTAL_SELL_Value[0] + q->tick_BS_SELL_Value[j];

					q->M1_BS_CROSS_TOTAL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_CROSS_TOTAL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//�߼��Ƿ�������
					q->M1_BS_CROSS_TOTAL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SELL_Value[0] + q->tick_BS_SELL_Value[j];


					//TOTAL ����CROSS?
					//С��





					double tb = q->tick_BS_BUY_Value[j];
					double ts = q->tick_BS_SELL_Value[j];


					if (tb < CAL_MIDDLE_AMOUNT)
					{
						q->M1_BS_SMALL_BUY_Value[0] = q->M1_BS_SMALL_BUY_Value[0] + tb;
						q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_TOTAL_SMALL_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] + tb;  //���


						q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] + tb;  //���
					}

					//�е�
					else if (tb >= CAL_MIDDLE_AMOUNT && tb < CAL_BIG_AMOUNT)
					{
						// printf("[%d]�е� %0.02f\n", j, tb);
						q->M1_BS_MIDDLE_BUY_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //���

						q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //���
					}

					else// if (tb >= CAL_BIG_AMOUNT)
					{
						//printf("[%d]�� %0.02f\n", j, tb);
						q->M1_BS_BIG_BUY_Value[0] = q->M1_BS_BIG_BUY_Value[0] + tb;
						q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_TOTAL_BIG_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] + tb;  //���

						q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] + tb;  //���
					}



					if (ts < CAL_MIDDLE_AMOUNT)
					{
						q->M1_BS_SMALL_SELL_Value[0] = q->M1_BS_SMALL_SELL_Value[0] + ts;
						q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_TOTAL_SMALL_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] - ts;  //���

						q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] - ts;  //���
					}
					else if (ts >= CAL_MIDDLE_AMOUNT && ts < CAL_BIG_AMOUNT)
					{
						q->M1_BS_MIDDLE_SELL_Value[0] = q->M1_BS_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //���

						q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //���
					}
					else// if (ts >= CAL_BIG_AMOUNT)
					{
						q->M1_BS_BIG_SELL_Value[0] = q->M1_BS_BIG_SELL_Value[0] + ts;
						q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_TOTAL_BIG_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] - ts;  //���

						q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] - ts;  //���
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
					//printf("��ǰʱ�䣺[%.06f]\n", tick_data[2]);
					//printf("\n__%dʱ�䣺[%.06f][%d][%s]tb[%0.02f]ts[%0.02f]����[%0.02f]��[%0.02f]��[%0.02f]����[%0.02f]С��[%0.02f]С��[%0.02f]С��[%0.02f]С������[%0.02f]�о�[%0.02f]����[%0.02f]����[%0.02f]�е�����[%0.02f]��[%0.02f]����[%0.02f]����[%0.02f]�󵥱���[%0.02f]\n",


					//}
					//printf("------------------\n");




				}






				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3����


				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3����




				//��ӡK��������
				//	 cerr << "--->>> K:" << InstrumentID_n << "_" <<dbtoch(tick_data[1])<< "_" << tick_data[2]<< "_" << q->M1_open[1]<< "_" << q->M1_high[1]<< "_" << q->M1_low[1]<< "_" << q->M1_close[1] << endl;
			}
			else
			{
				// printf("\n\n\n_______________________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx__________________________kaishi \n\n\n");
				//�������һ��K�ߣ��Լ�������
				if (q->M1_open[0] == 0) { q->M1_open[0] = pDepthMarketData->LastPrice; }

				q->M1_high[0] = max(q->M1_high[0], pDepthMarketData->LastPrice);
				q->M1_low[0] = zero_min(q->M1_low[0], pDepthMarketData->LastPrice);
				q->M1_close[0] = pDepthMarketData->LastPrice;

				//q->M1_BS_PURE_Value[1]=q->M1_BS_PURE_Value[0];
				//q->M1_BS_PURE_Value[0]=NewVolume;

				//q->M1_BS_PURE_Value[0]=tick_BS_PURE_Value[0]+tick_BS_PURE_Value[1]+tick_BS_PURE_Value[2];
				//q->M1_BS_BUY_Value[0]=tick_BS_BUY_Value[0]+tick_BS_BUY_Value[1]+tick_BS_BUY_Value[2];
				//q->M1_BS_SELL_Value[0]=tick_BS_SELL_Value[0]+tick_BS_SELL_Value[1]+tick_BS_SELL_Value[2];

				//TICK���˺�ÿ��1��TICK��1����Ӧ�ø�Ϊ60��TICK��ÿ��2��TICK��Ӧ��λ120��
				//q->M1_BS_PURE_Value[0]=0;
				//q->M1_BS_BUY_Value[0]=0;
				//q->M1_BS_SELL_Value[0]=0;
				//printf("BBBBB \n");
				// printf("��һ����\n");
				//for(int j=0;j<MAX_PURE_TICK_PERMIN_NUM;j++)
				//{



				// cout<<temptime;
				//cout<<temptime2;
				//��������
				//if(tick_BS_Time[j]>0)
				////printf("ʱ�䰡B%d[%.06f][%.06f]\n",j,tick_BS_Time[j], ((double)(tick_BS_Time[j])).precision(2));
				//printf("ʱ�䰡B   %d[%.06f]_[%.06f]\n",j,tick_BS_Time[j],temptime);
				//printf("CCCCC \n");

				//  if(temptime-temptime2>=0.0001 && (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //һ����ʱ����������Ҫ�ų�00:00��
				// if((tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //һ����ʱ����������Ҫ�ų�00:00��

				//if(tick_BS_Time[j]>0)
				// printf("ʱ���[%.06f][%.06f]_[%.06f]\n",tick_BS_Time[j],tick_BS_Time[j+1],temptime-temptime2);
				//if(tick_BS_Time[j]>0)
				//printf("ʱ���TICK[%.06f]M1[%.06f]_M2[%.06f]\n",tick_BS_Time[j],tempq->M1_A,tempq->M1_B);

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
					q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] + tb;  //���

					q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] + tb;  //���
				}

				//�е�
				else if (tb >= CAL_MIDDLE_AMOUNT && tb < CAL_BIG_AMOUNT)
				{
					// printf("[%d]�е� %0.02f\n", j, tb);
					q->M1_BS_MIDDLE_BUY_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //���

					q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //���
				}

				else// if (tb >= CAL_BIG_AMOUNT)
				{
					//printf("[%d]�� %0.02f\n", j, tb);
					q->M1_BS_BIG_BUY_Value[0] = q->M1_BS_BIG_BUY_Value[0] + tb;
					q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_TOTAL_BIG_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] + tb;  //���

					q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] + tb;  //���
				}

				if (ts < CAL_MIDDLE_AMOUNT)
				{
					q->M1_BS_SMALL_SELL_Value[0] = q->M1_BS_SMALL_SELL_Value[0] + ts;
					q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_TOTAL_SMALL_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] - ts;  //���

					q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] + ts;
					q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] - ts;  //���
				}
				else if (ts >= CAL_MIDDLE_AMOUNT && ts < CAL_BIG_AMOUNT)
				{
					q->M1_BS_MIDDLE_SELL_Value[0] = q->M1_BS_MIDDLE_SELL_Value[0] + ts;
					q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //���
				}
				else// if (ts >= CAL_BIG_AMOUNT)
				{
					q->M1_BS_BIG_SELL_Value[0] = q->M1_BS_BIG_SELL_Value[0] + ts;
					q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_TOTAL_BIG_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] - ts;  //���
				}






				//printf("1���Ӿ���B[%d]����[%d]����[%d]���������[%.02f]\n",(int)M1_BS_PURE_Value[0],(int)M1_BS_BUY_Value[0],(int)M1_BS_SELL_Value[0],MMBL);
				//M1_BS_PURE_Value[0]=Plus(M1_BS_PURE_Value[0],tick_BS_PURE_Value[0]);
				//M1_BS_BL_Value[0]=Ave(M1_BS_BL_Value[0],M1_BS_BL_Value[1],M1_BS_BL_Value[2],M1_BS_BL_Value[3],M1_BS_BL_Value[4],M1_BS_BL_Value[5],M1_BS_BL_Value[6],M1_BS_BL_Value[7],M1_BS_BL_Value[8],M1_BS_BL_Value[9]);
				




				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3����

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3����
				if (q->TickFileWritepaths[0] == '\0')
				{
					q->tick_data[8] = pDepthMarketData->UpperLimitPrice;
					q->tick_data[9] = pDepthMarketData->LowerLimitPrice;

					strcpy(q->TickFileWritepaths, "./TickData/");
					strcat(q->TickFileWritepaths, pDepthMarketData->InstrumentID);
					strcat(q->TickFileWritepaths, "_");
					//�ָ�strcat_s(q->TickFileWritepaths, TradingDay);
					strcat(q->TickFileWritepaths, ".txt");

					//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
					ifstream inf;
					ofstream ouf;
					inf.open(q->TickFileWritepaths, ios::out);


					/////////////////////////////////////////////////
					//char str[200] = { 0 };
					//strcat_s(str, 200, "SingleData/TradeRecord.txt");

					//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
					//	ifstream inf;
					//ofstream ouf;
					//inf.open(TickFileWritepaths[i], ios::out);
					//inf.open(str, ios::out);
					//}

					//��¼TICK����
					//ofstream o_file(str, ios::app);
					//if ( RunMode && ( check0 || check1 || check2) )

					//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

					//o_file << content << "\t" << endl;

					//else
					//{
					//o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //������д�뵽�ı��ļ���
					//}
					//o_file.close();						//�ر��ļ�




				}

				//��¼TICK����
				ofstream o_file(q->TickFileWritepaths, ios::app);

				if (check1 || check2 || check3 || check4 || check5 || check6 || check7)
				{
					o_file << dbtoch(q->tick_data[1]) << "\t" << q->tick_data[2] << "\t" << pDepthMarketData->UpdateMillisec << "\t" << q->tick_AskPrice1[0] << "\t" << q->tick_AskVolume1[0] << "\t" << q->tick_BidPrice1[0] << "\t" << q->tick_BidVolume1[0] << "\t" << q->tick_data[4] << "\t" << dbtoch(q->tick_Volume[0]) << "\t" << dbtoch(q->tick_OpenInterest[0]) << "\t" << q->Day_open[2] << "\t" << q->Day_high[2] << "\t" << q->Day_low[2] << "\t" << q->Day_close[2] << "\t" << q->Day_open[1] << "\t" << q->Day_high[1] << "\t" << q->Day_low[1] << "\t" << q->Day_close[1] << "\t" << q->tick_data[8] << "\t" << q->tick_data[9] << endl;
				}
				else
				{
					o_file << dbtoch(q->tick_data[1]) << "\t" << q->tick_data[2] << "\t" << pDepthMarketData->UpdateMillisec << "\t" << q->tick_AskPrice1[0] << "\t" << q->tick_AskVolume1[0] << "\t" << q->tick_BidPrice1[0] << "\t" << q->tick_BidVolume1[0] << "\t" << q->tick_data[4] << "\t" << dbtoch(q->tick_Volume[0]) << "\t" << dbtoch(q->tick_OpenInterest[0]) << endl; //������д�뵽�ı��ļ���
				}
				o_file.close();						//�ر��ļ�


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
				//��ҹ��
				bool FistCheck2 = (!FistCheck1) && (!typecheck1) && (!typecheck2);
				//��ҹ��
				//��if ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i<=17))
				//    if ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i <= 17) || (tick_data[i][2]<0.0230 && i <= 17))  //�޸�9

				if (seconds>45 && seconds<55 && q->M1Klinesig == true)
				{
					//printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxת��xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n\n");
					q->M1Klinesig = false;
				}



				//�������                                                                                //��ҹ��                                               //��ҹ��
				if ((q->tick_data[2]>0.0858 && typecheck1) || (q->tick_data[2]>0.0925 && typecheck2) || (q->tick_data[2]>0.0858 && FistCheck1) || ((q->tick_data[2]>0.2058 || q->tick_data[2]<0.1530) && FistCheck2))  //�޸�9
				{
					q->FristTick = true;
				}

				q->ReceiveTick = false;
				q->tick_data[0] = 1;		//���ñ�־λ

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
									   //CROSS��ͳ��

									   //double	tick_data[10];				//TICK��������
									   //double	tick_AskPrice1[MAX_TICK_NUM];			//��һ��
									   //double	tick_BidPrice1[MAX_TICK_NUM];			//��һ��
									   //double	tick_AskVolume1[MAX_TICK_NUM];		//��һ��
									   //double	tick_BidVolume1[MAX_TICK_NUM];		//��һ��
									   //double	tick_Volume[MAX_TICK_NUM];			//�ɽ���
									   //double	tick_OpenInterest[MAX_TICK_NUM];		//�ֲ���



	delete[]q->tick_BS_BIG_BUY_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->tick_BS_BIG_SELL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->tick_BS_BIG_PURE_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������

	delete[]q->tick_BS_BUY_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->tick_BS_SELL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->tick_BS_PURE_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������


							   // double	tick_BS_BUY_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
							   // double	tick_BS_SELL_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->tick_BS_Time;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������

	delete[]q->tick_BS_BL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


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

	delete[]q->M1_open;// = new double[max_pure_m1_num];	//����K�߿�
	delete[]q->M1_high;// = new double[max_pure_m1_num];	//����K�߸�
	delete[]q->M1_low;// = new double[max_pure_m1_num];		//����K�ߵ�
	delete[]q->M1_close;// = new double[max_pure_m1_num];	//����K����
					 //bool	M1Klinesig;		//1����K�߱�־
	delete[]q->M1_BS_Time;// = new double[max_pure_m1_num];


	delete[]q->M1_BS_BIG_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_BIG_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_BIG_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_BIG_BL_Value; //= new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M1_BS_MIDDLE_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_MIDDLE_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_MIDDLE_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_MIDDLE_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M1_BS_SMALL_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_SMALL_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_SMALL_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_SMALL_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M1_BS_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M1_BS_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������




						   //3����
	delete[]q->M3_open;// = new double[max_pure_m3_num];	//����K�߿�
	delete[]q->M3_high;// = new double[max_pure_m3_num];	//����K�߸�
	delete[]q->M3_low;//= new double[max_pure_m3_num];		//����K�ߵ�
	delete[]q->M3_close;// = new double[max_pure_m3_num];	//����K����
					 //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->M3_BS_Time;// = new double[max_pure_m3_num];
					   // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
					   // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
					   // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
					   // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	delete[]q->M3_BS_BIG_PURE_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M3_BS_BIG_BUY_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M3_BS_BIG_SELL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M3_BS_BIG_BL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M3_BS_BUY_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M3_BS_SELL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M3_BS_PURE_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M3_BS_BL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
						   //3����


						   //5����
	delete[]q->M5_open;// = new double[max_pure_m5_num];	//����K�߿�
	delete[]q->M5_high;// = new double[max_pure_m5_num];	//����K�߸�
	delete[]q->M5_low;// = new double[max_pure_m5_num];		//����K�ߵ�
	delete[]q->M5_close;// = new double[max_pure_m5_num];	//����K����
					 //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->M5_BS_Time;// = new double[max_pure_m5_num];
					   // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
					   // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
					   // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
					   // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	delete[]q->M5_BS_BIG_PURE_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M5_BS_BIG_BUY_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M5_BS_BIG_SELL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M5_BS_BIG_BL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M5_BS_BUY_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M5_BS_SELL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M5_BS_PURE_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M5_BS_BL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
						   //5����


						   //10����
	delete[]q->M10_open;// = ; new double[max_pure_m10_num];	//����K�߿�
	delete[]q->M10_high;// = new double[max_pure_m10_num];	//����K�߸�
	delete[]q->M10_low;// = new double[max_pure_m10_num];		//����K�ߵ�
	delete[]q->M10_close;// = new double[max_pure_m10_num];	//����K����
					  //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->M10_BS_Time;// = new double[max_pure_m10_num];
						// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	delete[]q->M10_BS_BIG_PURE_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M10_BS_BIG_BUY_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M10_BS_BIG_SELL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M10_BS_BIG_BL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M10_BS_BUY_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M10_BS_SELL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M10_BS_PURE_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M10_BS_BL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
							//10����

							//15����
	delete[]q->M15_open;// = new double[max_pure_m15_num];	//����K�߿�
	delete[]q->M15_high;// = new double[max_pure_m15_num];	//����K�߸�
	delete[]q->M15_low;// = new double[max_pure_m15_num];		//����K�ߵ�
	delete[]q->M15_close;// = new double[max_pure_m15_num];	//����K����
					  //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->M15_BS_Time;// = new double[max_pure_m15_num];
						// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	delete[]q->M15_BS_BIG_PURE_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M15_BS_BIG_BUY_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M15_BS_BIG_SELL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M15_BS_BIG_BL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M15_BS_BUY_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M15_BS_SELL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M15_BS_PURE_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M15_BS_BL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
							//15����



							//30����
	delete[]q->M30_open;// = new double[max_pure_m30_num];	//����K�߿�
	delete[]q->M30_high;// = new double[max_pure_m30_num];	//����K�߸�
	delete[]q->M30_low;// = new double[max_pure_m30_num];		//����K�ߵ�
	delete[]q->M30_close;// = new double[max_pure_m30_num];	//����K����
					  //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->M30_BS_Time;// = new double[max_pure_m30_num];
						// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	delete[]q->M30_BS_BIG_PURE_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M30_BS_BIG_BUY_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M30_BS_BIG_SELL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M30_BS_BIG_BL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M30_BS_BUY_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M30_BS_SELL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M30_BS_PURE_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M30_BS_BL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
							//30����


							//60����
	delete[]q->M60_open;// = new double[max_pure_m60_num];	//����K�߿�
	delete[]q->M60_high;// = new double[max_pure_m60_num];	//����K�߸�
	delete[]q->M60_low;// = new double[max_pure_m60_num];		//����K�ߵ�
	delete[]q->M60_close;// = new double[max_pure_m60_num];	//����K����
					  //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->M60_BS_Time;// = new double[max_pure_m60_num];
						// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
						// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	delete[]q->M60_BS_BIG_PURE_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M60_BS_BIG_BUY_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M60_BS_BIG_SELL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M60_BS_BIG_BL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	delete[]q->M60_BS_BUY_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M60_BS_SELL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M60_BS_PURE_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	delete[]q->M60_BS_BL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������								
							//60����


							//120����
	delete[]q->M120_open;// = new double[max_pure_m120_num];	//����K�߿�
	delete[]q->M120_high;// = new double[max_pure_m120_num];	//����K�߸�
	delete[]q->M120_low;// = new double[max_pure_m120_num];		//����K�ߵ�
	delete[]q->M120_close;// = new double[max_pure_m120_num];	//����K����
					   //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->M120_BS_Time;// = new double[max_pure_m120_num];
						 // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
						 // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
						 // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
						 // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	delete[]q->M120_BS_BIG_PURE_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M120_BS_BIG_BUY_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M120_BS_BIG_SELL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M120_BS_BIG_BL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������


	delete[]q->M120_BS_BUY_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M120_BS_SELL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M120_BS_PURE_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M120_BS_BL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
							 //120����

							 //240����
	delete[]q->M240_open;// = new double[max_pure_m240_num];	//����K�߿�
	delete[]q->M240_high;// = new double[max_pure_m240_num];	//����K�߸�
	delete[]q->M240_low;// = new double[max_pure_m240_num];		//����K�ߵ�
	delete[]q->M240_close;// = new double[max_pure_m240_num];	//����K����
					   //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->M240_BS_Time;// = new double[max_pure_m240_num];
						 // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
						 // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
						 // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
						 // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	delete[]q->M240_BS_BIG_PURE_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M240_BS_BIG_BUY_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M240_BS_BIG_SELL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M240_BS_BIG_BL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������


	delete[]q->M240_BS_BUY_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M240_BS_SELL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M240_BS_PURE_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->M240_BS_BL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
							 //240����


							 //240����
	delete[]q->D1_open;// = new double[max_pure_d1_num];	//����K�߿�
	delete[]q->D1_high;// = new double[max_pure_d1_num];	//����K�߸�
	delete[]q->D1_low;// = new double[max_pure_d1_num];		//����K�ߵ�
	delete[]q->D1_close;// = new double[max_pure_d1_num];	//����K����
					 //	bool	M3Klinesig[20];		//3����K�߱�־
	delete[]q->D1_BS_Time;// = new double[max_pure_d1_num];
					   // double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
					   // double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
					   // double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
					   // double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	delete[]q->D1_BS_BIG_PURE_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->D1_BS_BIG_BUY_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->D1_BS_BIG_SELL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->D1_BS_BIG_BL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������


	delete[]q->D1_BS_BUY_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->D1_BS_SELL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->D1_BS_PURE_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	delete[]q->D1_BS_BL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
						   //240����


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
	q->sort = 0;//space/ƽ��ʱ������

			 //dwLastActiveTime=0;			 // ��¼���û��Ļʱ�䣨������ʹ�ã�
			 //LoginErrorTime=0;
			 //LoginErrorNum=0;
			 //char keystr[MAX_USERNAME];	 // �û���

	memset(&(q->keystr), 0, MAX_USERNAME);
	//memset(&checkcod,0,sizeof(checkcod));
	//DWORD ip[30];  //6���ַ�+1λ\0
	//u_short port[6];  //6���ַ�+1λ\0

	q->EmaCount = 0;
	//si=0;     //���ҳ���
	//PEER_RECORD_INFO deallist;
	//ADDR_RADER_INFO addr[MAX_ADDR_NUMBER]; // �ɽڵ��˽���ն˺͹����ն���ɵ�����
	//u_char AddrNum;					 // addr����Ԫ������
	//ADDR_RADER_INFO p2pAddr;				 // P2Pͨ��ʱӦʹ�õĵ�ַ���ͻ���ʹ�ã�
	//char oncehash[7];
	//memset(tick_data, 0, sizeof(tick_data));
	//enum  entry_status states;

	//PositionAmount[TYPE_NUM]= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //�ֲַ�������
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




	//int CROSS_SINGLE_SELECT[TYPE_NUM][MAX_CROSSSINGLE_NUM];   //�����źŷ��� 0�գ�1��  2��


	//CROSS_SINGLE_TURN[TYPE_NUM][MAX_CROSSSINGLE_NUM]= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //����


	//CROSS_SINGLE_PRICE[TYPE_NUM][MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��۸�
	// memset(CROSS_SINGLE_PRICE, 0, sizeof(CROSS_SINGLE_PRICE));

	//CROSS_SINGLE_SPACE_VALUE[TYPE_NUM][MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��ռ�
	// memset(CROSS_SINGLE_SPACE_VALUE, 0, sizeof(CROSS_SINGLE_SPACE_VALUE));

	//CROSS_SINGLE_SPACE_RATE[TYPE_NUM][MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮�䲨���ռ��ʣ�Ӧ�ô���3������
	//memset(CROSS_SINGLE_SPACE_RATE, 0, sizeof(CROSS_SINGLE_SPACE_RATE));


	//CROSS_SINGLE_SPACE_PER_M1[TYPE_NUM][MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�ۺϳ�ÿ�����ź�֮��ƽ�������ռ��ʣ�Ӧ�ô���3������
	//memset(CROSS_SINGLE_SPACE_PER_M1, 0, sizeof(CROSS_SINGLE_SPACE_PER_M1));

	//double CROSS_SINGLE_SPACE_VALUE_M_EMA_L[MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��ռ�,EMA
	//double CROSS_SINGLE_SPACE_VALUE_M_EMA_R[MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��ռ�,EMA
	//double CROSS_SINGLE_SPACE_VALUE_N_EMA_L[MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��ռ�,EMA
	//double CROSS_SINGLE_SPACE_VALUE_N_EMA_R[MAX_CROSSSINGLE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��ռ�,EMA


	//EMA_SPACE_M[TYPE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // ��M��EMA�ռ�
	// memset(EMA_SPACE_M, 0, sizeof(EMA_SPACE_M));

	//EMA_SPACE_N[TYPE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // ��N��EMA�ռ�
	//memset(EMA_SPACE_N, 0, sizeof(EMA_SPACE_N));

	//  //EMA_SPACE_MN[TYPE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };// ��MN��EMA ƽ���ռ�
	//memset(EMA_SPACE_MN, 0, sizeof(EMA_SPACE_MN));
	//PEER_INFO   OrderUserPeerList;  // �����ò����ֵ��û��б�;
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
															 //CROSS��ͳ��

															 //double	tick_data[10];				//TICK��������
															 //double	tick_AskPrice1[MAX_TICK_NUM];			//��һ��
															 //double	tick_BidPrice1[MAX_TICK_NUM];			//��һ��
															 //double	tick_AskVolume1[MAX_TICK_NUM];		//��һ��
															 //double	tick_BidVolume1[MAX_TICK_NUM];		//��һ��
															 // double	tick_Volume[MAX_TICK_NUM];			//�ɽ���
															 // double	tick_OpenInterest[MAX_TICK_NUM];		//�ֲ���



	q->tick_BS_BIG_BUY_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->tick_BS_BIG_BUY_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BIG_BUY_Value));


	q->tick_BS_BIG_SELL_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->tick_BS_BIG_SELL_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BIG_SELL_Value));


	q->tick_BS_BIG_PURE_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->tick_BS_BIG_PURE_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BIG_PURE_Value));


	q->tick_BS_BUY_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->tick_BS_BUY_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_BUY_Value));


	q->tick_BS_SELL_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->tick_BS_SELL_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_SELL_Value));


	q->tick_BS_PURE_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->tick_BS_PURE_Value, 0, max_pure_tick_num * sizeof(q->tick_BS_PURE_Value));

														// double	tick_BS_BUY_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
														// double	tick_BS_SELL_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	q->tick_BS_Time = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->tick_BS_Time, 0, max_pure_tick_num * sizeof(q->tick_BS_Time));


	q->tick_BS_BL_Value = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
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



	q->M1_open = new double[max_pure_m1_num];	//����K�߿�
	memset(q->M1_open, 0, max_pure_m1_num * sizeof(q->M1_open));

	q->M1_high = new double[max_pure_m1_num];	//����K�߸�
	memset(q->M1_high, 0, max_pure_m1_num * sizeof(q->M1_high));

	q->M1_low = new double[max_pure_m1_num];		//����K�ߵ�
	memset(q->M1_low, 0, max_pure_m1_num * sizeof(q->M1_low));


	q->M1_close = new double[max_pure_m1_num];	//����K����
	memset(q->M1_close, 0, max_pure_m1_num * sizeof(q->M1_close));

											//bool	M1Klinesig;		//1����K�߱�־
	q->M1_BS_Time = new double[max_pure_m1_num];
	memset(q->M1_BS_Time, 0, max_pure_m1_num * sizeof(q->M1_BS_Time));

	q->M1_BS_BIG_PURE_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_BIG_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BIG_PURE_Value));


	q->M1_BS_BIG_BUY_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_BIG_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BIG_BUY_Value));


	q->M1_BS_BIG_SELL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_BIG_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BIG_SELL_Value));


	q->M1_BS_BIG_BL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M1_BS_BIG_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BIG_BL_Value));

	q->M1_BS_MIDDLE_PURE_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_MIDDLE_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_MIDDLE_PURE_Value));


	q->M1_BS_MIDDLE_BUY_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_MIDDLE_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_MIDDLE_BUY_Value));


	q->M1_BS_MIDDLE_SELL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_MIDDLE_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_MIDDLE_SELL_Value));


	q->M1_BS_MIDDLE_BL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M1_BS_MIDDLE_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_MIDDLE_BL_Value));


	q->M1_BS_SMALL_PURE_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_SMALL_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SMALL_PURE_Value));


	q->M1_BS_SMALL_BUY_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_SMALL_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SMALL_BUY_Value));


	q->M1_BS_SMALL_SELL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_SMALL_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SMALL_SELL_Value));



	q->M1_BS_SMALL_BL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M1_BS_SMALL_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SMALL_BL_Value));

	q->M1_BS_PURE_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_PURE_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_PURE_Value));


	q->M1_BS_BUY_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_BUY_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BUY_Value));


	q->M1_BS_SELL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M1_BS_SELL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_SELL_Value));


	q->M1_BS_BL_Value = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M1_BS_BL_Value, 0, max_pure_m1_num * sizeof(q->M1_BS_BL_Value));



													//3����
	q->M3_open = new double[max_pure_m3_num];	//����K�߿�
	memset(q->M3_open, 0, max_pure_m3_num * sizeof(q->M3_open));


	q->M3_high = new double[max_pure_m3_num];	//����K�߸�
	memset(q->M3_high, 0, max_pure_m3_num * sizeof(q->M3_high));


	q->M3_low = new double[max_pure_m3_num];		//����K�ߵ�
	memset(q->M3_low, 0, max_pure_m3_num * sizeof(q->M3_low));

	q->M3_close = new double[max_pure_m3_num];	//����K����
	memset(q->M3_close, 0, max_pure_m3_num * sizeof(q->M3_close));


											//	bool	M3Klinesig[20];		//3����K�߱�־
	q->M3_BS_Time = new double[max_pure_m3_num];
	memset(q->M3_BS_Time, 0, max_pure_m3_num * sizeof(q->M3_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	q->M3_BS_BIG_PURE_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M3_BS_BIG_PURE_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BIG_PURE_Value));


	q->M3_BS_BIG_BUY_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M3_BS_BIG_BUY_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BIG_BUY_Value));


	q->M3_BS_BIG_SELL_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M3_BS_BIG_SELL_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BIG_SELL_Value));


	q->M3_BS_BIG_BL_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M3_BS_BIG_BL_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BIG_BL_Value));

	q->M3_BS_BUY_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M3_BS_BUY_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BUY_Value));


	q->M3_BS_SELL_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M3_BS_SELL_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_SELL_Value));



	q->M3_BS_PURE_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M3_BS_PURE_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_PURE_Value));


	q->M3_BS_BL_Value = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M3_BS_BL_Value, 0, max_pure_m3_num * sizeof(q->M3_BS_BL_Value));												//3����


													//5����
	q->M5_open = new double[max_pure_m5_num];	//����K�߿�
	memset(q->M5_open, 0, max_pure_m5_num * sizeof(q->M5_open));

	q->M5_high = new double[max_pure_m5_num];	//����K�߸�
	memset(q->M5_high, 0, max_pure_m5_num * sizeof(q->M5_high));


	q->M5_low = new double[max_pure_m5_num];		//����K�ߵ�
	memset(q->M5_low, 0, max_pure_m5_num * sizeof(q->M5_low));

	q->M5_close = new double[max_pure_m5_num];	//����K����
	memset(q->M5_close, 0, max_pure_m5_num * sizeof(q->M5_close));

											//	bool	M3Klinesig[20];		//3����K�߱�־
	q->M5_BS_Time = new double[max_pure_m5_num];
	memset(q->M5_BS_Time, 0, max_pure_m5_num * sizeof(q->M5_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	q->M5_BS_BIG_PURE_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M5_BS_BIG_PURE_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BIG_PURE_Value));

	q->M5_BS_BIG_BUY_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M5_BS_BIG_BUY_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BIG_BUY_Value));

	q->M5_BS_BIG_SELL_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M5_BS_BIG_SELL_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BIG_SELL_Value));

	q->M5_BS_BIG_BL_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M5_BS_BIG_BL_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BIG_BL_Value));

	q->M5_BS_BUY_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M5_BS_BUY_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BUY_Value));


	q->M5_BS_SELL_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M5_BS_SELL_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_SELL_Value));

	q->M5_BS_PURE_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M5_BS_PURE_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_PURE_Value));


	q->M5_BS_BL_Value = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M5_BS_BL_Value, 0, max_pure_m5_num * sizeof(q->M5_BS_BL_Value));

													//10����
	q->M10_open = new double[max_pure_m10_num];	//����K�߿�
	memset(q->M10_open, 0, max_pure_m10_num * sizeof(q->M10_open));

	q->M10_high = new double[max_pure_m10_num];	//����K�߸�
	memset(q->M10_high, 0, max_pure_m10_num * sizeof(q->M10_high));


	q->M10_low = new double[max_pure_m10_num];		//����K�ߵ�
	memset(q->M10_low, 0, max_pure_m10_num * sizeof(q->M10_low));

	q->M10_close = new double[max_pure_m10_num];	//����K����
	memset(q->M10_close, 0, max_pure_m10_num * sizeof(q->M10_close));
												//	bool	M3Klinesig[20];		//3����K�߱�־
	q->M10_BS_Time = new double[max_pure_m10_num];
	memset(q->M10_BS_Time, 0, max_pure_m10_num * sizeof(q->M10_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	q->M10_BS_BIG_PURE_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M10_BS_BIG_PURE_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BIG_PURE_Value));

	q->M10_BS_BIG_BUY_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M10_BS_BIG_BUY_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BIG_BUY_Value));


	q->M10_BS_BIG_SELL_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M10_BS_BIG_SELL_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BIG_SELL_Value));


	q->M10_BS_BIG_BL_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M10_BS_BIG_BL_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BIG_BL_Value));

	q->M10_BS_BUY_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M10_BS_BUY_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BUY_Value));

	q->M10_BS_SELL_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M10_BS_SELL_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_SELL_Value));

	q->M10_BS_PURE_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M10_BS_PURE_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_PURE_Value));


	q->M10_BS_BL_Value = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M10_BS_BL_Value, 0, max_pure_m10_num * sizeof(q->M10_BS_BL_Value));
													//10����

													//15����
	q->M15_open = new double[max_pure_m15_num];	//����K�߿�
	memset(q->M15_open, 0, max_pure_m15_num * sizeof(q->M15_open));

	q->M15_high = new double[max_pure_m15_num];	//����K�߸�
	memset(q->M15_high, 0, max_pure_m15_num * sizeof(q->M15_high));

	q->M15_low = new double[max_pure_m15_num];		//����K�ߵ�
	memset(q->M15_low, 0, max_pure_m15_num * sizeof(q->M15_low));

	q->M15_close = new double[max_pure_m15_num];	//����K����
	memset(q->M15_close, 0, max_pure_m15_num * sizeof(q->M15_close));
												//	bool	M3Klinesig[20];		//3����K�߱�־
	q->M15_BS_Time = new double[max_pure_m15_num];
	memset(q->M15_BS_Time, 0, max_pure_m15_num * sizeof(q->M15_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	q->M15_BS_BIG_PURE_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M15_BS_BIG_PURE_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BIG_PURE_Value));

	q->M15_BS_BIG_BUY_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M15_BS_BIG_BUY_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BIG_BUY_Value));


	q->M15_BS_BIG_SELL_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M15_BS_BIG_SELL_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BIG_SELL_Value));

	q->M15_BS_BIG_BL_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M15_BS_BIG_BL_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BIG_BL_Value));

	q->M15_BS_BUY_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M15_BS_BUY_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BUY_Value));


	q->M15_BS_SELL_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M15_BS_SELL_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_SELL_Value));


	q->M15_BS_PURE_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M15_BS_PURE_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_PURE_Value));

	q->M15_BS_BL_Value = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M15_BS_BL_Value, 0, max_pure_m15_num * sizeof(q->M15_BS_BL_Value));												//15����



													//30����
	q->M30_open = new double[max_pure_m30_num];	//����K�߿�
	memset(q->M30_open, 0, max_pure_m30_num * sizeof(q->M30_open));

	q->M30_high = new double[max_pure_m30_num];	//����K�߸�
	memset(q->M30_high, 0, max_pure_m30_num * sizeof(q->M30_high));


	q->M30_low = new double[max_pure_m30_num];		//����K�ߵ�
	memset(q->M30_low, 0, max_pure_m30_num * sizeof(q->M30_low));


	q->M30_close = new double[max_pure_m30_num];	//����K����
	memset(q->M30_close, 0, max_pure_m30_num * sizeof(q->M30_close));

												//	bool	M3Klinesig[20];		//3����K�߱�־
	q->M30_BS_Time = new double[max_pure_m30_num];
	memset(q->M30_BS_Time, 0, max_pure_m30_num * sizeof(q->M30_BS_Time));

	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	q->M30_BS_BIG_PURE_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M30_BS_BIG_PURE_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BIG_PURE_Value));


	q->M30_BS_BIG_BUY_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M30_BS_BIG_BUY_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BIG_BUY_Value));

	q->M30_BS_BIG_SELL_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M30_BS_BIG_SELL_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BIG_SELL_Value));

	q->M30_BS_BIG_BL_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M30_BS_BIG_BL_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BIG_BL_Value));

	q->M30_BS_BUY_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M30_BS_BUY_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BUY_Value));

	q->M30_BS_SELL_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M30_BS_SELL_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_SELL_Value));


	q->M30_BS_PURE_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M30_BS_PURE_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_PURE_Value));


	q->M30_BS_BL_Value = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M30_BS_BL_Value, 0, max_pure_m30_num * sizeof(q->M30_BS_BL_Value));											//30����


													//60����
	q->M60_open = new double[max_pure_m60_num];	//����K�߿�
	memset(q->M60_open, 0, max_pure_m60_num * sizeof(q->M60_open));


	q->M60_high = new double[max_pure_m60_num];	//����K�߸�
	memset(q->M60_high, 0, max_pure_m60_num * sizeof(q->M60_high));


	q->M60_low = new double[max_pure_m60_num];		//����K�ߵ�
	memset(q->M60_low, 0, max_pure_m60_num * sizeof(q->M60_low));


	q->M60_close = new double[max_pure_m60_num];	//����K����
	memset(q->M60_close, 0, max_pure_m60_num * sizeof(q->M60_close));

												//	bool	M3Klinesig[20];		//3����K�߱�־
	q->M60_BS_Time = new double[max_pure_m60_num];
	memset(q->M60_BS_Time, 0, max_pure_m60_num * sizeof(q->M60_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	q->M60_BS_BIG_PURE_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M60_BS_BIG_PURE_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BIG_PURE_Value));

	q->M60_BS_BIG_BUY_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M60_BS_BIG_BUY_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BIG_BUY_Value));

	q->M60_BS_BIG_SELL_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M60_BS_BIG_SELL_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BIG_SELL_Value));


	q->M60_BS_BIG_BL_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	memset(q->M60_BS_BIG_BL_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BIG_BL_Value));

	q->M60_BS_BUY_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M60_BS_BUY_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BUY_Value));

	q->M60_BS_SELL_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M60_BS_SELL_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_SELL_Value));


	q->M60_BS_PURE_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	memset(q->M60_BS_PURE_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_PURE_Value));

	q->M60_BS_BL_Value = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������								
	memset(q->M60_BS_BL_Value, 0, max_pure_m60_num * sizeof(q->M60_BS_BL_Value));


													//120����
	q->M120_open = new double[max_pure_m120_num];	//����K�߿�
	memset(q->M120_open, 0, max_pure_m120_num * sizeof(q->M120_open));

	q->M120_high = new double[max_pure_m120_num];	//����K�߸�
	memset(q->M120_high, 0, max_pure_m120_num * sizeof(q->M120_high));

	q->M120_low = new double[max_pure_m120_num];		//����K�ߵ�
	memset(q->M120_low, 0, max_pure_m120_num * sizeof(q->M120_low));

	q->M120_close = new double[max_pure_m120_num];	//����K����
	memset(q->M120_close, 0, max_pure_m120_num * sizeof(q->M120_close));
												//	bool	M3Klinesig[20];		//3����K�߱�־
	q->M120_BS_Time = new double[max_pure_m120_num];
	memset(q->M120_BS_Time, 0, max_pure_m120_num * sizeof(q->M120_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	q->M120_BS_BIG_PURE_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M120_BS_BIG_PURE_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BIG_PURE_Value));

	q->M120_BS_BIG_BUY_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M120_BS_BIG_BUY_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BIG_BUY_Value));

	q->M120_BS_BIG_SELL_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M120_BS_BIG_SELL_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BIG_SELL_Value));

	q->M120_BS_BIG_BL_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	memset(q->M120_BS_BIG_BL_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BIG_BL_Value));

	q->M120_BS_BUY_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M120_BS_BUY_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BUY_Value));

	q->M120_BS_SELL_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M120_BS_SELL_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_SELL_Value));

	q->M120_BS_PURE_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M120_BS_PURE_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_PURE_Value));

	q->M120_BS_BL_Value = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
	memset(q->M120_BS_BL_Value, 0, max_pure_m120_num * sizeof(q->M120_BS_BL_Value));												//120����

														//240����
	q->M240_open = new double[max_pure_m240_num];	//����K�߿�
	memset(q->M240_open, 0, max_pure_m240_num * sizeof(q->M240_open));

	q->M240_high = new double[max_pure_m240_num];	//����K�߸�
	memset(q->M240_high, 0, max_pure_m240_num * sizeof(q->M240_high));

	q->M240_low = new double[max_pure_m240_num];		//����K�ߵ�
	memset(q->M240_low, 0, max_pure_m240_num * sizeof(q->M240_low));

	q->M240_close = new double[max_pure_m240_num];	//����K����
	memset(q->M240_close, 0, max_pure_m240_num * sizeof(q->M240_close));
												//	bool	M3Klinesig[20];		//3����K�߱�־
	q->M240_BS_Time = new double[max_pure_m240_num];
	memset(q->M240_BS_Time, 0, max_pure_m240_num * sizeof(q->M240_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	q->M240_BS_BIG_PURE_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M240_BS_BIG_PURE_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BIG_PURE_Value));

	q->M240_BS_BIG_BUY_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M240_BS_BIG_BUY_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BIG_BUY_Value));

	q->M240_BS_BIG_SELL_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M240_BS_BIG_SELL_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BIG_SELL_Value));

	q->M240_BS_BIG_BL_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	memset(q->M240_BS_BIG_BL_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BIG_BL_Value));

	q->M240_BS_BUY_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M240_BS_BUY_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BUY_Value));

	q->M240_BS_SELL_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M240_BS_SELL_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_SELL_Value));

	q->M240_BS_PURE_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->M240_BS_PURE_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_PURE_Value));

	q->M240_BS_BL_Value = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
	memset(q->M240_BS_BL_Value, 0, max_pure_m240_num * sizeof(q->M240_BS_BL_Value));												//240����


														//240����
	q->D1_open = new double[max_pure_d1_num];	//����K�߿�
	memset(q->D1_open, 0, max_pure_d1_num * sizeof(q->D1_open));

	q->D1_high = new double[max_pure_d1_num];	//����K�߸�
	memset(q->D1_high, 0, max_pure_d1_num * sizeof(q->D1_high));

	q->D1_low = new double[max_pure_d1_num];		//����K�ߵ�
	memset(q->D1_low, 0, max_pure_d1_num * sizeof(q->D1_low));

	q->D1_close = new double[max_pure_d1_num];	//����K����
	memset(q->D1_close, 0, max_pure_d1_num * sizeof(q->D1_close));
											//	bool	M3Klinesig[20];		//3����K�߱�־
	q->D1_BS_Time = new double[max_pure_d1_num];
	memset(q->D1_BS_Time, 0, max_pure_d1_num * sizeof(q->D1_BS_Time));
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	q->D1_BS_BIG_PURE_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->D1_BS_BIG_PURE_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BIG_PURE_Value));

	q->D1_BS_BIG_BUY_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->D1_BS_BIG_BUY_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BIG_BUY_Value));

	q->D1_BS_BIG_SELL_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->D1_BS_BIG_SELL_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BIG_SELL_Value));

	q->D1_BS_BIG_BL_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	memset(q->D1_BS_BIG_BL_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BIG_BL_Value));

	q->D1_BS_BUY_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->D1_BS_BUY_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BUY_Value));

	q->D1_BS_SELL_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->D1_BS_SELL_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_SELL_Value));

	q->D1_BS_PURE_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	memset(q->D1_BS_PURE_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_PURE_Value));

	q->D1_BS_BL_Value = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
	memset(q->D1_BS_BL_Value, 0, max_pure_d1_num * sizeof(q->D1_BS_BL_Value));											//240����



}
void UpdatePriceHash(const char * InstrumentID, CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	//double pDepthMarketData->LastPrice = pDepthMarketData->LastPrice;

	if (pDepthMarketData->LastPrice < 1) //�ָ�
										 //if (NewPrice < 5 || thistime<-1e-10) //С��5Ԫ������۸񣬲�������
	{
		return;// false;
	}

	//int  Millisecs = pDepthMarketData->UpdateMillisec;
	//ʱ��

	
	/*
	//ʱ��
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
		//printf("*************û�ҵ��ú�Լ��%s\n", InstrumentID);
		mapData.insert(value);
	}
	else
	{
		printf("xxxxxxxxxxxxxxx�ҵ��ú�Լ��%s\n", InstrumentID);
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
	{	  // printf("*************û�ҵ��ú�Լ��%s\n", InstrumentID);
		    GuestOnlineHash value;
		    InitGuestOnlineHash(&value);
			mapData.insert(std::make_pair(InstrumentID, value));
			//ClearGuestOnlineHash(&value);
	        it = mapData.find(InstrumentID);
	}


	if (it != mapData.end())
	{

		//printf("xxxxxxxxxxxxxxx�ҵ��ú�Լ��%s\n", InstrumentID);
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
			//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX���Ӧ�ü���Ƿ�ָ�ReceiveTick[i] = true;  //�����Ҫ��飬����12���Ժ����е�ԭ��


			int Q_BarTime_1 = hours * 60 * 60 + minutes * 60 + seconds;						//ʱ��������
			double Q_BarTime_2 = (1 / 10e1)*hours + (1 / 10e3)*minutes + (1 / 10e5)*seconds;	//ʱ���ʽ0.145100 = 14��51��00	

																								//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX���Ӧ�ü�ʶ�Ƿ�ָ�Q_BarTime_1n[i] = Q_BarTime_1;

			q->tick_data[0] = 2;		//���ñ�־λ
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




			check1 = (q->tick_data[2]>0.0910 && q->tick_data[2]<0.0915 && typecheck1) || (q->tick_data[2]<0.1518 && q->tick_data[2]>0.0914 && !q->FristTick && typecheck1);  //��ծ
			check2 = (q->tick_data[2]>0.0925 && q->tick_data[2]<0.0930 && typecheck2) || (q->tick_data[2]<0.1505 && q->tick_data[2]>0.0929 && !q->FristTick && typecheck2); //��ָ
			check3 = (q->tick_data[2]>0.0856 && q->tick_data[2]<0.0900 && typecheck3) || (q->tick_data[2]<0.1505 && q->tick_data[2]>0.0859 && !q->FristTick && typecheck3);  //����
			check4 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck4) || ((q->tick_data[2]<0.2335 && q->tick_data[2]>0.2059) && !q->FristTick && typecheck4); //��ҹ��  23:30����
			check5 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck5) || ((q->tick_data[2]<0.2305 && q->tick_data[2]>0.2059) && !q->FristTick && typecheck5); //��ҹ��  23:00����
			check6 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck6) || ((q->tick_data[2]<0.0105 || q->tick_data[2]>0.2059) && !q->FristTick && typecheck6); //��ҹ��  1:00����
			check7 = (q->tick_data[2]>0.2056 && q->tick_data[2]<0.2100 && typecheck7) || ((q->tick_data[2]<0.0235 || q->tick_data[2]>0.2059) && !q->FristTick && typecheck7); //��ҹ��  2:30����  


		    //if(1)
			if ((check1 || check2 || check3 || check4 || check5 || check6 || check7))
			{
				//printf("change......\n");

				q->M1Klinesig = true;
				q->tick_data[8] = pDepthMarketData->UpperLimitPrice;	//��ͣ��
				q->tick_data[9] = pDepthMarketData->LowerLimitPrice;	//��ͣ��					


				if ((pDepthMarketData->OpenPrice<1e-10) || _isnan(pDepthMarketData->OpenPrice))
				{
					//printf("pDepthMarketData->OpenPrice <1e-10\n");

					char chr[150] = { 0 };
					_snprintf_s(chr, sizeof(chr), sizeof(chr), "OpenPrice<1e-10��OpenPrice������Ч������[%0.06f][%0.02f]", Q_BarTime_2, pDepthMarketData->LastPrice);
					WirteSingleRecordToFile8(0, chr);
					return;
				}

				if (pDepthMarketData->OpenPrice>((pDepthMarketData->UpperLimitPrice) + 1) || pDepthMarketData->OpenPrice<((pDepthMarketData->LowerLimitPrice) - 1))
				{
					//printf("pDepthMarketData->OpenPrice�����ǵ�ͣ��\n");

					char chr[150] = { 0 };
					_snprintf_s(chr, sizeof(chr), sizeof(chr), "pDepthMarketData->OpenPrice�����ǵ�ͣ��[%0.06f][%0.02f] ��ͣ[%0.02f] ��ͣ[%0.02f]", Q_BarTime_2, pDepthMarketData->LastPrice, pDepthMarketData->UpperLimitPrice, pDepthMarketData->LowerLimitPrice);
					WirteSingleRecordToFile8(0, chr);
					return;
				}

				q->Day_open[0] = pDepthMarketData->OpenPrice;

				q->M1_open[1] = q->Day_open[0];
				q->M1_high[1] = q->Day_open[0];
				q->M1_low[1] = q->Day_open[0];
				q->M1_close[1] = q->Day_open[0];

				q->M1_open[0] = q->Day_open[0];
				q->M1_high[0] = q->Day_open[0];		    //Ϊʲô��Ӱ��ˢ�£��Ƿ��б�Ҫ���ڣ�-----------------------------------------------------------------------------------
				q->M1_low[0] = q->Day_open[0];
				q->M1_close[0] = q->Day_open[0];
				//Ϊʲô��Ӱ��ˢ�£��Ƿ��б�Ҫ���ڣ�-----------------------------------------------------------------------------------

				//�ָ� printf("\nPPPPPPPPPPPPPPPPPPPPPPPPP__________________________���������____________________________pppppppppppppppppppppppppp");
				q->M1_BS_Time[0] = q->tick_BS_Time[0];	   //Ϊʲôÿ�ζ�ˢ��

				q->M3_open[1] = q->Day_open[0];
				q->M3_high[1] = q->Day_open[0];
				q->M3_low[1] = q->Day_open[0];
				q->M3_close[1] = q->Day_open[0];
				q->M3_open[0] = q->Day_open[0];
				q->M3_high[0] = q->Day_open[0];
				q->M3_low[0] = q->Day_open[0];
				q->M3_close[0] = q->Day_open[0];
				q->M3_BS_Time[0] = q->tick_BS_Time[0];	   //Ϊʲôÿ�ζ�ˢ��


				q->M5_open[1] = q->Day_open[0];
				q->M5_high[1] = q->Day_open[0];
				q->M5_low[1] = q->Day_open[0];
				q->M5_close[1] = q->Day_open[0];
				q->M5_open[0] = q->Day_open[0];
				q->M5_high[0] = q->Day_open[0];
				q->M5_low[0] = q->Day_open[0];
				q->M5_close[0] = q->Day_open[0];
				q->M5_BS_Time[0] = q->tick_BS_Time[0];	   //Ϊʲôÿ�ζ�ˢ��


				q->M10_open[1] = q->Day_open[0];
				q->M10_high[1] = q->Day_open[0];
				q->M10_low[1] = q->Day_open[0];
				q->M10_close[1] = q->Day_open[0];
				q->M10_open[0] = q->Day_open[0];
				q->M10_high[0] = q->Day_open[0];
				q->M10_low[0] = q->Day_open[0];
				q->M10_close[0] = q->Day_open[0];
				q->M10_BS_Time[0] = q->tick_BS_Time[0];	   //Ϊʲôÿ�ζ�ˢ��

				q->M15_open[1] = q->Day_open[0];
				q->M15_high[1] = q->Day_open[0];
				q->M15_low[1] = q->Day_open[0];
				q->M15_close[1] = q->Day_open[0];
				q->M15_open[0] = q->Day_open[0];
				q->M15_high[0] = q->Day_open[0];
				q->M15_low[0] = q->Day_open[0];
				q->M15_close[0] = q->Day_open[0];
				q->M15_BS_Time[0] = q->tick_BS_Time[0];	   //Ϊʲôÿ�ζ�ˢ��


				q->M30_open[1] = q->Day_open[0];
				q->M30_high[1] = q->Day_open[0];
				q->M30_low[1] = q->Day_open[0];
				q->M30_close[1] = q->Day_open[0];
				q->M30_open[0] = q->Day_open[0];
				q->M30_high[0] = q->Day_open[0];
				q->M30_low[0] = q->Day_open[0];
				q->M30_close[0] = q->Day_open[0];
				q->M30_BS_Time[0] = q->tick_BS_Time[0];	   //Ϊʲôÿ�ζ�ˢ��


				q->M60_open[1] = q->Day_open[0];
				q->M60_high[1] = q->Day_open[0];
				q->M60_low[1] = q->Day_open[0];
				q->M60_close[1] = q->Day_open[0];
				q->M60_open[0] = q->Day_open[0];
				q->M60_high[0] = q->Day_open[0];
				q->M60_low[0] = q->Day_open[0];
				q->M60_close[0] = q->Day_open[0];
				q->M60_BS_Time[0] = q->tick_BS_Time[0];	   //Ϊʲôÿ�ζ�ˢ��

														   //?????????????��ɶ��
														   //q->Day_CloseProfit = 0;
														   //q->Day_CloseProfitA = 0;
														   //q->Day_CloseProfitB = 0;

														   //q->Day_TradeNumb = 0;

			}






			//������60-129
			for (int j = max_pure_tick_move_num; j >MAX_TICK_MOVE_NUM; j--) //ע�������������ν�ѭ����
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
				q->tick_OpenInterest[j] = q->tick_OpenInterest[j - 1]; //�ֲ���

																	   
				q->tick_BS_BUY_Value[j] = q->tick_BS_BUY_Value[j - 1];
				//tick_BS_BUY_Time[j] = tick_BS_BUY_Time[j-1];
				q->tick_BS_SELL_Value[j] = q->tick_BS_SELL_Value[j - 1];
				//tick_BS_SELL_Time[j] = tick_BS_SELL_Time[j-1];
				q->tick_BS_PURE_Value[j] = q->tick_BS_PURE_Value[j - 1];
				q->tick_BS_Time[j] = q->tick_BS_Time[j - 1];
				q->tick_BS_BL_Value[j] = q->tick_BS_BL_Value[j - 1];
				//			tick_BS_BL_Time[j] = tick_BS_BL_Time[j-1];
				
			}



			//ʵ�̿���ǰ��¼���������˴���������ֵ�������δ���
			//bool sstate;
			// if (serverselect) //ʵ��ʱ��
			//{
			//�ϵ�sstate=(tick_data[2]>0.0250 && tick_data[2]<0.0850) || (tick_data[2]>0.1550 && tick_data[2]<0.2050); 
			bool sstate = (q->tick_data[2]>0.0250 && q->tick_data[2]<0.0850) || (q->tick_data[2]>0.1550 && q->tick_data[2]<0.2050);
			//}
			//else            //24Сʱ������
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
			q->tick_Volume[0] = pDepthMarketData->Volume;  //�ܳɽ���   ��λʱ��ɽ���tick_Volume[0]-tick_Volume[1]
			q->tick_OpenInterest[0] = pDepthMarketData->OpenInterest;
			


			//printf("\n�ʼ� [%.02f] ��һ��[%.02f] ��һ��[%.02f]\n",tick_data[4],tick_AskPrice1[0],tick_BidPrice1[0]);
			//�ָ�a  		printf("\n ��һ[%.02f] ��һ[%.02f] ����[%.02f]ʱ��[%.08f]\n",tick_AskVolume1[0]-tick_BidVolume1[1],tick_OpenInterest[0],tick_data[3],Q_BarTime_2); //2�κ�������һ����˵��û����

			//���ÿ�ʳɽ���	

			//char DD[200];
			//string ttt=DD;


			//Ϊ���ظ�2��
			double thisvol = 0;
			//���
			if (q->tick_Volume[1]>0)
			{
				thisvol = q->tick_Volume[0] - q->tick_Volume[1];
			}
			//���

			if (q->tick_data[4] == q->tick_AskPrice1[0])  //��һ�۳ɽ���Ӧ������������
			{
				q->tick_BS_BUY_Value[0] = thisvol;
				q->tick_BS_SELL_Value[0] = 0;
				q->tick_BS_PURE_Value[0] = thisvol;
				// tick_BS_BUY_Time[0]=  tick_data[2];
				// tick_BS_SELL_Time[0]= tick_data[2];
				q->tick_BS_Time[0] = q->tick_data[2];
				//printf("\n���������� [%.00f] ��һ��[%.00f] ��һ��[%.00f]\n",tick_Volume[0]-tick_Volume[1],tick_AskVolume1[0],tick_BidVolume1[0]);
				// printf("\n���������� [%.00f]\n",tick_Volume[0]-tick_Volume[1]);
			}
			else //��һ�۳ɽ���Ӧ������������
			{
				q->tick_BS_BUY_Value[0] = 0;
				q->tick_BS_SELL_Value[0] = thisvol;//pDepthMarketData->Volume;
				q->tick_BS_PURE_Value[0] = (-1)*thisvol;//(-1)*pDepthMarketData->Volume;
														// tick_BS_BUY_Time[0]=  tick_data[2];
														// tick_BS_SELL_Time[0]= tick_data[2];
				q->tick_BS_Time[0] = q->tick_data[2];
				// printf("\n���������� [%.00f]\n",tick_Volume[1]-tick_Volume[0]);

			}

					

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////



			//printf("%s ����Ƿ��������\n", InstrumentID);
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
			case TYPE_T:  //ע���ծʱ��û����
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
				//printf("************************����**********************\n\n");

				//char th[20] = { 0 };
				//sprintf_s(th,sizeof(th),"%d   %0.06f", q->period_M1, Q_BarTime_2);
				//WirteSingleRecordToFile8(0, th);


				q->tick_VolumeLast = pDepthMarketData->Volume;

				q->M1Klinesig = true;



				if (q->period_M1)
				{

					//printf("%s ����M1\n", InstrumentID);

					for (int j = max_pure_m1_move_num; j >0; j--) //1~59ִ����
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

							bool tempdata = true; //��ʱ������Ӧ��ɾ������
						}
						else if (_isnan(q->M1_open[j - 1]) || _isnan(q->M1_high[j - 1]) || _isnan(q->M1_low[j - 1]) || _isnan(q->M1_close[j - 1]) || _isnan(q->M1_BS_Time[j - 1]) || _isnan(q->M1_BS_BUY_Value[j - 1]) || _isnan(q->M1_BS_SELL_Value[j - 1]))
						{

							if (j - 1 < 4)
							{
								//printf("����(��������) [%d][%0.06f] M1_open[%0.02f] M1_high[%0.02f] M1_low[%0.02f] M1_close[%0.02f] M1_BS_Time[%0.02f] M1_BS_BUY_Value[%0.02f] M1_BS_SELL_Value[%0.02f]\n", j - 1, Q_BarTime_2, M1_open[j - 1], M1_high[j - 1], M1_low[j - 1], M1_close[j - 1], M1_BS_Time[j - 1], M1_BS_BUY_Value[j - 1], M1_BS_SELL_Value[j - 1]);

								//char chr[150] = { 0 };
								//sprintf_s(chr, sizeof(chr), "����(��������) [%d][%s]M1_open[%0.02f] M1_high[%0.02f] M1_low[%0.02f] M1_close[%0.02f] M1_BS_Time[%0.02f] M1_BS_BUY_Value[%0.02f] M1_BS_SELL_Value[%0.02f]", j - 1, thistime, q->M1_open[j - 1], q->M1_high[j - 1], q->M1_low[j - 1], q->M1_close[j - 1], q->M1_BS_Time[j - 1], q->M1_BS_BUY_Value[j - 1], q->M1_BS_SELL_Value[j - 1]);
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
							printf("M1:[%d]����[%0.02f]����[%0.02f]���[%0.02f]���[%0.02f]\n", i, q->M1_open[i], q->M1_close[i], q->M1_high[i], q->M1_low[i]);

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
							printf("M3:[%d]����[%0.02f]����[%0.02f]���[%0.02f]���[%0.02f]\n", i, q->M3_open[i], q->M3_close[i], q->M3_high[i], q->M3_low[i]);

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
							printf("M5:[%d]����[%0.02f]����[%0.02f]���[%0.02f]���[%0.02f]\n", i, q->M5_open[i], q->M5_close[i], q->M5_high[i], q->M5_low[i]);

							char th[100] = { 0 };
							_snprintf_s(th, sizeof(th), sizeof(th), "[%d]%0.06f M5 open[%0.02f]close[%0.02f]high[%0.02f]low[%0.02f]", i, q->M5_BS_Time[i], q->M5_open[i], q->M5_close[i], q->M5_high[i], q->M5_low[i]);
							WirteSingleRecordToFile8(0, th);
						}



					}
















				}

				int tempm = 0;
				if (q->period_M3)
				{
					//printf("%s ����M3\n", InstrumentID);
					//M3����
					int tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M3_BS_Time[1]);
					if (NotFirst_3M)
					{

						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M3_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M3_BS_Time[1]));
						//if (tempm >= 3-1 || (tempm<0 && tempm >= -1437-1))
						if (tempm >= 3 || (tempm<0 && tempm >= -1437))
						{
							for (int j = max_pure_m3_move_num; j >1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M3_open[1] = q->M1_open[3];//M3_open[0];
							q->M3_high[1] = zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1])); //M3_high[j - 1];
							q->M3_low[1] = zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])); //M3_low[j - 1];
							q->M3_close[1] = q->M1_close[1]; //M3_close[j - 1];
							q->M3_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //M1_BS_Time[0];//M3_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M3_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// M3_BS_PURE_Value[j - 1];
							q->M3_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// M3_BS_BUY_Value[j - 1];
							q->M3_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//M3_BS_SELL_Value[j - 1];
							q->M3_BS_BL_Value[1] = q->M3_BS_BUY_Value[1] / q->M3_BS_SELL_Value[1];   //M3_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M3_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// M3_BS_BIG_PURE_Value[j - 1];
							q->M3_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// M3_BS_BIG_BUY_Value[j - 1];
							q->M3_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// M3_BS_BIG_SELL_Value[j - 1];
							q->M3_BS_BIG_BL_Value[1] = q->M3_BS_BIG_BUY_Value[1] / q->M3_BS_BIG_SELL_Value[1];// M3_BS_BIG_BL_Value[j - 1];
																											  //��M1���ӵĴ�����
																											  // int id = i;
																											  //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]M3_BS_Time[%0.02f] M3_close[%0.02f] M3_BS_PURE_Value[%0.02f] M3_BS_BUY_Value[%0.02f] M3_BS_SELL_Value[%0.02f] \n",
																											  //  tick_data[2], InstrumentID_n[id], i, M3_BS_Time[id][1], M3_close[id][1], M3_BS_PURE_Value[id][1], M3_BS_BUY_Value[id][1], M3_BS_SELL_Value[id][1]);
						}


					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 3 || (tempm<0 && tempm >= -1437)) && CheckFirst(q->M1_BS_Time[1], 3))
						{

							NotFirst_3M = true;

						}
					}
					//M3����		
				}


				if (q->period_M5)
				{
					//printf("%s ����M5\n", InstrumentID);
					//M5����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M5_BS_Time[1]);
					if (NotFirst_5M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M5_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M5_BS_Time[1]));
						//if (tempm >= 5-1 || (tempm < 0 && tempm >= -1435-1))

						if (tempm >= 5 || (tempm < 0 && tempm >= -1435))
						{
							for (int j = max_pure_m5_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M5_open[1] = q->M1_open[5];//M5_open[0];
							q->M5_high[1] = zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))));
							q->M5_low[1] = zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))));
							q->M5_close[1] = q->M1_close[1]; //M5_close[j - 1];
							q->M5_BS_Time[1] = q->M1_BS_Time[1]; //5�����ڵ����1��������ʱ�䣩   //M1_BS_Time[0];//M5_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ


																 //δ����
							q->M5_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// M5_BS_PURE_Value[j - 1];
							q->M5_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// M5_BS_BUY_Value[j - 1];
							q->M5_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//M5_BS_SELL_Value[j - 1];
							q->M5_BS_BL_Value[1] = q->M5_BS_BUY_Value[1] / q->M5_BS_SELL_Value[1];   //M5_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M5_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// M5_BS_BIG_PURE_Value[j - 1];
							q->M5_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// M5_BS_BIG_BUY_Value[j - 1];
							q->M5_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// M5_BS_BIG_SELL_Value[j - 1];
							q->M5_BS_BIG_BL_Value[1] = q->M5_BS_BIG_BUY_Value[1] / q->M5_BS_BIG_SELL_Value[1];// M5_BS_BIG_BL_Value[j - 1];

																											  //δ����
																											  //��M1���ӵĴ�����
																											  // int id = i;
																											  //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]M5_BS_Time[%0.02f] M5_close[%0.02f] M5_BS_PURE_Value[%0.02f] M5_BS_BUY_Value[%0.02f] M5_BS_SELL_Value[%0.02f] \n",
																											  //  tick_data[2], InstrumentID_n[id], i, M5_BS_Time[id][1], M5_close[id][1], M5_BS_PURE_Value[id][1], M5_BS_BUY_Value[id][1], M5_BS_SELL_Value[id][1]);
						}
					}
					else
					{

						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 5 || (tempm<0 && tempm >= -1435)) && CheckFirst(q->M1_BS_Time[1], 5))
						{

							NotFirst_5M = true;

						}


					}
					//M5����		

				}


				if (q->period_M10)
				{
					//printf("%s ����M10\n", InstrumentID);
					//M10����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M10_BS_Time[1]);
					if (NotFirst_10M)
					{

						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M10_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M10_BS_Time[1]));
						if (tempm >= 10 || (tempm < 0 && tempm >= -1430))
						{
							for (int j = max_pure_m10_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M10_open[1] = q->M1_open[10];//q->M10_open[0];
							q->M10_high[1] = zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))))))))); //q->M10_high[j - 1];
							q->M10_low[1] = zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))))))))); //q->M10_low[j - 1];
							q->M10_close[1] = q->M1_close[1]; //q->M10_close[j - 1];
							q->M10_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //M1_BS_Time[0];//q->M10_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M10_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M10_BS_PURE_Value[j - 1];
							q->M10_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M10_BS_BUY_Value[j - 1];
							q->M10_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M10_BS_SELL_Value[j - 1];
							q->M10_BS_BL_Value[1] = q->M10_BS_BUY_Value[1] / q->M10_BS_SELL_Value[1];   //q->M10_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M10_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M10_BS_BIG_PURE_Value[j - 1];
							q->M10_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M10_BS_BIG_BUY_Value[j - 1];
							q->M10_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M10_BS_BIG_SELL_Value[j - 1];
							q->M10_BS_BIG_BL_Value[1] = q->M10_BS_BIG_BUY_Value[1] / q->M10_BS_BIG_SELL_Value[1];// q->M10_BS_BIG_BL_Value[j - 1];
																												 //��M1���ӵĴ�����
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M10_BS_Time[%0.02f] q->M10_close[%0.02f] q->M10_BS_PURE_Value[%0.02f] q->M10_BS_BUY_Value[%0.02f] q->M10_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M10_BS_Time[id][1], q->M10_close[id][1], q->M10_BS_PURE_Value[id][1], q->M10_BS_BUY_Value[id][1], q->M10_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 10 || (tempm<0 && tempm >= -1430)) && CheckFirst(q->M1_BS_Time[1], 10))
						{

							NotFirst_10M = true;

						}
					}
					//M10����


				}

				if (q->period_M15)
				{
					//printf("%s ����M15\n", InstrumentID);
					//M15����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M15_BS_Time[1]);
					if (NotFirst_15M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] M1_BS_Time[%0.06f] M15_BS_Time[%0.06f]\n", tempm, (M1_BS_Time[1]), (M15_BS_Time[1]));
						if (tempm >= 15 || (tempm < 0 && tempm >= -1425))
						{
							for (int j = max_pure_m15_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M15_open[1] = q->M1_open[15];//M15_open[0];

							q->M15_high[1] = zero_max(q->M1_high[15], zero_max(q->M1_high[14], zero_max(q->M1_high[13], zero_max(q->M1_high[12], zero_max(q->M1_high[11], zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1])))))))))))))); //q->M10_high[j - 1];
							q->M15_low[1] = zero_min(q->M1_low[15], zero_min(q->M1_low[14], zero_min(q->M1_low[13], zero_min(q->M1_low[12], zero_min(q->M1_low[11], zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])))))))))))))); //M15_low[j - 1];
							q->M15_close[1] = q->M1_close[1]; //q->M15_close[j - 1];
							q->M15_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //q->M1_BS_Time[0];//q->M15_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M15_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M15_BS_PURE_Value[j - 1];
							q->M15_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M15_BS_BUY_Value[j - 1];
							q->M15_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M15_BS_SELL_Value[j - 1];
							q->M15_BS_BL_Value[1] = q->M15_BS_BUY_Value[1] / q->M15_BS_SELL_Value[1];   //q->M15_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M15_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M15_BS_BIG_PURE_Value[j - 1];
							q->M15_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M15_BS_BIG_BUY_Value[j - 1];
							q->M15_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M15_BS_BIG_SELL_Value[j - 1];
							q->M15_BS_BIG_BL_Value[1] = q->M15_BS_BIG_BUY_Value[1] / q->M15_BS_BIG_SELL_Value[1];// q->M15_BS_BIG_BL_Value[j - 1];
																												 //��M1���ӵĴ�����
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M15_BS_Time[%0.02f] q->M15_close[%0.02f] q->M15_BS_PURE_Value[%0.02f] q->M15_BS_BUY_Value[%0.02f] q->M15_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M15_BS_Time[id][1], q->M15_close[id][1], q->M15_BS_PURE_Value[id][1], q->M15_BS_BUY_Value[id][1], q->M15_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 15 || (tempm<0 && tempm >= -1425)) && CheckFirst(q->M1_BS_Time[1], 15))
						{

							NotFirst_15M = true;

						}
					}
					//M15����			

				}


				if (q->period_M30)
				{

					//M30����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M30_BS_Time[1]);
					if (NotFirst_30M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] q->M1_BS_Time[%0.06f] M30_BS_Time[%0.06f]\n", tempm, (q->M1_BS_Time[1]), (M30_BS_Time[1]));
						if (tempm >= 30 || (tempm < 0 && tempm >= -1410))
						{
							for (int j = max_pure_m30_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
							q->M30_open[1] = q->M1_open[30];//M30_open[0];
															//M30_high[1] = max(q->M1_high[3], max(q->M1_high[2], q->M1_high[1])); //M30_high[j - 1];
															//M30_low[1] = zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1])); //M30_low[j - 1];

							q->M30_high[1] = zero_max(q->M1_high[30], zero_max(q->M1_high[29], zero_max(q->M1_high[28], zero_max(q->M1_high[27], zero_max(q->M1_high[26], zero_max(q->M1_high[25], zero_max(q->M1_high[24], zero_max(q->M1_high[23], zero_max(q->M1_high[22], zero_max(q->M1_high[21], zero_max(q->M1_high[20], zero_max(q->M1_high[19], zero_max(q->M1_high[18], zero_max(q->M1_high[17], zero_max(q->M1_high[16], zero_max(q->M1_high[15], zero_max(q->M1_high[14], zero_max(q->M1_high[13], zero_max(q->M1_high[12], zero_max(q->M1_high[11], zero_max(q->M1_high[10], zero_max(q->M1_high[9], zero_max(q->M1_high[8], zero_max(q->M1_high[7], zero_max(q->M1_high[6], zero_max(q->M1_high[5], zero_max(q->M1_high[4], zero_max(q->M1_high[3], zero_max(q->M1_high[2], q->M1_high[1]))))))))))))))))))))))))))))); //q->M15_high[j - 1];
							q->M30_low[1] = zero_min(q->M1_low[30], zero_min(q->M1_low[29], zero_min(q->M1_low[28], zero_min(q->M1_low[27], zero_min(q->M1_low[26], zero_min(q->M1_low[25], zero_min(q->M1_low[24], zero_min(q->M1_low[23], zero_min(q->M1_low[22], zero_min(q->M1_low[21], zero_min(q->M1_low[20], zero_min(q->M1_low[19], zero_min(q->M1_low[18], zero_min(q->M1_low[17], zero_min(q->M1_low[16], zero_min(q->M1_low[15], zero_min(q->M1_low[14], zero_min(q->M1_low[13], zero_min(q->M1_low[12], zero_min(q->M1_low[11], zero_min(q->M1_low[10], zero_min(q->M1_low[9], zero_min(q->M1_low[8], zero_min(q->M1_low[7], zero_min(q->M1_low[6], zero_min(q->M1_low[5], zero_min(q->M1_low[4], zero_min(q->M1_low[3], zero_min(q->M1_low[2], q->M1_low[1]))))))))))))))))))))))))))))); //q->M15_low[j - 1];

							q->M30_close[1] = q->M1_close[1]; //q->M30_close[j - 1];
							q->M30_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //q->M1_BS_Time[0];//q->M30_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M30_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M30_BS_PURE_Value[j - 1];
							q->M30_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M30_BS_BUY_Value[j - 1];
							q->M30_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M30_BS_SELL_Value[j - 1];
							q->M30_BS_BL_Value[1] = q->M30_BS_BUY_Value[1] / q->M30_BS_SELL_Value[1];   //q->M30_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M30_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M30_BS_BIG_PURE_Value[j - 1];
							q->M30_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M30_BS_BIG_BUY_Value[j - 1];
							q->M30_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M30_BS_BIG_SELL_Value[j - 1];
							q->M30_BS_BIG_BL_Value[1] = q->M30_BS_BIG_BUY_Value[1] / q->M30_BS_BIG_SELL_Value[1];// q->M30_BS_BIG_BL_Value[j - 1];
																												 //��M1���ӵĴ�����
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M30_BS_Time[%0.02f] q->M30_close[%0.02f] q->M30_BS_PURE_Value[%0.02f] q->M30_BS_BUY_Value[%0.02f] q->M30_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M30_BS_Time[id][1], q->M30_close[id][1], q->M30_BS_PURE_Value[id][1], q->M30_BS_BUY_Value[id][1], q->M30_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 30 || (tempm<0 && tempm >= -1410)) && CheckFirst(q->M1_BS_Time[1], 30))
						{
							NotFirst_30M = true;
						}
					}
					//M30����			

				}

				if (q->period_M60)
				{

					//M60����
					tempm = GetMin(q->M1_BS_Time[1]) - GetMin(q->M60_BS_Time[1]);
					if (NotFirst_60M)
					{
						//  00:00-23:57=0-1438=-1437     00:01-23:57=1-1438=-1438        00:02-23:58=1-1439=-1436 
						//printf("*******tempm[%d] q->M1_BS_Time[%0.06f] q->M60_BS_Time[%0.06f]\n", tempm, (q->M1_BS_Time[1]), (q->M60_BS_Time[1]));
						if (tempm >= 60 || (tempm < 0 && tempm >= -1380))
						{
							for (int j = max_pure_m60_move_num; j > 1; j--) //2~59ִ����
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

							//��M1���ӵĴ�����
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
							q->M60_BS_Time[1] = q->M1_BS_Time[1]; //3�����ڵ����1��������ʱ�䣩   //q->M1_BS_Time[0];//q->M60_BS_Time[j - 1]; ?????????????????????????????????????????????�Ƿ���ȷ

							q->M60_BS_PURE_Value[1] = q->M1_BS_PURE_Value[1] + q->M1_BS_PURE_Value[2] + q->M1_BS_PURE_Value[3];// q->M60_BS_PURE_Value[j - 1];
							q->M60_BS_BUY_Value[1] = q->M1_BS_BUY_Value[1] + q->M1_BS_BUY_Value[2] + q->M1_BS_BUY_Value[3];// q->M60_BS_BUY_Value[j - 1];
							q->M60_BS_SELL_Value[1] = q->M1_BS_SELL_Value[1] + q->M1_BS_SELL_Value[2] + q->M1_BS_SELL_Value[3];//q->M60_BS_SELL_Value[j - 1];
							q->M60_BS_BL_Value[1] = q->M60_BS_BUY_Value[1] / q->M60_BS_SELL_Value[1];   //q->M60_BS_BL_Value[j - 1]; ?????????????????????????�Ƿ���ȷ

							q->M60_BS_BIG_PURE_Value[1] = q->M1_BS_BIG_PURE_Value[1] + q->M1_BS_BIG_PURE_Value[2] + q->M1_BS_BIG_PURE_Value[3];// q->M60_BS_BIG_PURE_Value[j - 1];
							q->M60_BS_BIG_BUY_Value[1] = q->M1_BS_BIG_BUY_Value[1] + q->M1_BS_BIG_BUY_Value[2] + q->M1_BS_BIG_BUY_Value[3];// q->M60_BS_BIG_BUY_Value[j - 1];
							q->M60_BS_BIG_SELL_Value[1] = q->M1_BS_BIG_SELL_Value[1] + q->M1_BS_BIG_SELL_Value[2] + q->M1_BS_BIG_SELL_Value[3];// q->M60_BS_BIG_SELL_Value[j - 1];
							q->M60_BS_BIG_BL_Value[1] = q->M60_BS_BIG_BUY_Value[1] / q->M60_BS_BIG_SELL_Value[1];// q->M60_BS_BIG_BL_Value[j - 1];
																												 //��M1���ӵĴ�����
																												 // int id = i;
																												 //  printf("\n0000000000000000000000000000000[%0.06f][%s][%d]q->M60_BS_Time[%0.02f] q->M60_close[%0.02f] q->M60_BS_PURE_Value[%0.02f] q->M60_BS_BUY_Value[%0.02f] q->M60_BS_SELL_Value[%0.02f] \n",
																												 //  tick_data[2], InstrumentID_n[id], i, q->M60_BS_Time[id][1], q->M60_close[id][1], q->M60_BS_PURE_Value[id][1], q->M60_BS_BUY_Value[id][1], q->M60_BS_SELL_Value[id][1]);
						}
					}
					else
					{
						//��������/���ڷ�����Ϊ0��Ϊ��ʼ����
						if ((tempm >= 60 || (tempm<0 && tempm >= -1380)) && CheckFirst(q->M1_BS_Time[1], 60))
						{
							NotFirst_60M = true;
						}
					}
					//M60����		

				}






				//����1M���������,���鿪һ�����߳�
				//if (tempdata == true)
				//{
				//�ָ�printf("\n���������\n");
				//for (int k = 0;k<TYPE_NUM;k++)
				//	{
				//	CalRelativity(i, k);
				//	}

				//}

				//����1M��������ԣ����鿪һ�����߳�    

				//����Ƿ���Ųλ֮ǰ����

				
				//printf("M1 pDepthMarketData->LastPrice[%0.02f][%0.02f][%0.02f][%0.02f]\n",q->M1_open[0],q->M1_high[0],q->M1_low[0], q->M1_close[0]);
				q->M1_open[0] = pDepthMarketData->LastPrice;
				q->M1_high[0] = pDepthMarketData->LastPrice;
				q->M1_low[0] = pDepthMarketData->LastPrice;
				q->M1_close[0] = pDepthMarketData->LastPrice;	//�Ƿ��ǰ��ÿ��TICK��=0�ظ���	���Ƿ����ɾ��ǰ��	=0��ֵ
																


																//TOTAL����ֻ�ۼƣ��������0


				q->M1_BS_Time[0] = q->tick_BS_Time[0];
				//ÿ����֮����1��

				for (int j = 0; j < MAX_PURE_TICK_PERMIN_NUM; j++)
				{


					//printf("A3\n");
					//double temptime=tick_BS_Time[j];
					//double temptime2=tick_BS_Time[j+1];
					//��������
					//cout.setf(ios::fixed);
					// cout.setf(ios::showpoint);
					//cout.precision(4);

					//cout<<temptime;
					//cout<<temptime2;
					//��������
					//if(tick_BS_Time[j]>0)
					////printf("ʱ�䰡B%d[%.06f][%.06f]\n",j,tick_BS_Time[j], ((double)(tick_BS_Time[j])).precision(2));
					//printf("ʱ�䰡XXXX______________   %d[%.06f]_[%.06f]\n",j,tick_BS_Time[j],temptime);

					//if(temptime-temptime2>=0.0001 && (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //һ����ʱ����������Ҫ�ų�00:00��
					// if(  (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //һ����ʱ����������Ҫ�ų�00:00��
					// {

					q->M1_BS_PURE_Value[0] = q->M1_BS_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_BUY_Value[0] = q->M1_BS_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//�߼��Ƿ�������
					q->M1_BS_SELL_Value[0] = q->M1_BS_SELL_Value[0] + q->tick_BS_SELL_Value[j];

					//bl,middle�����
					q->M1_BS_TOTAL_PURE_Value[0] = q->M1_BS_TOTAL_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_TOTAL_BUY_Value[0] = q->M1_BS_TOTAL_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//�߼��Ƿ�������
					q->M1_BS_TOTAL_SELL_Value[0] = q->M1_BS_TOTAL_SELL_Value[0] + q->tick_BS_SELL_Value[j];

					q->M1_BS_CROSS_TOTAL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_PURE_Value[0] + q->tick_BS_PURE_Value[j];
					q->M1_BS_CROSS_TOTAL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BUY_Value[0] + q->tick_BS_BUY_Value[j];			//�߼��Ƿ�������
					q->M1_BS_CROSS_TOTAL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SELL_Value[0] + q->tick_BS_SELL_Value[j];


					//TOTAL ����CROSS?
					//С��





					double tb = q->tick_BS_BUY_Value[j];
					double ts = q->tick_BS_SELL_Value[j];


					if (tb < CAL_MIDDLE_AMOUNT)
					{
						q->M1_BS_SMALL_BUY_Value[0] = q->M1_BS_SMALL_BUY_Value[0] + tb;
						q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_TOTAL_SMALL_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] + tb;  //���


						q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] + tb;  //���
					}

					//�е�
					else if (tb >= CAL_MIDDLE_AMOUNT && tb < CAL_BIG_AMOUNT)
					{
						// printf("[%d]�е� %0.02f\n", j, tb);
						q->M1_BS_MIDDLE_BUY_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //���

						q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //���
					}

					else// if (tb >= CAL_BIG_AMOUNT)
					{
						//printf("[%d]�� %0.02f\n", j, tb);
						q->M1_BS_BIG_BUY_Value[0] = q->M1_BS_BIG_BUY_Value[0] + tb;
						q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] + tb;

						q->M1_BS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_TOTAL_BIG_BUY_Value[0] + tb;
						q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] + tb;  //���

						q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] + tb;
						q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] + tb;  //���
					}



					if (ts < CAL_MIDDLE_AMOUNT)
					{
						q->M1_BS_SMALL_SELL_Value[0] = q->M1_BS_SMALL_SELL_Value[0] + ts;
						q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_TOTAL_SMALL_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] - ts;  //���

						q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] - ts;  //���
					}
					else if (ts >= CAL_MIDDLE_AMOUNT && ts < CAL_BIG_AMOUNT)
					{
						q->M1_BS_MIDDLE_SELL_Value[0] = q->M1_BS_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //���

						q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //���
					}
					else// if (ts >= CAL_BIG_AMOUNT)
					{
						q->M1_BS_BIG_SELL_Value[0] = q->M1_BS_BIG_SELL_Value[0] + ts;
						q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] - ts;

						q->M1_BS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_TOTAL_BIG_SELL_Value[0] + ts;
						q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] - ts;  //���

						q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_SELL_Value[0] + ts;
						q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] - ts;  //���
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
					//printf("��ǰʱ�䣺[%.06f]\n", tick_data[2]);
					//printf("\n__%dʱ�䣺[%.06f][%d][%s]tb[%0.02f]ts[%0.02f]����[%0.02f]��[%0.02f]��[%0.02f]����[%0.02f]С��[%0.02f]С��[%0.02f]С��[%0.02f]С������[%0.02f]�о�[%0.02f]����[%0.02f]����[%0.02f]�е�����[%0.02f]��[%0.02f]����[%0.02f]����[%0.02f]�󵥱���[%0.02f]\n",


					//}
					//printf("------------------\n");




				}






				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3����


				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3����

				//printf("A6\n");



				//��ӡK��������
				//	 cerr << "--->>> K:" << InstrumentID_n << "_" <<dbtoch(tick_data[1])<< "_" << tick_data[2]<< "_" << q->M1_open[1]<< "_" << q->M1_high[1]<< "_" << q->M1_low[1]<< "_" << q->M1_close[1] << endl;
			}
			else
			{
				//printf("A7\n");
				// printf("\n\n\n_______________________xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx__________________________kaishi \n\n\n");
				//�������һ��K�ߣ��Լ�������
				if (q->M1_open[0] == 0) { q->M1_open[0] = pDepthMarketData->LastPrice; }

				q->M1_high[0] = max(q->M1_high[0], pDepthMarketData->LastPrice);
				q->M1_low[0] = zero_min(q->M1_low[0], pDepthMarketData->LastPrice);
				q->M1_close[0] = pDepthMarketData->LastPrice;


				
				//q->M1_BS_PURE_Value[1]=q->M1_BS_PURE_Value[0];
				//q->M1_BS_PURE_Value[0]=NewVolume;

				//q->M1_BS_PURE_Value[0]=tick_BS_PURE_Value[0]+tick_BS_PURE_Value[1]+tick_BS_PURE_Value[2];
				//q->M1_BS_BUY_Value[0]=tick_BS_BUY_Value[0]+tick_BS_BUY_Value[1]+tick_BS_BUY_Value[2];
				//q->M1_BS_SELL_Value[0]=tick_BS_SELL_Value[0]+tick_BS_SELL_Value[1]+tick_BS_SELL_Value[2];

				//TICK���˺�ÿ��1��TICK��1����Ӧ�ø�Ϊ60��TICK��ÿ��2��TICK��Ӧ��λ120��
				//q->M1_BS_PURE_Value[0]=0;
				//q->M1_BS_BUY_Value[0]=0;
				//q->M1_BS_SELL_Value[0]=0;
				//printf("BBBBB \n");
				// printf("��һ����\n");
				//for(int j=0;j<MAX_PURE_TICK_PERMIN_NUM;j++)
				//{



				// cout<<temptime;
				//cout<<temptime2;
				//��������
				//if(tick_BS_Time[j]>0)
				////printf("ʱ�䰡B%d[%.06f][%.06f]\n",j,tick_BS_Time[j], ((double)(tick_BS_Time[j])).precision(2));
				//printf("ʱ�䰡B   %d[%.06f]_[%.06f]\n",j,tick_BS_Time[j],temptime);
				//printf("CCCCC \n");

				//  if(temptime-temptime2>=0.0001 && (tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //һ����ʱ����������Ҫ�ų�00:00��
				// if((tick_BS_Time[j+1]!=0 || tick_BS_Time[j+3]!=0)) //һ����ʱ����������Ҫ�ų�00:00��

				//if(tick_BS_Time[j]>0)
				// printf("ʱ���[%.06f][%.06f]_[%.06f]\n",tick_BS_Time[j],tick_BS_Time[j+1],temptime-temptime2);
				//if(tick_BS_Time[j]>0)
				//printf("ʱ���TICK[%.06f]M1[%.06f]_M2[%.06f]\n",tick_BS_Time[j],tempq->M1_A,tempq->M1_B);

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
					q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] + tb;  //���

					q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] + tb;  //���
				}

				//�е�
				else if (tb >= CAL_MIDDLE_AMOUNT && tb < CAL_BIG_AMOUNT)
				{
					// printf("[%d]�е� %0.02f\n", j, tb);
					q->M1_BS_MIDDLE_BUY_Value[0] = q->M1_BS_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_TOTAL_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //���

					q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value[0] + tb;  //���
				}

				else// if (tb >= CAL_BIG_AMOUNT)
				{
					//printf("[%d]�� %0.02f\n", j, tb);
					q->M1_BS_BIG_BUY_Value[0] = q->M1_BS_BIG_BUY_Value[0] + tb;
					q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] + tb;

					q->M1_BS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_TOTAL_BIG_BUY_Value[0] + tb;
					q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] + tb;  //���

					q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_BUY_Value[0] + tb;
					q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_BIG_PURE_Value[0] + tb;  //���
				}

				if (ts < CAL_MIDDLE_AMOUNT)
				{
					q->M1_BS_SMALL_SELL_Value[0] = q->M1_BS_SMALL_SELL_Value[0] + ts;
					q->M1_BS_SMALL_PURE_Value[0] = q->M1_BS_SMALL_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_TOTAL_SMALL_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_TOTAL_SMALL_PURE_Value[0] - ts;  //���

					q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_SELL_Value[0] + ts;
					q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] = q->M1_BS_CROSS_TOTAL_SMALL_PURE_Value[0] - ts;  //���
				}
				else if (ts >= CAL_MIDDLE_AMOUNT && ts < CAL_BIG_AMOUNT)
				{
					q->M1_BS_MIDDLE_SELL_Value[0] = q->M1_BS_MIDDLE_SELL_Value[0] + ts;
					q->M1_BS_MIDDLE_PURE_Value[0] = q->M1_BS_MIDDLE_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] = q->M1_BS_TOTAL_MIDDLE_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] = q->M1_BS_TOTAL_MIDDLE_PURE_Value[0] - ts;  //���
				}
				else// if (ts >= CAL_BIG_AMOUNT)
				{
					q->M1_BS_BIG_SELL_Value[0] = q->M1_BS_BIG_SELL_Value[0] + ts;
					q->M1_BS_BIG_PURE_Value[0] = q->M1_BS_BIG_PURE_Value[0] - ts;

					q->M1_BS_TOTAL_BIG_SELL_Value[0] = q->M1_BS_TOTAL_BIG_SELL_Value[0] + ts;
					q->M1_BS_TOTAL_BIG_PURE_Value[0] = q->M1_BS_TOTAL_BIG_PURE_Value[0] - ts;  //���
				}

				///////////////////////////////////////////////////////////////////////////////////////////////////////��ʱ
				//�ޱ�Ҫ����ɾ��

				//�ޱ�Ҫ


				////////////////////////////////////////////////////////////////////////////////////////////////////////////��ʱ







				//printf("1���Ӿ���B[%d]����[%d]����[%d]���������[%.02f]\n",(int)M1_BS_PURE_Value[0],(int)M1_BS_BUY_Value[0],(int)M1_BS_SELL_Value[0],MMBL);
				//M1_BS_PURE_Value[0]=Plus(M1_BS_PURE_Value[0],tick_BS_PURE_Value[0]);
				//M1_BS_BL_Value[0]=Ave(M1_BS_BL_Value[0],M1_BS_BL_Value[1],M1_BS_BL_Value[2],M1_BS_BL_Value[3],M1_BS_BL_Value[4],M1_BS_BL_Value[5],M1_BS_BL_Value[6],M1_BS_BL_Value[7],M1_BS_BL_Value[8],M1_BS_BL_Value[9]);
				




				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3����

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////M3����
				if (q->TickFileWritepaths[0] == '\0')
				{
					q->tick_data[8] = pDepthMarketData->UpperLimitPrice;
					q->tick_data[9] = pDepthMarketData->LowerLimitPrice;

					strcpy(q->TickFileWritepaths, "./TickData/");
					strcat(q->TickFileWritepaths, pDepthMarketData->InstrumentID);
					strcat(q->TickFileWritepaths, "_");
					//�ָ�strcat_s(q->TickFileWritepaths, TradingDay);
					strcat(q->TickFileWritepaths, ".txt");

					//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
					ifstream inf;
					ofstream ouf;
					inf.open(q->TickFileWritepaths, ios::out);


					/////////////////////////////////////////////////
					//char str[200] = { 0 };
					//strcat_s(str, 200, "SingleData/TradeRecord.txt");

					//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
					//	ifstream inf;
					//ofstream ouf;
					//inf.open(TickFileWritepaths[i], ios::out);
					//inf.open(str, ios::out);
					//}

					//��¼TICK����
					//ofstream o_file(str, ios::app);
					//if ( RunMode && ( check0 || check1 || check2) )

					//printf("xxxxxxxxxxxxxx%.06f\n",dbtoch(tick_data[id][1]));

					//o_file << content << "\t" << endl;

					//else
					//{
					//o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //������д�뵽�ı��ļ���
					//}
					//o_file.close();						//�ر��ļ�




				}

				//��¼TICK����
				ofstream o_file(q->TickFileWritepaths, ios::app);

				if (check1 || check2 || check3 || check4 || check5 || check6 || check7)
				{
					o_file << dbtoch(q->tick_data[1]) << "\t" << q->tick_data[2] << "\t" << pDepthMarketData->UpdateMillisec << "\t" << q->tick_AskPrice1[0] << "\t" << q->tick_AskVolume1[0] << "\t" << q->tick_BidPrice1[0] << "\t" << q->tick_BidVolume1[0] << "\t" << q->tick_data[4] << "\t" << dbtoch(q->tick_Volume[0]) << "\t" << dbtoch(q->tick_OpenInterest[0]) << "\t" << q->Day_open[2] << "\t" << q->Day_high[2] << "\t" << q->Day_low[2] << "\t" << q->Day_close[2] << "\t" << q->Day_open[1] << "\t" << q->Day_high[1] << "\t" << q->Day_low[1] << "\t" << q->Day_close[1] << "\t" << q->tick_data[8] << "\t" << q->tick_data[9] << endl;
				}
				else
				{
					o_file << dbtoch(q->tick_data[1]) << "\t" << q->tick_data[2] << "\t" << pDepthMarketData->UpdateMillisec << "\t" << q->tick_AskPrice1[0] << "\t" << q->tick_AskVolume1[0] << "\t" << q->tick_BidPrice1[0] << "\t" << q->tick_BidVolume1[0] << "\t" << q->tick_data[4] << "\t" << dbtoch(q->tick_Volume[0]) << "\t" << dbtoch(q->tick_OpenInterest[0]) << endl; //������д�뵽�ı��ļ���
				}
				o_file.close();						//�ر��ļ�


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
				//��ҹ��
				bool FistCheck2 = (!FistCheck1) && (!typecheck1) && (!typecheck2);
				//��ҹ��
				//��if ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i<=17))
				//    if ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i <= 17) || (tick_data[i][2]<0.0230 && i <= 17))  //�޸�9

				if (seconds>45 && seconds<55 && q->M1Klinesig == true)
				{
					//printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxת��xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n\n");
					q->M1Klinesig = false;
				}



				//�������                                                                                //��ҹ��                                               //��ҹ��
				if ((q->tick_data[2]>0.0858 && typecheck1) || (q->tick_data[2]>0.0925 && typecheck2) || (q->tick_data[2]>0.0858 && FistCheck1) || ((q->tick_data[2]>0.2058 || q->tick_data[2]<0.1530) && FistCheck2))  //�޸�9
				{
					q->FristTick = true;
				}

				q->ReceiveTick = false;
				q->tick_data[0] = 1;		//���ñ�־λ

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
    ///�û���¼����
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
		///��ȡ��ǰ������
		cerr << "TradeingDay: " << mpUserApi->GetTradingDay() << endl;
		mInitOK = TRUE;

 
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
			printf("��¼ʧ��,ErrorID=0x%04x, ErrMsg=%s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);

			CThostFtdcRspUserLoginField tn;
			memset(&tn, 0, sizeof(CThostFtdcRspUserLoginField));
			memcpy_s(&tn, sizeof(CThostFtdcRspUserLoginField), pRspUserLogin, sizeof(CThostFtdcRspUserLoginField));

	}
	else
	{
		if (*OnRspUserLoginCallback)
			OnRspUserLoginCallback();


			printf("��¼�ɹ�\n");
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

	//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
	ifstream inf;
	ofstream ouf;
	//inf.open(TickFileWritepaths[i], ios::out);
	inf.open(str, ios::out);
 

	//��¼TICK����
	ofstream o_file(str, ios::app);
 

	o_file << ticktime<<","<<price << "," << endl;

 
	o_file.close();						//�ر��ļ�

}


void CMdSpi::WirteTick_Detail(const char * InstrumentID,const CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	char str[200] = { 0 };
	strcat_s(str, 200, "TickData_Detail\\TickData_");
	strcat_s(str, 200, InstrumentID);
	strcat_s(str, 200, ".csv");

	//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
	ifstream inf;
	ofstream ouf;
	inf.open(str, ios::out);
	//}
	if (!inf)
	{
		cout << "�ļ����ܴ�" << endl;
		ofstream o_file(str, ios::app);

		o_file << "������" << ","
			<< "��Լ" << ","
			<< "ʱ��" << ","
			<< "����" << ","
			<< "�ּ�" << ","
			<< "�ֲ�" << ","  //�ֲ���
			<< "�ɽ���" << ","
			<< "��һ" << ","
			//<< "���"<< ","
			//<< "����" << ","
			//<< "����" << ","
			//<< "����" << ","
			<< "��һ��" << ","
			//<< "�����" << ","
			//<< "������" << ","
			//<< "������" << ","
			//<< "������" << ","
			<< "��һ" << ","
			//<< "����" << ","
			//<< "����" << ","
			//<< "����" << ","
			//<< "����" << ","
			<< "��һ��" << ","
		    //<< "������" << ","
			//<< "������" << ","
			//<< "������" << ","
			//<< "������" << ","
			<< "��ͣ��" << ","
			<< "��ͣ��" << endl;
	}
	else
	{
		ofstream o_file(str, ios::app);
	
	
	}
	//��¼TICK����
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
		<< pDepthMarketData->OpenInterest << ","  //�ֲ���
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

 
 
	o_file.close();						//�ر��ļ�

}
*/

//PYTHON
/*
#include "include_Python/Python.h"
void strategy2(const char * filename)
{

	Py_Initialize();	          //ʹ��python֮ǰ��Ҫ����Py_Initialize();����������г�ʼ��
	PyObject * pModule = NULL;	  //��������
	PyObject * pFunc = NULL;	  //��������


	pModule = PyImport_ImportModule(filename);	            //������Ҫ���õ�Python�ļ���
	//char tempstr[300] = { 0 };
	//_snprintf_s(tempstr,sizeof(tempstr),sizeof(tempstr),"run %s", filename);
	pFunc = PyObject_GetAttrString(pModule, "main_strategy");	//������Ҫ���õĺ�����
	PyEval_CallObject(pFunc, NULL);	                        //���ú���,NULL��ʾ����Ϊ��
															//Py_Finalize();	              //����Py_Finalize,�����Py_Initialize���Ӧ��.
															//���û����

}


void strategy3(const char * filename,const char * instrument, double price)
{
	Py_Initialize();

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	pModule = PyImport_ImportModule(filename);	    //Test001:Python�ļ���
	pFunc = PyObject_GetAttrString(pModule, "main_strategy");	//Add:Python�ļ��еĺ�����
													//��������:
	PyObject *pArgs = PyTuple_New(2);               //�������õĲ������ݾ�����Ԫ�����ʽ�����,2��ʾ��������
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 5));//0---���  i��ʾ����int�ͱ���
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 5));//1---���
	//PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", price));//1---���
													 //����ֵ
	PyObject *pReturn = NULL;
	pReturn = PyEval_CallObject(pFunc, pArgs);	    //���ú���
													//������ֵת��Ϊint����
	int result;
	PyArg_Parse(pReturn, "i", &result);    //i��ʾת����int�ͱ���
	//cout << "5+7 = " << result << endl;
	if (result==1)
	{
		printf("%s������[%s]�����ź�", instrument, filename);
	
	}
	//Py_Finalize();
}
*/


	/*

void CMdSpi::strategy()
{

	
	
	
	//strategy1();




	Py_Initialize();	          //ʹ��python֮ǰ��Ҫ����Py_Initialize();����������г�ʼ��
	PyObject * pModule = NULL;	  //��������
	PyObject * pFunc = NULL;	  //��������


	pModule = PyImport_ImportModule("Test001");	            //������Ҫ���õ�Python�ļ���
	pFunc = PyObject_GetAttrString(pModule, "HelloWorld");	//������Ҫ���õĺ�����
	PyEval_CallObject(pFunc, NULL);	                        //���ú���,NULL��ʾ����Ϊ��
															//Py_Finalize();	              //����Py_Finalize,�����Py_Initialize���Ӧ��.
															//���û����
	//TestTransferDict();

	
}*/
//�������ݵ�����Ϊ�ֵ�
/*
void TestTransferDict()
{
	Py_Initialize();

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	pModule = PyImport_ImportModule("Test001");	    //Test001:Python�ļ���
	pFunc = PyObject_GetAttrString(pModule, "TestDict");	//Add:Python�ļ��еĺ�����
															//��������:
	PyObject *pArgs = PyTuple_New(1);
	PyObject *pDict = PyDict_New();   //�����ֵ����ͱ���
	PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "WangYao")); //���ֵ����ͱ������������
	PyDict_SetItemString(pDict, "Age", Py_BuildValue("i", 25));         //���ֵ����ͱ������������
	PyTuple_SetItem(pArgs, 0, pDict);//0---���  ���ֵ����ͱ�����ӵ�����Ԫ����
									 //����ֵ
	PyObject *pReturn = NULL;
	pReturn = PyEval_CallObject(pFunc, pArgs);	    //���ú���
													//������ֵ:
	int size = PyDict_Size(pReturn);
	cout << "�����ֵ�Ĵ�СΪ: " << size << endl;
	PyObject *pNewAge = PyDict_GetItemString(pReturn, "Age");
	int newAge;
	PyArg_Parse(pNewAge, "i", &newAge);
	cout << "True Age: " << newAge << endl;

	Py_Finalize();
}
*/
bool firststate = true;


//����
HANDLE hThread[12];
unsigned int uiThreadId[12];
//����
/*
DWORD WINAPI strategy(const LPVOID lpParam)
//UINT strategy(LPVOID pParam)
// void CMdSpi::strategy()
{
	Py_Initialize();

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	pModule = PyImport_ImportModule("Test001");	    //Test001:Python�ļ���
	pFunc = PyObject_GetAttrString(pModule, "TestDict");	//Add:Python�ļ��еĺ�����
															//��������:
	PyObject *pArgs = PyTuple_New(1);
	PyObject *pDict = PyDict_New();   //�����ֵ����ͱ���
	PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "WangYao")); //���ֵ����ͱ������������
	PyDict_SetItemString(pDict, "Age", Py_BuildValue("i", 25));         //���ֵ����ͱ������������
	PyTuple_SetItem(pArgs, 0, pDict);//0---���  ���ֵ����ͱ�����ӵ�����Ԫ����
									 //����ֵ
	PyObject *pReturn = NULL;
	pReturn = PyEval_CallObject(pFunc, pArgs);	    //���ú���
													//������ֵ:
	Py_ssize_t size = PyDict_Size(pReturn);
	cout << "�����ֵ�Ĵ�СΪ: " << size << endl;
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
		idrand* ir = (idrand*)lpParam;//��ȡ�����ṹ��
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
			Trading(ir->beginid, ir->endid);	//��ͻ

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
	system_times = (double)((sys_time.wHour) / 10e1) + (double)((sys_time.wMinute) / 10e3) + (double)((sys_time.wSecond) / 10e5);	//��ʽʱ��0.145100
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
	//����ʱ�����
	double Nowtime = GetLocalTimeSec2();
	if (begintime1 > -0.1 && endtime1 > -0.1)
	{
		if (Nowtime<begintime1 && Nowtime>endtime1)
	  {
		  
		printf("���ڽ���ʱ���ڣ��������ݱ��ܾ�[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
		return;
	  }
	
	}
	if (begintime2 > -0.1 && endtime2 > -0.1)
	{
		if (Nowtime > begintime2 && Nowtime < endtime2)
		{
			printf("���ڽ���ʱ���ڣ��������ݱ��ܾ�[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			return;
		}
	}
	if (begintime3 > -0.1 && endtime3 > -0.1)
	{
		if (Nowtime > begintime3 && Nowtime < endtime3)
		{
			printf("���ڽ���ʱ���ڣ��������ݱ��ܾ�[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			return;
		}
	}
	if (begintime4 > -0.1 && endtime4 > -0.1)
	{
		if (Nowtime > begintime4 && Nowtime < endtime4)
		{
			printf("���ڽ���ʱ���ڣ��������ݱ��ܾ�[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			return;
		}
	}
	//����ʱ�����
	if (gMarket.find(pDepthMarketData->InstrumentID) != gMarket.end())
	{
		memcpy(depthdata[gMarket[pDepthMarketData->InstrumentID]], pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		UpdatePriceHash((char*)(&pDepthMarketData->InstrumentID), pDepthMarketData);

		/*
		printf("savetickstate:[%d]\n", savetickstate);
		if (savetickstate==2)
		{   
			printf("��������(��ϸ)��...\r");
			WirteTick_Detail((char*)pDepthMarketData->InstrumentID, pDepthMarketData);
		}
		else if (savetickstate == 1)
		{
			printf("��������(����)��...\r");
			WirteTick_Simple((char*)pDepthMarketData->InstrumentID, (char*)pDepthMarketData->UpdateTime, (double)pDepthMarketData->LastPrice);
		}
		*/

		//TThostFtdcInstrumentIDTypeStruct tn;
		//memset(&tn, 0, sizeof(TThostFtdcInstrumentIDTypeStruct));

		//strncpy_s(tn.Instrument, sizeof(tn.Instrument), pDepthMarketData->InstrumentID, sizeof(pDepthMarketData->InstrumentID));

		//EnterCriticalSection(&g_csdata);
		//ticknamelist.push_back(tn);
		//LeaveCriticalSection(&g_csdata);
		//ʱ������
		//SetEvent(hEvent[EID_OnRtnDepthMarketData]);
		//return;
		//�������ʱ���Σ��Զ����
		/*
       //�������ڼ۸�����
		string instrumentstr = pDepthMarketData->InstrumentID;
		QS_Strategy_Map::iterator it = mapStrategy.find(instrumentstr);
		if (it == mapStrategy.end())
		{
				printf("û�ҵ��ú�Լ�Ĳ���%s\n", pDepthMarketData->InstrumentID); 
		}
		else
		{
			for (int k = 0; k < it->second.strategyfilenum; k++)
			{
				if (true) 
				{ 
					printf("����[%s]�Ĳ����ļ���[%s] ��λ[%d]\n", pDepthMarketData->InstrumentID, (it->second.strategyfile[k]).c_str(), it->second.position[k]);
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


///��������Ӧ��
void  CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (*OnRspSubMarketDataCallback)
		OnRspSubMarketDataCallback(0);

};

///ȡ����������Ӧ��
void  CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{ 
	if (*OnRspUnSubMarketDataCallback)
		OnRspUnSubMarketDataCallback();
};

///ѯ��֪ͨ
void  CMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{

};

///�ǳ�������Ӧ
void CMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (*OnRspUserLogoutCallback)
		OnRspUserLogoutCallback();
}