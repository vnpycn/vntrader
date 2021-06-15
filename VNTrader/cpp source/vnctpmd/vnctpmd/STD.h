/***************************************************************************/
/* Module:          DataL.h                                             */
/* Purpose:         Data Structure Of Trade                                */
/* Stub Module:                                                            */
/* Driver Module:                                                          */
/* Author:          chenhongzhang                                            */
/* Corporation:     Qian Long                                              */
/* Date:            2003.11.01 -                                           */
/***************************************************************************/
#ifndef _STD_HEADER
#define _STD_HEADER
//#include <Afxtempl.h>
//baimh,20060731���������ʱ����mfc������Afxtempl.h����뱨��
//TCHAR.h��_T
//stdio.h:sprintf

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <vector>
using namespace std;

#ifdef	__MY_MFC	//ʹ����MFC
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#else
#include <TCHAR.h>
#include <windows.h>
#endif	// end __MY_MFC	//ʹ����MFC

//#include "TListBuffer.h"

class	STD
{
public:
	static long DateAdd(long date,long step);
	static long     GetCount(const char * src,char ch);
	//ȥ����ֹ�ո�
	static char *	ltrim(char *str);
	static char *	rtrim(char *str);
	static char *	alltrim(char *str);
	static char *   TrimEnter(char* str);
	static char*	TrimChar(char* str, char c) ;

	static char*	DeleteChar(char* str, char c) { return TrimChar(str, c); }

	static WCHAR*	trimEnterAndSpace(WCHAR* str);

	static char*	TrimSpaceAndLine(char* str);

	// ��һ���ַ����в������ִ�����chΪ�ָ��������ҵ�num�����ִ�����1��ʼ
	// �磺||��Ʊ����|5800|200|55555555|000001|�չA|29.00|200|
	// ����'|'�ָ���
	static char *	GetValue(const char *src,char *dest , int size, int num,char ch);
	static int		GetValue(const char *src,int num,char ch);

	//��ʽ��Ϊnum���ַ�������ʱ���ch�ַ�������ʱ���ȡ���ҽ�ȡ�ַ�
	static char*	padl(char *str,short num,char ch);
	static char*	padr(char *str,short num,char ch);

	//����num���ַ���������num��ʱ��ֻ������num���ַ�
	static char*	strcpyr(char* dest, const char* src, int num);	//�������ַ�

	//static char*	my_strncpy(char* dest, const char* src, int num);

	static	char*	my_strncpy(char* dst, const char* src, int num, char end = '\0');		//�������ݣ�ֱ������������
	static	char*	strncpy_trim(char* dst, const char* src, int num, char end = '\0');	//�������ݲ�ȥ���ո�ֱ������������

	//srcԭ���ݣ�lenԭ���ݿɴ洢�����ֵ
	static	char*	strreplace(char *src, int len, const char *oldstr, const char *newstr);

	static long		my_fatol(const char* str);	// �������ַ���ת�������Σ�����10000
	static long		my_ftol(const double f);	// ������ת�������Σ�����10000
	static char*	my_ltoa(const long l, char* out, int size);		// ����ת�����ַ��������Σ�10000

	//��ȡtimevalue��Ӧ�������գ�����20061010��ʽ�����ڣ�timevalueΪ0ʱȡ��ǰʱ��
	static long			GetDate(int& year, int& month, int& day, long timevalue = 0);
	static long			GetDate(long timevalue = 0);	//����20061010��ʽ�ĵ�ǰ����
	//��ȡtimevalue��Ӧ��ʱ���룬����154201��ʽ��ʱ�䣬timevalueΪ0ʱȡ��ǰʱ��
	static long			GetTime(int& hour, int& minute, int& second, long timevalue = 0);
	static long			GetTime(long timevalue = 0);	//����20061010��ʽ�ĵ�ǰ����

	static void			GetTime(int& year, int& month, int& day, int& hour, int& minute, int& second, long timevalue = 0);
	static void			GetTime(int& date, int& time, long timevalue = 0);
	static char*		GetTimeString(time_t timevalue, char* out, int len);

	static long			DateMinus(long date1, long	date2 = 0);	//date2-date1�����������������ʽΪ20061103��date2Ϊ0ʱȡ��ǰ����

	static long		rand(int max_rand = 0x7fff);

	static char*	MobilePhoneCharReplace(const char* src, char* dst);
	//static char*	MobilePhoneEncrypt(const char* src, char* dst);
	// ��������ֵ��oper��Ϊ'+','-','/'���ֱ����ӡ�����������������Result�У�Dec�Ǵ�ŵ�С����λ����Ϊ0ʱû��С���㣬�����Ϊ1,2,3
	static int  Float(char *Float1,char *Float2,char oper,char *Result, int Dec);

	//��ʽ�������ַ����������е�','ȥ������:999,999,999.9999->999999999.9999
	static double FormatMoneyDouble(const char* money);
	static char* FormatMoneyString(char* out, const char* money, int dec);

	static int FormatInt(const char* money);

	static int Utf8ToAnsi(char* out, int len, const char *szUTF8, int utf_len/*, int nCodePage = 936*/);
	static int AnsiToUtf8(char* out, int len, const char *src);
//	static int UnicodeToUtf8(TListBuffer& unicode, TListBuffer& utf8);
	static int UnicodeToUtf8(const WCHAR* unicode, char* utf8, int size);
	static WCHAR*	Utf8ToUnicode(const char* src);	//����new

	static int		days_of_year ( int year, int mon, int day );	//ĳ���Ǹ���ĵڼ���
	static long		days_num(long startrq,long endrq);				//��������֮����������

