/*
1.���ļ�ΪVNTrader �ڻ�CTP���׿�ײ����
2.VNTrader����C++�⿪ԴЭ��MITЭ��

��VNPY��Դ���������׵ģ����õ�ԭʼ���߿϶��ģ���������http://www.vnpy.cn��վ�ϣ�
������ڡ���Դ˵���͸�л.txt�����������ļ����ϸ��·���ÿһ���°汾��vnpy���

�ٷ���վ��http://www.vnpy.cn
*/
#pragma once
#undef UNICODE
#undef _UNICODE
#define WM_STRATEGY WM_USER+1 
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN           
#include <windows.h>
#include "CTP/ThostFtdcUserApiDataType.h"
#include "CTP/ThostFtdcUserApiStruct.h"
#include "CTP/ThostFtdcMdApi.h"
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <hash_map>
#define PYCTPMARKET_EXPORTS

#include <list>
struct cmdcontent
{
	int cmd;
	char content[31];
};
#define MAX_EVENTNUM  64


//�ص�����
#define SYSTEM_EMPTY                  8000 //��
#define SYSTEM_LOGIN_SCUESS           8001 //��¼�ɹ�
#define SYSTEM_LOGIN_DENIED           8002 //��¼���ܾ�
//#define SYSTEM_LOGIN_ERRORPASSWORD  8003 //������� ??
#define SYSTEM_LOGINOUT_SCUESS        8004 //�ǳ��ɹ�
#define SYSTEM_NETCONNECT_SCUESS      8005 //���ӳɹ�
#define SYSTEM_NETCONNECT_BREAK       8006 //�Ͽ�����
#define SYSTEM_NETCONNECT_FAILER      8007 //����ʧ�� ??
#define SYSTEM_SUBCRIBE_SCUESS        8008 //���ĳɹ�
#define SYSTEM_UNSUBCRIBE_SCUESS      8009 //ȡ�����ĳɹ�
#define SYSTEM_NEWTICK                8010 //��Tick����
#define SYSTEM_SYSTEM_ERROR           8011 //����Ӧ��
#define SYSTEM_QRY_FORQUOTE           8012 //
//#define SYSTEM_LOG                    8013 //��־
struct TThostFtdcInstrumentIDTypeStruct
{
	TThostFtdcInstrumentIDType Instrument;
};

 

#define  EID_OnFrontConnected      0
#define  EID_OnFrontDisconnected   1
#define  EID_OnRspUserLogin        2
//#define  EID_OnRspUserLogin_Scuess 3
//#define  EID_OnRspUserLogin_Failer 2
//#define  EID_OnRspUserLogin_Scuess 3
#define  EID_OnRtnDepthMarketData  4
#define  EID_IsErrorRspInfo        5
#define  EID_OnRspSubMarketData    6
#define  EID_OnRspUnSubMarketData  7
#define  EID_OnRspUserLogout       8
#define  EID_OnRspForQuote         9
