

#pragma once

//���ڶ���
#define QL_ALL   10000  	// ��������
#define QL_M1    10001    	// M1   1����
#define QL_M3    10002  	    // M3   3����
#define QL_M5    10003  	    // M5   5����
#define QL_M10   10004  	// M10  10����
#define QL_M15   10005  	// M15  15����
#define QL_M30   10006  	// M30  30����
#define QL_M60   10007  	// M60  60����
#define QL_M120  10008  	// M120 120����
#define QL_M240  10009  	// M120 120����
#define QL_D1    10010  	    // D1   1��



// PRICE TYPE
#define QL_CLOSE  10010   // ���̼�
#define QL_OPEN   10011   // ���̼�
#define QL_HIGH   10012   // ��߼�
#define QL_LOW    10013   // ��ͼ�


// PRICE TYPE
#define QL_MA     10014   // ���̼�
#define QL_RSI    10015   // ���̼�
#define QL_SAR    10016   // ��߼�
#define QL_MACD   10017   // ��ͼ�
#define QL_CCI    10018   // ��ͼ�
#define QL_ATI    10019   // ��ͼ�



#define  MAX_TICK_NUM   60//120//60  //�������ݣ�֮ȡ��һ���±�λ0��Ԫ�أ���������ν
#define  MAX_TICK_MOVE_NUM   59


#define  MAX_PURE_TICK_PERMIN_NUM   120 // 240//120  //ÿ����TICK��60X2  //TICK������120��δ���ͳ�ƣ�



/*

#define  max_pure_tick_num   130  //����120TICK�������� ������
#define  max_pure_tick_move_num   129  //����120TICK�������� ������

#define  max_pure_m1_num          480  //ÿ1���� 120TICK
#define  max_pure_m1_move_num     479  //ÿ1���� 120TICK

#define  max_pure_m3_num          361  //160  //ÿ3���� 360TICK
#define  max_pure_m3_move_num     360  //159  //ÿ3���� 360TICK

#define  max_pure_m5_num          96  //ÿ3���� 360TICK
#define  max_pure_m5_move_num     95 //ÿ3���� 360TICK

#define  max_pure_m10_num          96  //ÿ3���� 360TICK
#define  max_pure_m10_move_num     95 //ÿ3���� 360TICK

#define  max_pure_m15_num          96  //ÿ3���� 360TICK
#define  max_pure_m15_move_num     95 //ÿ3���� 360TICK

#define  max_pure_m30_num          96  //ÿ3���� 360TICK
#define  max_pure_m30_move_num     95 //ÿ3���� 360TICK

#define  max_pure_m60_num          96  //ÿ3���� 360TICK
#define  max_pure_m60_move_num     95 //ÿ3���� 360TICK

#define  max_pure_m120_num          96  //ÿ3���� 360TICK
#define  max_pure_m120_move_num     95 //ÿ3���� 360TICK

#define  max_pure_m240_num          96  //ÿ3���� 360TICK
#define  max_pure_m240_move_num     95 //ÿ3���� 360TICK

#define  max_pure_d1_num          30  //ÿ3���� 360TICK
#define  max_pure_d1_move_num     29 //ÿ3���� 360TICK

#define  max_pure_m480_num        100  //��ÿ��8Сʱ����Ϊ480����
#define  max_pure_m480_move_num   99  //��ÿ��8Сʱ����Ϊ480����
*/


extern int max_pure_tick_num;// = 130;  //����120TICK�������� ������
extern int max_pure_tick_move_num;// = 129; //����120TICK�������� ������

extern int max_pure_m1_num;//= 480;  //ÿ1���� 120TICK
extern int max_pure_m1_move_num;// = 479;  //ÿ1���� 120TICK

extern int max_pure_m3_num;// = 361;  //160  //ÿ3���� 360TICK
extern int max_pure_m3_move_num;// = 360;  //159  //ÿ3���� 360TICK

