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
//baimh,20060731，如果编译时不带mfc，则有Afxtempl.h会编译报错
//TCHAR.h：_T
//stdio.h:sprintf

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <vector>
using namespace std;

#ifdef	__MY_MFC	//使用了MFC
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
#endif	// end __MY_MFC	//使用了MFC

//#include "TListBuffer.h"

class	STD
{
public:
	static long DateAdd(long date,long step);
	static long     GetCount(const char * src,char ch);
	//去除起止空格
	static char *	ltrim(char *str);
	static char *	rtrim(char *str);
	static char *	alltrim(char *str);
	static char *   TrimEnter(char* str);
	static char*	TrimChar(char* str, char c) ;

	static char*	DeleteChar(char* str, char c) { return TrimChar(str, c); }

	static WCHAR*	trimEnterAndSpace(WCHAR* str);

	static char*	TrimSpaceAndLine(char* str);

	// 从一个字符串中查找子字串，以ch为分隔符，查找第num个子字串，从1开始
	// 如：||股票买卖|5800|200|55555555|000001|深发展A|29.00|200|
	// 是以'|'分隔符
	static char *	GetValue(const char *src,char *dest , int size, int num,char ch);
	static int		GetValue(const char *src,int num,char ch);

	//格式化为num个字符，不足时添加ch字符，超过时左截取或右截取字符
	static char*	padl(char *str,short num,char ch);
	static char*	padr(char *str,short num,char ch);

	//拷贝num个字符串，超过num个时，只拷贝右num个字符
	static char*	strcpyr(char* dest, const char* src, int num);	//拷贝右字符

	//static char*	my_strncpy(char* dest, const char* src, int num);

	static	char*	my_strncpy(char* dst, const char* src, int num, char end = '\0');		//拷贝数据，直到遇到结束符
	static	char*	strncpy_trim(char* dst, const char* src, int num, char end = '\0');	//拷贝数据并去掉空格，直到遇到结束符

	//src原数据，len原数据可存储的最大值
	static	char*	strreplace(char *src, int len, const char *oldstr, const char *newstr);

	static long		my_fatol(const char* str);	// 浮点型字符串转换成整形，乘了10000
	static long		my_ftol(const double f);	// 浮点型转换成整形，乘了10000
	static char*	my_ltoa(const long l, char* out, int size);		// 整形转换成字符串，整形＊10000

	//获取timevalue对应的年月日，返回20061010格式的日期，timevalue为0时取当前时间
	static long			GetDate(int& year, int& month, int& day, long timevalue = 0);
	static long			GetDate(long timevalue = 0);	//返回20061010格式的当前日期
	//获取timevalue对应的时分秒，返回154201格式的时间，timevalue为0时取当前时间
	static long			GetTime(int& hour, int& minute, int& second, long timevalue = 0);
	static long			GetTime(long timevalue = 0);	//返回20061010格式的当前日期

	static void			GetTime(int& year, int& month, int& day, int& hour, int& minute, int& second, long timevalue = 0);
	static void			GetTime(int& date, int& time, long timevalue = 0);
	static char*		GetTimeString(time_t timevalue, char* out, int len);

	static long			DateMinus(long date1, long	date2 = 0);	//date2-date1，两个日期相减，格式为20061103，date2为0时取当前日期

	static long		rand(int max_rand = 0x7fff);

	static char*	MobilePhoneCharReplace(const char* src, char* dst);
	//static char*	MobilePhoneEncrypt(const char* src, char* dst);
	// 计算两数值，oper可为'+','-','/'，分别代表加、减、除，结果存放在Result中，Dec是存放的小数点位数，为0时没有小数点，否则可为1,2,3
	static int  Float(char *Float1,char *Float2,char oper,char *Result, int Dec);

	//格式化浮点字符串，将其中的','去掉，如:999,999,999.9999->999999999.9999
	static double FormatMoneyDouble(const char* money);
	static char* FormatMoneyString(char* out, const char* money, int dec);

	static int FormatInt(const char* money);

	static int Utf8ToAnsi(char* out, int len, const char *szUTF8, int utf_len/*, int nCodePage = 936*/);
	static int AnsiToUtf8(char* out, int len, const char *src);
//	static int UnicodeToUtf8(TListBuffer& unicode, TListBuffer& utf8);
	static int UnicodeToUtf8(const WCHAR* unicode, char* utf8, int size);
	static WCHAR*	Utf8ToUnicode(const char* src);	//返回new

	static int		days_of_year ( int year, int mon, int day );	//某日是该年的第几天
	static long		days_num(long startrq,long endrq);				//两个日期之间相差的天数

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
	
	
	//得到某日是第几个星期week*7+weekday(0-6)
	//除以7的数为第几个星期
	//除以7的余数为一个星期的星期几，0：为星期天，1：星期一,...
	static long		getweek(long date ); //得到某日是第几个星期week*7+weekday(0-6)
	
	static long getnextdate(long rq);         //取后一日期
	static long getpreviousrq(long rq);         //取前一日期
	
	static bool IsRunnian(long date);		//date YYYYMMDD 判断是不是润年
	static int GetOtherDay(int date,int nSpan); //由date 经过 nSpan 天 返回计算后的日期YYYYMMDD
												//nSpan>=0 向前增加 nSpan<0 向后减 -364<nSpan<365

	//格式yyyymmdd-hhmmss
	static time_t	GetSeconds(const char* times);
	
	//将ANSI转换成UNICODE，返回转换的字符个数，包含结束符
	static	int		ConvertToUnicode(WCHAR* dst, int size, const char* src, int len = -1);
	
	//将ANSI转换成UNICODE，返回转换的字符个数，包含结束符，但目标数据类别为char,注意对齐方式,size指明dst能存储的字节个数+1
	static	int		ConvertToUnicode(char* dst, int size, const char* src, int len = -1);
	//将ANSI转换成UNICODE，返回转换后的字符串，注意使用者需调用delete[]
	static WCHAR*	ConvertToUnicode(char* src);
	
	//将UNICODE转换成ANSI，返回转换的字符个数，包含结束符
	static	int		ConvertToAnsi(char* dst, int size, const WCHAR* src, int len = -1);
	
	//将UNICODE转换成ANSI，返回转换的字符个数，包含结束符，但原数据类别为char，注意对齐方式,len指明src的字节个数
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
	

	//区分大小写，假设在'|'或'='之前没有空格，如“JY = fjjj | ”是错误的
	//从一个字符串中查找参数，格式：name1=1111|name2=2222|...
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

	//检测string字符串是否在list中，假设string、list已经都是小写了
	static bool		isInList(const char* string, const char* list, char ch);

	static WCHAR*	ReadFile(const char* file);

};

#define		dim(v)		(sizeof(v)/sizeof(v[0]))




#endif