	static INT64		FloatToInt(double f);

	static INT64		IntDiv(INT64 value, INT32 times)
	{
		if (times > 0)
		{
			return (value + (times>>1))/times;
		}
		else
		{
			return value;
		}
	}
	
	
	//�õ�ĳ���ǵڼ�������week*7+weekday(0-6)
	//����7����Ϊ�ڼ�������
	//����7������Ϊһ�����ڵ����ڼ���0��Ϊ�����죬1������һ,...
	static long		getweek(long date ); //�õ�ĳ���ǵڼ�������week*7+weekday(0-6)
	
	static long getnextdate(long rq);         //ȡ��һ����
	static long getpreviousrq(long rq);         //ȡǰһ����
	
	static bool IsRunnian(long date);		//date YYYYMMDD �ж��ǲ�������
	static int GetOtherDay(int date,int nSpan); //��date ���� nSpan �� ���ؼ���������YYYYMMDD
												//nSpan>=0 ��ǰ���� nSpan<0 ���� -364<nSpan<365

	//��ʽyyyymmdd-hhmmss
	static time_t	GetSeconds(const char* times);
	
	//��ANSIת����UNICODE������ת�����ַ�����������������
	static	int		ConvertToUnicode(WCHAR* dst, int size, const char* src, int len = -1);
	
	//��ANSIת����UNICODE������ת�����ַ���������������������Ŀ���������Ϊchar,ע����뷽ʽ,sizeָ��dst�ܴ洢���ֽڸ���+1
	static	int		ConvertToUnicode(char* dst, int size, const char* src, int len = -1);
	//��ANSIת����UNICODE������ת������ַ�����ע��ʹ���������delete[]
	static WCHAR*	ConvertToUnicode(char* src);
	
	//��UNICODEת����ANSI������ת�����ַ�����������������
	static	int		ConvertToAnsi(char* dst, int size, const WCHAR* src, int len = -1);
	
	//��UNICODEת����ANSI������ת�����ַ���������������������ԭ�������Ϊchar��ע����뷽ʽ,lenָ��src���ֽڸ���
	static	int		ConvertToAnsi(char* dst, int size, const char* src, int len);
//	static	int		ConvertToAnsi(char* dst, int size, TListBuffer* src)	//char
	//{
	//	return ConvertToAnsi(dst, size, src->GetPtr(), src->size());
	//}
//	static	int		ConvertToAnsi(TListBuffer* dst, TListBuffer* src)	//char
	//{
	//	dst->SetBufferSize(src->size()+100);
	//	int ret = ConvertToAnsi(dst->GetPtr(), dst->GetBufferSize(), src->GetPtr(), src->size());
	//	dst->Add(ret);
	//	return ret;
	//}

	//---------------------------------------------------------------------------------------
	static BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize,DWORD &dwReturn);
	

	//���ִ�Сд��������'|'��'='֮ǰû�пո��硰JY = fjjj | ���Ǵ����
	//��һ���ַ����в��Ҳ�������ʽ��name1=1111|name2=2222|...
	static char*	GetPara(const char* src, char* dst, int size, const char* para);
	static int		GetPara(const char* src, const char* para);

	//first = '\1' second = '='
	static char*	GetPara(const char* src, char* dst, int size, const char* para, char first, char second);
	static int		GetPara(const char* src, const char* para, char first, char second);

	static long GZCompress(char * src, int srclen, vector<char>& dst);
//	static long GZUncompress(char * src, int srclen, TListBuffer& dst);	//char


	static INT32		GetDateTimeFromString(const char* str)
	{
		if (strchr(str, '/'))
		{
			return STD::GetValue(str, 1, '/') * 10000 + STD::GetValue(str, 2, '/') * 100 + STD::GetValue(str, 3, '/');
		}
		else if (strchr(str, '\\'))
		{
			return STD::GetValue(str, 1, '\\') * 10000 + STD::GetValue(str, 2, '\\') * 100 + STD::GetValue(str, 3, '\\');
		}
		else if (strchr(str, '-'))
		{
			return STD::GetValue(str, 1, '-') * 10000 + STD::GetValue(str, 2, '-') * 100 + STD::GetValue(str, 3, '-');
		}
		else if (strchr(str, ':'))
		{
			return STD::GetValue(str, 1, ':') * 10000 + STD::GetValue(str, 2, ':') * 100 + STD::GetValue(str, 3, ':');
		}
		else
		{
			return atoi(str);
		}
	}

	static long		GetVersionLong(const char* ver);

	static char*  MD5_JustifytoStr(char* out_md5, int size, const char md5[16])
	{   
		_snprintf_s(out_md5, size, size-1, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\0",
			md5[0] & 0xFF, md5[1] & 0xFF, md5[2] & 0xFF, md5[3] & 0xFF,
			md5[4] & 0xFF, md5[5] & 0xFF, md5[6] & 0xFF, md5[7] & 0xFF, 
			md5[8] & 0xFF, md5[9] & 0xFF, md5[10] & 0xFF, md5[11] & 0xFF, 
			md5[12] & 0xFF, md5[13] & 0xFF, md5[14] & 0xFF, md5[15] & 0xFF);
		return out_md5;
	}  

	//���string�ַ����Ƿ���list�У�����string��list�Ѿ�����Сд��
	static bool		isInList(const char* string, const char* list, char ch);

	static WCHAR*	ReadFile(const char* file);

};

#define		dim(v)		(sizeof(v)/sizeof(v[0]))




#endif