extern int max_pure_m5_num;// = 96;  //ÿ3���� 360TICK
extern int max_pure_m5_move_num;// = 95; //ÿ3���� 360TICK

extern int max_pure_m10_num;// = 96;  //ÿ3���� 360TICK
extern int max_pure_m10_move_num;// = 95; //ÿ3���� 360TICK

extern int  max_pure_m15_num;//= 96;  //ÿ3���� 360TICK
extern int  max_pure_m15_move_num;// = 95; //ÿ3���� 360TICK

extern int  max_pure_m30_num;// = 96;  //ÿ3���� 360TICK
extern int  max_pure_m30_move_num;// = 95; //ÿ3���� 360TICK

extern int  max_pure_m60_num;// = 96; //ÿ3���� 360TICK
extern int  max_pure_m60_move_num;// = 95; //ÿ3���� 360TICK

extern int  max_pure_m120_num;// = 96;  //ÿ3���� 360TICK
extern int  max_pure_m120_move_num;//= 95; //ÿ3���� 360TICK

extern int  max_pure_m240_num;//= 96;  //ÿ3���� 360TICK
extern int  max_pure_m240_move_num;// = 95;//ÿ3���� 360TICK

extern int  max_pure_d1_num;// = 30;  //ÿ3���� 360TICK
extern int  max_pure_d1_move_num;// = 29; //ÿ3���� 360TICK

extern int  max_pure_m480_num;// = 100;  //��ÿ��8Сʱ����Ϊ480����
extern int  max_pure_m480_move_num;// = 99;  //��ÿ��8Сʱ����Ϊ480����


//extern char	InstrumentID_n[TYPE_NUM][10];

#define MAX_USERNAME 33   //15
#define MAX_TRY_NUMBER 5  //5
#define MAX_ADDR_NUMBER 5
#define MAX_PACKET_SIZE 1024
#define SERVER_PORT 4567

#define  MAX_CROSSSINGLE_NUM   500
#define  MAX_CROSSSINGLE_MOVE_NUM   499
#define TYPE_NUM   20


#define TYPE_ARR_LEN 60  //Ӧ�õ�������Ʒ�ֺ������100000~1000059

#define  TYPE_NI   100000 //��λ���±꿪ʼ
#define  TYPE_ZN   100001 //��λ���±꿪ʼ
#define  TYPE_AL   100002 //��λ���±꿪ʼ
#define  TYPE_CU   100003 //��λ���±꿪ʼ
#define  TYPE_AU   100004 //��λ���±꿪ʼ
#define  TYPE_AG   100005 //��λ���±꿪ʼ
#define  TYPE_I    100006 //��λ���±꿪ʼ
#define  TYPE_RU   100007 //��λ���±꿪ʼ
#define  TYPE_TA   100008 //��λ���±꿪ʼ
#define  TYPE_A    100009 //��λ���±꿪ʼ
#define  TYPE_M    100010 //��λ���±꿪ʼ
#define  TYPE_Y    100011 //��λ���±꿪ʼ
#define  TYPE_P    100012 //��λ���±꿪ʼ
#define  TYPE_RB   100013 //��λ���±꿪ʼ
#define  TYPE_MA   100014 //��λ���±꿪ʼ
#define  TYPE_PP   100015 //��λ���±꿪ʼ
#define  TYPE_CS   100016 //��λ���±꿪ʼ
#define  TYPE_JD   100017 //��λ���±꿪ʼ
#define  TYPE_BU   100018 //��λ���±꿪ʼ
#define  TYPE_FG   100019 //��λ���±꿪ʼ
#define  TYPE_L    100020 //��λ���±꿪ʼ
#define  TYPE_V    100021 //��λ���±꿪ʼ
#define  TYPE_J    100022 //��λ���±꿪ʼ

#define  TYPE_SR   100023 //��λ���±꿪ʼ
#define  TYPE_RM   100024 //��λ���±꿪ʼ
#define  TYPE_CF   100025 //��λ���±꿪ʼ

