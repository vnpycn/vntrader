#ifndef CIPNETTYPECONVERT_H_H_
#define CIPNETTYPECONVERT_H_H_
#include "inc.h"
#include "..\public\CConvertString.h"
namespace HongLib
{
	class CIpNetTypeConvert:private CConvertString
	{
	public:
		inline static TCHAR* WideCharToTCHAR(const WCHAR* pwcWideName,DWORD* pdwANSILen = NULL);
		inline static TCHAR* PhyNameBeToTCHAR(const BYTE* pdwPhyByteAddr,const DWORD& dwPhyByteLen,DWORD* pdwPhyCharLen = NULL);
		inline static TCHAR* ByteToTCHAR(const BYTE* pdwByteArrPointer,const DWORD& dwByteLen,DWORD* pdwTCHARLen);
		inline static void* IPHpCopyMemory(const void* pvSrc,const DWORD& dwSrcSize);
		inline static TCHAR* DWORDToAddr(const DWORD& dwValue,DWORD* pdwSize);
		inline static ULONG ADDRToULONG(const TCHAR* tcAddr);
	};

	/*****************
	���ܣ�MAC��ַBYTEת��Ϊchar��
	������pdwPhyAddr  BYTE��MAC�����ַָ�� 
	pdwPhyAddrLen BYTE��MAC���鳤��
	ptcPhyAddr  TCHAR��ָ��
	pdwSize TCHAR���ڴ��ַ���
	���أ�true ת���ɹ�
	false ת��ʧ��
	ʹ�ã�
	1.��ptcPhyAddr=NULL ͨ�� pdwSize��ȡ����
	2.�����ڴ�
	3.�ٵ��ú���
	*****************/
	TCHAR* CIpNetTypeConvert::PhyNameBeToTCHAR(const BYTE* pdwPhyByteAddr,const DWORD& dwPhyByteLen,DWORD* pdwPhyCharLen)
	{

		TCHAR* tmpBuffer;
		if(NULL == pdwPhyByteAddr || 0 == dwPhyByteLen)
		{
			return NULL;
		}
		else
		{
			tmpBuffer = new TCHAR[dwPhyByteLen*3];
		}
#ifdef UNICODE
		wchar_t wcTmpString[3];
		HRESULT nRet;


		nRet = StringCchPrintf(wcTmpString,3,TEXT("%X"),pdwPhyByteAddr[0]);
		if(!SUCCEEDED(nRet))
		{
			delete[] tmpBuffer;
			return NULL;
		}

		nRet = StringCchCopy(tmpBuffer,(size_t)(dwPhyByteLen*3),wcTmpString);
		if(!SUCCEEDED(nRet))
		{
			delete[] tmpBuffer;
			return NULL;
		}

		for(DWORD i=1;i<dwPhyByteLen;i++)
		{
			nRet = StringCchCat(tmpBuffer,(size_t)(dwPhyByteLen*3),TEXT("-"));
			if(!SUCCEEDED(nRet))
			{
				delete[] tmpBuffer;
				return NULL;
			}


			nRet = StringCchPrintf(wcTmpString,3,TEXT("%X"),pdwPhyByteAddr[i]);
			if(!SUCCEEDED(nRet))
			{
				delete[] tmpBuffer;
				return NULL;
			}

			nRet = StringCchCat(tmpBuffer,(size_t)(dwPhyByteLen*3),wcTmpString);
			if(!SUCCEEDED(nRet))
			{
				delete[] tmpBuffer;
				return NULL;
			}

		}


#else
		char cTmpString[3]; 
		HRESULT nRet;
		nRet = StringCchPrintf(cTmpString,3,TEXT("%02X"),pdwPhyByteAddr[0]);
		if(!SUCCEEDED(nRet))
		{
			delete[] tmpBuffer;
			return NULL;
		}
		nRet = StringCchCopy(tmpBuffer,(size_t)(dwPhyByteLen*3),cTmpString);
		if(!SUCCEEDED(nRet))
		{
			delete[] tmpBuffer;
			return NULL;
		}

		for(DWORD i=1;i< dwPhyByteLen;i++)
		{
			nRet = StringCchCat(tmpBuffer,(size_t)(dwPhyByteLen*3),TEXT("-"));
			if(!SUCCEEDED(nRet))
			{
				delete[] tmpBuffer;
				return NULL;
			}


			nRet = StringCchPrintf(cTmpString,3,TEXT("%02X"),pdwPhyByteAddr[i]);
			if(!SUCCEEDED(nRet))
			{
				delete[] tmpBuffer;
				return NULL;
			}

			nRet = StringCchCat(tmpBuffer,(size_t)(dwPhyByteLen*3),cTmpString);
			if(!SUCCEEDED(nRet))
			{
				delete[] tmpBuffer;
				return NULL;
			}

		}
#endif
		return tmpBuffer;
	}
	/***********************
	���ܣ���UNICODEת��ΪANSI
	������ͬ��

	***********************/
	TCHAR* CIpNetTypeConvert::WideCharToTCHAR(const WCHAR* pwcWideName,DWORD* pdwANSILen)
	{
		TCHAR* pTmpChar;
		size_t tsTmpSize;
		if(NULL  != pwcWideName)
			return NULL;
#ifdef UNICODE

		HRESULT nRet = StringCchLength((LPCTSTR)pwcWideName,MAX_INTERFACE_NAME_LEN,&tsTmpSize);

		if(SUCCEEDED(nRet))
		{
			if(0 == tsTmpSize)
				return NULL;

			if(NULL != pdwANSILen)
			{
				*pdwANSILen = static_cast<DWORD>(tsTmpSize + 1);//�õ��ַ���С
			}
			pTmpChar = new wchar_t[tsTmpSize + 1];
			wmemcpy(pTmpChar,pwcWideName,tsTmpSize);
			return pTmpChar;
		}
		return NULL;
#else

		CConvertString::WideCharToANSIAutoAlloc(pwcWideName,pTmpChar,(int*)&tsTmpSize);
		return pTmpChar;

#endif
	}
	/*****************
	���ܣ���BYTE������ת��ΪTCHAR*��
	������ͬ��
	*****************/
	TCHAR* CIpNetTypeConvert::ByteToTCHAR(const BYTE* pdwByteArrPointer,const DWORD& dwByteLen,DWORD* pdwTCHARLen)
	{
		TCHAR* ptcTmpCHAR;
#ifdef UNICODE
		CConvertString::ANSIToWideCharAutoAlloc((LPCSTR)pdwByteArrPointer,ptcTmpCHAR);
		if(NULL != pdwTCHARLen)
			*pdwTCHARLen = dwByteLen;

#else
		if(0 == dwByteLen)
			return NULL;
		ptcTmpCHAR = new char[dwByteLen];
		memcpy(ptcTmpCHAR,(wchar_t*)pdwByteArrPointer,dwByteLen);
		if(NULL != pdwTCHARLen)
			*pdwTCHARLen = dwByteLen;

#endif
		return ptcTmpCHAR;

	}


