#ifndef CINITSOCKET_H_H_
#define CINITSOCKET_H_H_
#include <winsock2.h>
#include <windows.h>
#include "CWindowsException.h"
#pragma comment(lib,"WS2_32")
/****************************************************
this is windows socket class 
author:BigHead
date:2010.12.21
***************************************************/
class CInitSocket
{
public:
	CInitSocket(BYTE majorVer = 2,BYTE minorVer = 2)
	{
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer,majorVer);
		if(0 != ::WSAStartup(sockVersion,&wsaData))
		{
			DWORD nError = (long)WSAGetLastError();
			throw CWindowsException(nError,"WSAStartup error");
		}
	}
	~CInitSocket()
	{
		::WSACleanup();

	}
};

#endif