#define  TYPE_C    100026 //��λ���±꿪ʼ
#define  TYPE_WH   100027 //��λ���±꿪ʼ
#define  TYPE_SM   100028 //��λ���±꿪ʼ
#define  TYPE_SF   100029 //��λ���±꿪ʼ


//���ӵ�
#define  TYPE_PB   100030 //��λ���±꿪ʼ
#define  TYPE_SN   100031 //��λ���±꿪ʼ
#define  TYPE_WR   100032 //��λ���±꿪ʼ
#define  TYPE_HC   100033 //��λ���±꿪ʼ
#define  TYPE_FU   100034 //��λ���±꿪ʼ
//���ӵ�



#define  TYPE_IC   100035 //��λ���±꿪ʼ
#define  TYPE_IF   100036 //��λ���±꿪ʼ
#define  TYPE_IH   100037 //��λ���±꿪ʼ

#define  TYPE_T    100038 //ʮ��ծ
#define  TYPE_TF   100039 //���ծ

#define  TYPE_NEW1    100040 
#define  TYPE_NEW2    100041
#define  TYPE_NEW3    100042
#define  TYPE_NEW4    100043
#define  TYPE_NEW5    100044
#define  TYPE_NEW6    100045
#define  TYPE_NEW7    100046
#define  TYPE_NEW8    100047
#define  TYPE_NEW9    100048
#define  TYPE_NEW10   100049
#define  TYPE_NEW11   100050
#define  TYPE_NEW12   100051 
#define  TYPE_NEW13   100052
#define  TYPE_NEW14   100053
#define  TYPE_NEW15   100054
#define  TYPE_NEW16   100055
#define  TYPE_NEW17   100056
#define  TYPE_NEW18   100057
#define  TYPE_NEW19   100058
#define  TYPE_NEW20   100059



#pragma pack(8)

//typedef
struct GuestOnlineHash                      //����ڵ�
{
	//char name[8], address[20], num[11];
	//unsigned long key;
	int key;
	//int M;
	//int N;
	char InstrumentID[10];
	int  TYPEID;
	char	TickFileWritepaths[20];
	int  tick_VolumeLast;
	bool period_M1;
	bool period_M3;
	bool period_M5;
	bool period_M10;
	bool period_M15;
	bool period_M30;
	bool period_M60;
	bool period_M120;
	bool period_M240;
	bool period_D1;


	bool	ReceiveTick;
	bool	FristTick;
	bool	LastTick;

	//double	tick_data[10];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:������Ϣ
 
	//bool	LastTick;

	int avespace;
	int tradenum;

	int sort;//space/ƽ��ʱ������

			 //DWORD dwLastActiveTime;			 // ��¼���û��Ļʱ�䣨������ʹ�ã�
			 //DWORD LoginErrorTime;
			 //int LoginErrorNum;
	char keystr[MAX_USERNAME];	 // �û���
								 //char checkcode[4];
								 //DWORD ip[30];  //6���ַ�+1λ\0
								 //u_short port[6];  //6���ַ�+1λ\0

	double nottime[5];
	double starttime[5];
	double endtime[5];

								 //CROSS��ͳ��
	double	*M1_BS_CROSS_TOTAL_BIG_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_BIG_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_BIG_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_BIG_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	double	*M1_BS_CROSS_TOTAL_MIDDLE_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_MIDDLE_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_MIDDLE_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_MIDDLE_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	double	*M1_BS_CROSS_TOTAL_SMALL_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_SMALL_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_SMALL_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_SMALL_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


	double	*M1_BS_CROSS_TOTAL_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_CROSS_TOTAL_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
														//CROSS��ͳ��

	 double	tick_data[10];				//TICK��������
	 double	tick_AskPrice1[MAX_TICK_NUM];			//��һ��
	 double	tick_BidPrice1[MAX_TICK_NUM];			//��һ��
	 double	tick_AskVolume1[MAX_TICK_NUM];		//��һ��
	 double	tick_BidVolume1[MAX_TICK_NUM];		//��һ��
	 double	tick_Volume[MAX_TICK_NUM];			//�ɽ���
	 double	tick_OpenInterest[MAX_TICK_NUM];		//�ֲ���



	 double	*tick_BS_BIG_BUY_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	 double	*tick_BS_BIG_SELL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	 double	*tick_BS_BIG_PURE_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������

	 double	*tick_BS_BUY_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	 double	*tick_BS_SELL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	 double	*tick_BS_PURE_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������


													// double	tick_BS_BUY_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
													// double	tick_BS_SELL_Time[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*tick_BS_Time;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������

	double	*tick_BS_BL_Value;// = new double[max_pure_tick_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M1_BS_TOTAL_BIG_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_BIG_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_BIG_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_BIG_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	double	*M1_BS_TOTAL_MIDDLE_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_MIDDLE_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_MIDDLE_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_MIDDLE_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	double	*M1_BS_TOTAL_SMALL_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_SMALL_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_SMALL_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_SMALL_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


	double	*M1_BS_TOTAL_PURE_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_SELL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_BUY_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*M1_BS_TOTAL_BL_Value;// = new double[max_pure_m1_num];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	double  *M1_open;// = new double[max_pure_m1_num];	//����K�߿�
	double  *M1_high;// = new double[max_pure_m1_num];	//����K�߸�
	double  *M1_low;// = new double[max_pure_m1_num];		//����K�ߵ�
	double  *M1_close;// = new double[max_pure_m1_num];	//����K����
	bool	M1Klinesig;		//1����K�߱�־
	double	*M1_BS_Time;// = new double[max_pure_m1_num];


	double	*M1_BS_BIG_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_BIG_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_BIG_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_BIG_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M1_BS_MIDDLE_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_MIDDLE_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_MIDDLE_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_MIDDLE_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M1_BS_SMALL_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_SMALL_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_SMALL_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_SMALL_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M1_BS_PURE_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_BUY_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_SELL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M1_BS_BL_Value;// = new double[max_pure_m1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������




												//3����
	double  *M3_open;// = new double[max_pure_m3_num];	//����K�߿�
	double  *M3_high;// = new double[max_pure_m3_num];	//����K�߸�
	double  *M3_low;// = new double[max_pure_m3_num];		//����K�ߵ�
	double  *M3_close;// = new double[max_pure_m3_num];	//����K����
										//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*M3_BS_Time;// = new double[max_pure_m3_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	double	*M3_BS_BIG_PURE_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M3_BS_BIG_BUY_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M3_BS_BIG_SELL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M3_BS_BIG_BL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M3_BS_BUY_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M3_BS_SELL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M3_BS_PURE_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M3_BS_BL_Value;// = new double[max_pure_m3_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
												//3����


												//5����
	double  *M5_open;// = new double[max_pure_m5_num];	//����K�߿�
	double  *M5_high;// = new double[max_pure_m5_num];	//����K�߸�
	double  *M5_low;// = new double[max_pure_m5_num];		//����K�ߵ�
	double  *M5_close;// = new double[max_pure_m5_num];	//����K����
										//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*M5_BS_Time;// = new double[max_pure_m5_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	double	*M5_BS_BIG_PURE_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M5_BS_BIG_BUY_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M5_BS_BIG_SELL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M5_BS_BIG_BL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M5_BS_BUY_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M5_BS_SELL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M5_BS_PURE_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M5_BS_BL_Value;// = new double[max_pure_m5_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
												//5����


