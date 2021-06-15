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
	���ܣ�UNICODEת��ΪANSI�Զ�����ת������ַ����ڴ�
	������
	lpWideChar UNICODE�ַ���ָ�� ����
	pnWideNum UNICODE�ַ�����С ����
	plpANSIChar ANSI�ַ���ָ�� �������
	pnANSINum ANSI�ַ�����С ���
	����ֵ��false ʧ��
	true �ɹ�
	˵����plpANSICharָ���ڴ���Ҫ���ó�����л��ջ���
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
	���ܣ���ANSIת��ΪUNICODE
	������ͬ��
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
	���ܣ������ַ�ת��ΪANSI
	������lpWideChar ���ַ� ����
	plpANSIChar ���루����� ��������NULL���ݽ�ȥ��pnANSINum������Ҫ�ڴ��С
	pnANSINum ���루���) ��plpANSIChar����NULL ������Ҫ�ռ��С 
	pnWideNum һ��Ϊ-1
	��plpANSIChar������NULL ������Ҫ�ڴ��С������ʵ��д��Ĵ�С
	����ֵ��true �ɹ� 
	false ʧ��
	˵����
	���������Զ������ڴ�
	ʹ�ã�1.��Ҫ�Ƚ�plpANSIChar=NULL ���ݽ�ȥ��ͨ��pnANSINum��ȡ��Ҫ�ڴ��С
	2.plpANSIChar = new char[nANSINum];����ռ�
	3.��plpANSIChar �� nANSINum ���ݽ�ȥ
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
	���ܣ������ַ�ת��ΪANSI
	������ͬ��
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