	/*************
	���ܣ��ڴ濽��
	������ͬ��
	**************/
	void* CIpNetTypeConvert::IPHpCopyMemory(const void* pvSrc,const DWORD& dwSrcSize)
	{
		if(NULL == pvSrc || 0 == dwSrcSize)
			return NULL;
		void* pvTmpDest = new char[dwSrcSize];
		memcpy(pvTmpDest,pvSrc,dwSrcSize);
		return pvTmpDest;
	}
	/*************
	���ܣ�DWORDת��Ϊ�����ַ��ʽ�����磺0.0.0.0��
	������ͬ��
	**************/
	TCHAR* CIpNetTypeConvert::DWORDToAddr(const DWORD& dwValue,DWORD* pdwSize)
	{
		in_addr in;
		TCHAR* tcRetAddr;
		char* cTmpAddr;
		in.S_un.S_addr = dwValue;
		cTmpAddr = inet_ntoa(in);
#ifdef UNICODE
		CConvertString::ANSIToWideCharAutoAlloc(cTmpAddr,tcRetAddr,(int*)pdwSize);
#else
		size_t nTmpSize;
		HRESULT nRet = StringCchLength((char*)cTmpAddr,20,&nTmpSize);
		if(SUCCEEDED(nRet))
		{
			tcRetAddr = (TCHAR*)CIpNetTypeConvert::IPHpCopyMemory((void*)cTmpAddr,(DWORD)((nTmpSize+1)*2));
		}
		else
		{
tcRetAddr = NULL;
		}
#endif
		return tcRetAddr;
	}
	/*************
	���ܣ��������ַת��ΪULONG
	������ͬ��
	**************/
	ULONG CIpNetTypeConvert::ADDRToULONG(const TCHAR* tcAddr)
	{
		ULONG unAddr;
#ifdef UNICODE
		char* tTmpAddr;
		if(CConvertString::WideCharToANSIAutoAlloc(tcAddr,tTmpAddr))
		{
			unAddr = inet_addr(tTmpAddr);
			delete tTmpAddr;
		}
#else
  unAddr = inet_addr(tcAddr);
#endif
  return unAddr;
	}
}

#endif