												//10����
	double  *M10_open;// = new double[max_pure_m10_num];	//����K�߿�
	double  *M10_high;// = new double[max_pure_m10_num];	//����K�߸�
	double  *M10_low;// = new double[max_pure_m10_num];		//����K�ߵ�
	double  *M10_close;// = new double[max_pure_m10_num];	//����K����
											//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*M10_BS_Time;// = new double[max_pure_m10_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	double	*M10_BS_BIG_PURE_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M10_BS_BIG_BUY_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M10_BS_BIG_SELL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M10_BS_BIG_BL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M10_BS_BUY_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M10_BS_SELL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M10_BS_PURE_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M10_BS_BL_Value;// = new double[max_pure_m10_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
												//10����

												//15����
	double  *M15_open;// = new double[max_pure_m15_num];	//����K�߿�
	double  *M15_high;// = new double[max_pure_m15_num];	//����K�߸�
	double  *M15_low;// = new double[max_pure_m15_num];		//����K�ߵ�
	double  *M15_close;// = new double[max_pure_m15_num];	//����K����
											//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*M15_BS_Time;// = new double[max_pure_m15_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	double	*M15_BS_BIG_PURE_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M15_BS_BIG_BUY_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M15_BS_BIG_SELL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M15_BS_BIG_BL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M15_BS_BUY_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M15_BS_SELL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M15_BS_PURE_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M15_BS_BL_Value;// = new double[max_pure_m15_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
												//15����



												//30����
	double  *M30_open;// = new double[max_pure_m30_num];	//����K�߿�
	double  *M30_high;// = new double[max_pure_m30_num];	//����K�߸�
	double  *M30_low;// = new double[max_pure_m30_num];		//����K�ߵ�
	double  *M30_close;// = new double[max_pure_m30_num];	//����K����
											//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*M30_BS_Time;// = new double[max_pure_m30_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	double	*M30_BS_BIG_PURE_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M30_BS_BIG_BUY_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double  *M30_BS_BIG_SELL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M30_BS_BIG_BL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M30_BS_BUY_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M30_BS_SELL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M30_BS_PURE_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M30_BS_BL_Value;// = new double[max_pure_m30_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
												//30����


	//60����
	double  *M60_open;// = new double[max_pure_m60_num];	//����K�߿�
	double  *M60_high;// = new double[max_pure_m60_num];	//����K�߸�
	double  *M60_low;// = new double[max_pure_m60_num];		//����K�ߵ�
	double  *M60_close;// = new double[max_pure_m60_num];	//����K����
											//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*M60_BS_Time;// = new double[max_pure_m60_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������
	double	*M60_BS_BIG_PURE_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M60_BS_BIG_BUY_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M60_BS_BIG_SELL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M60_BS_BIG_BL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������


	double	*M60_BS_BUY_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M60_BS_SELL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M60_BS_PURE_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������
	double	*M60_BS_BL_Value;// = new double[max_pure_m60_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����,������������ѹ������								
	//60����


	//120����
	double  *M120_open;// = new double[max_pure_m120_num];	//����K�߿�
	double  *M120_high;// = new double[max_pure_m120_num];	//����K�߸�
	double  *M120_low;// = new double[max_pure_m120_num];		//����K�ߵ�
	double  *M120_close;// = new double[max_pure_m120_num];	//����K����
											//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*M120_BS_Time;// = new double[max_pure_m120_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	double	*M120_BS_BIG_PURE_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M120_BS_BIG_BUY_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M120_BS_BIG_SELL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M120_BS_BIG_BL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������


	double	*M120_BS_BUY_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M120_BS_SELL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M120_BS_PURE_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M120_BS_BL_Value;// = new double[max_pure_m120_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
	//120����

	//240����
	double  *M240_open;// = new double[max_pure_m240_num];	//����K�߿�
	double  *M240_high;// = new double[max_pure_m240_num];	//����K�߸�
	double  *M240_low;// = new double[max_pure_m240_num];		//����K�ߵ�
	double  *M240_close;// = new double[max_pure_m240_num];	//����K����
											//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*M240_BS_Time;// = new double[max_pure_m240_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	double	*M240_BS_BIG_PURE_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M240_BS_BIG_BUY_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M240_BS_BIG_SELL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M240_BS_BIG_BL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������


