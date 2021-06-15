#ifndef CCONVERTSTRING_H_H_
#define CCONVERTSTRING_H_H_
#include <windows.h>
namespace HongLib
{
	class CConvertString
	{
	public:
		CConvertString()
		{
		}
		~CConvertString()
		{
		}
		inline static bool WideCharToANSIAutoAlloc(LPCWSTR lpWideChar,LPSTR& plpANSIChar,int* pnANSINum = NULL,const int& pnWideNum = -1);
		inline static bool ANSIToWideCharAutoAlloc(LPCSTR lpANSIChar,LPWSTR& plpWideChar,int* pnWideNum = NULL,const int& pnANSINum = -1);
		inline static bool WideCharToANSI(LPCWSTR lpWideChar,LPSTR& plpANSIChar,int* pnANSINum,const int& pnWideNum = -1);
		inline static bool ANSIToWideChar(LPCSTR lpANSIChar,LPWSTR& plpWideChar,int* pnWideNum,const int& pnANSINum = -1);
	};
	/********************
	功能：UNICODE转换为ANSI自动分配转换后的字符串内存
	参数：
	lpWideChar UNICODE字符串指针 输入
	pnWideNum UNICODE字符串大小 输入
	plpANSIChar ANSI字符串指针 输入输出
	pnANSINum ANSI字符串大小 输出
	返回值：false 失败
	true 成功
	说明：plpANSIChar指向内存需要调用程序进行回收回收
	********************/
	bool CConvertString::WideCharToANSIAutoAlloc(LPCWSTR lpWideChar,LPSTR& plpANSIChar,int* pnANSINum,const int& pnWideNum)
	{
		plpANSIChar = NULL;
		int nTmpANSINum;
		if(NULL == lpWideChar)
			return false;
		nTmpANSINum = WideCharToMultiByte(CP_OEMCP,NULL,lpWideChar,pnWideNum,NULL,0,NULL,FALSE);
		if(nTmpANSINum <= 0)
			return false;
		plpANSIChar = new CHAR[nTmpANSINum];
		int nWritten;
		nWritten = WideCharToMultiByte(CP_OEMCP,NULL,lpWideChar,pnWideNum,plpANSIChar,nTmpANSINum,NULL,FALSE);
		if(nWritten <= 0)
		{
			delete[] plpANSIChar;
			return false;
		}
		if(NULL != pnANSINum)
		{
			*pnANSINum = nWritten;
		}
		return true;

	}
	/**********************
	功能：将ANSI转换为UNICODE
	其它：同上
	**********************/
	bool CConvertString::ANSIToWideCharAutoAlloc(LPCSTR lpANSIChar,LPWSTR& plpWideChar,int* pnWideNum,const int& pnANSINum)
	{
		plpWideChar = NULL;
		int nTmpWideNum;
		if(NULL == lpANSIChar)
			return false;
		nTmpWideNum = MultiByteToWideChar(CP_ACP,0,lpANSIChar,pnANSINum,NULL,0);

		if(nTmpWideNum <= 0)
			return false;
		plpWideChar = new WCHAR[nTmpWideNum];
		int nWritten;
		nWritten = MultiByteToWideChar(CP_ACP,0,lpANSIChar,pnANSINum,plpWideChar,nTmpWideNum);
		if(nWritten <= 0)
		{
			delete[] plpWideChar;
			return false;
		}
		if(NULL != pnWideNum)
		{
			*pnWideNum = nWritten;
		}
		return true;
	}


	/**********************
	功能：将宽字符转换为ANSI
	参数：lpWideChar 宽字符 输入
	plpANSIChar 输入（输出） 当它等于NULL传递进去则pnANSINum返回需要内存大小
	pnANSINum 输入（输出) 当plpANSIChar等于NULL 返回需要空间大小 
	pnWideNum 一般为-1
	当plpANSIChar不等于NULL 传递需要内存大小，返回实际写入的大小
	返回值：true 成功 
	false 失败
	说明：
	函数不会自动分配内存
	使用：1.需要先将plpANSIChar=NULL 传递进去，通过pnANSINum获取需要内存大小
	2.plpANSIChar = new char[nANSINum];分配空间
	3.将plpANSIChar 和 nANSINum 传递进去
	**********************/
	inline bool CConvertString::WideCharToANSI(LPCWSTR lpWideChar,LPSTR& plpANSIChar,int* pnANSINum,const int& pnWideNum)
	{
		if(NULL == pnANSINum || NULL == lpWideChar)
			return false;
		if(NULL == plpANSIChar)
		{
			*pnANSINum = WideCharToMultiByte(CP_OEMCP,NULL,lpWideChar,pnWideNum,NULL,0,NULL,FALSE);
			if(*pnANSINum > 0)
				return true;
			else
				return false;
		}
		int nWritten;
		nWritten = WideCharToMultiByte(CP_OEMCP,NULL,lpWideChar,pnWideNum,plpANSIChar,*pnANSINum,NULL,FALSE);
		*pnANSINum = nWritten;
		return true;

	}
	/**********************
	功能：将宽字符转换为ANSI
	其它：同上
	**********************/
	inline bool CConvertString::ANSIToWideChar(LPCSTR lpANSIChar,LPWSTR& plpWideChar,int* pnWideNum,const int& pnANSINum)
	{
		if(NULL == lpANSIChar || NULL == pnWideNum)
			return false;
		if(NULL == plpWideChar)
		{
			*pnWideNum = MultiByteToWideChar(CP_ACP,0,lpANSIChar,pnANSINum,NULL,0);
			if(*pnWideNum > 0)
				return true;
			else
				return false;
		}
		int nWritten;
		nWritten = MultiByteToWideChar(CP_ACP,0,lpANSIChar,pnANSINum,plpWideChar,*pnWideNum);
		*pnWideNum = nWritten;
		return true;

	}
}
#endif