	double	*M240_BS_BUY_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M240_BS_SELL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M240_BS_PURE_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*M240_BS_BL_Value;// = new double[max_pure_m240_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
	 //240����


	 //240����
	double  *D1_open;// = new double[max_pure_d1_num];	//����K�߿�
	double  *D1_high;// = new double[max_pure_d1_num];	//����K�߸�
	double  *D1_low;// = new double[max_pure_d1_num];		//����K�ߵ�
	double  *D1_close;// = new double[max_pure_d1_num];	//����K����
											//	bool	M3Klinesig[20];		//3����K�߱�־
	double	*D1_BS_Time;// = new double[max_pure_d1_num];
	// double	M3_BS_BUY_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_SELL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_PURE_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	// double	M3_BS_BL_Value[20][MAX_TICK_NUM];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������
	double	*D1_BS_BIG_PURE_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*D1_BS_BIG_BUY_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*D1_BS_BIG_SELL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*D1_BS_BIG_BL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������


	double	*D1_BS_BUY_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*D1_BS_SELL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*D1_BS_PURE_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������
	double	*D1_BS_BL_Value;// = new double[max_pure_d1_num];	//	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:�����TICK����,������������ѹ������								
													//240����

	double  Day_open[60];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��K�����ݿ� 
	double  Day_high[60];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��K�����ݸ�
	double  Day_low[60];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��K�����ݵ�
	double  Day_close[60];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��K��������


	int EmaCount;
	//int si;     //���ҳ���
	//PEER_RECORD_INFO deallist;
	//ADDR_RADER_INFO addr[MAX_ADDR_NUMBER]; // �ɽڵ��˽���ն˺͹����ն���ɵ�����
	//u_char AddrNum;					 // addr����Ԫ������
	//ADDR_RADER_INFO p2pAddr;				 // P2Pͨ��ʱӦʹ�õĵ�ַ���ͻ���ʹ�ã�
	//char oncehash[7];

	int PositionAmount[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //�ֲַ�������

	int LastTurn[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //������
	double LastTurnTime[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //������


	int LastCount[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //������
							//double LastTurnTime[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //������


	double LastStopTime[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //����

								  //int CROSS_SINGLE_SELECT[TYPE_NUM][MAX_CROSSSINGLE_NUM];   //�����źŷ��� 0�գ�1��  2��
								  //double CROSS_SINGLE_TIME[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //����



	bool LockState[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };




	double	LastHighPrice[TYPE_NUM];  //��ֹ̬�𣬱�����߼�
	double	LastLowPrice[TYPE_NUM];   //��ֹ̬�𣬱�����ͼ�

	double DynamicStopLossTime[TYPE_NUM]; //��ֹ̬���ʱ

	double open[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��۸�
	double close[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��۸�
	double high[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��۸�
	double low[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��۸�

											  //int CROSS_SINGLE_TURN[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //����,�źŷ��������1��2��3��-1��-2��-3

											  //double CROSS_SINGLE_PRICE[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��۸�
											  //double CROSS_SINGLE_SPACE_VALUE[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮��ռ�
											  //double CROSS_SINGLE_SPACE_RATE[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�����ź�֮�䲨���ռ��ʣ�Ӧ�ô���3������

											  //double CROSS_SINGLE_SPACE_PER_M1[TYPE_NUM][MAX_CROSSSINGLE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	  //�ۺϳ�ÿ�����ź�֮��ƽ�������ռ��ʣ�Ӧ�ô���3������


											  //double EMA_SPACE_M[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // ��M��EMA�ռ�
											  //double EMA_SPACE_N[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // ��N��EMA�ռ�

											  //double EMA_SPACE_MN[TYPE_NUM];// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };// ��MN��EMA ƽ���ռ�
											  //PEER_INFO   OrderUserPeerList;  // �����ò����ֵ��û��б�;



 



	
};
#pragma pack()