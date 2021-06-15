/***************************************************************************/
/* Module:          DataL.cpp                                             */
/* Purpose:         Implementation of DataL                               */
/* Stub Module:                                                            */
/* Driver Module:                                                          */
/* Author:          chenhongzhang                                            */
/* Corporation:     Qian Long                                              */
/* Date:            2003.11.01 -                                           */
/***************************************************************************/
#include <time.h>
#include "STD.h"
#include "TListBuffer.h"
#include <SYS\TIMEB.H>
#include <io.h>
#include "..\gzlib\zlib.h"
#include "..\..\include\rc5.h"
char * STD::ltrim(char *str){
	char *s=str;
	while (*s==' ') s++;
	strcpy(str,s);
	return str;
}

char *STD::rtrim(char *str){
	char *s=str+strlen(str)-1;
	while (s-str>=0 && (*s==' ' || *s=='\n')) s--;
	s++;
	*s=0;
	return str;
}

char *STD::alltrim(char *str)
{
	ltrim(str);
	rtrim(str);
	return str;
}

WCHAR*	STD::trimEnterAndSpace(WCHAR* str)
{
	WCHAR* p = str;
	while (1)
	{
		WCHAR v = *p;
		if (v == 0)
		{
			*str	= 0;
			break;
		}
		else if (v == '\r' || v == '\n' || v == ' ')
		{
			p++;
			continue;
		}
		wcscpy(str, p);
		break;
	}

	return str;
}

char*	STD::TrimSpaceAndLine(char* str)
{
	TrimEnter(str);
	return alltrim(str);
}

char * STD::padl(char *str,short num,char ch)
{
    size_t len = strlen(str);
    short movelen = num - strlen(str);

    if (movelen > 0) // 左添加字符ch
    {
        memmove(str + movelen, str, len);
        memset(str, ch, movelen);
    }
    else if (movelen < 0)// 取右字符
        memmove(str, str - movelen, num);

    str[num] = 0;

	return str;
}

char * STD::padr(char *str,short num,char ch)
{
    size_t len = strlen(str);
    short movelen = num - len;

    if (movelen > 0) // 右添加字符ch
        memset(str + len, ch, movelen);

    str[num] = 0;

	return str;
}

char* STD::strcpyr(char* dest, const char* src, int num)	//拷贝右字符
{
	size_t len		= strlen(src);
	int	movelen		= len-num;

	if (movelen > 0)
	{
		memcpy(dest, src+movelen, num);
		dest[num]	= 0;
	}
	else
		strcpy(dest, src);
	
	return dest;
}
/*
char*	STD::my_strncpy(char* dest, const char* src, int num)
{
	strncpy(dest, src, num-1);
	dest[num-1]	= 0;
	return dest;
}
*/

char*	STD::my_strncpy(char* dst, const char* src, int num, char end)	//拷贝数据，直到遇到结束符
{
	const char* psrc = src;
	char* pdst = dst;
	
	num--;
	while (num > 0)
	{
		char c = *psrc;
		if (c == end || c == '\0')
		{
			//*pdst	= '\0';
			break;
		}
		*pdst	= c;
		psrc++;
		pdst++;
		num--;
	}

	*pdst	= 0;
	
	return dst;
}
char*	STD::strncpy_trim(char* dst, const char* src, int num, char end)	//拷贝数据并去掉空格，直到遇到结束符
{
	const char* pstart = src;
	while (*pstart && *pstart == ' ') pstart++;	//取第一个非空格字符
	
	const char* pend	= pstart;
	while (*pend && *pend != end) pend++;	//找到结束符
	
	pend--;
	while (pend > pstart && *pend == ' ') pend--;	//去掉最后的空格
	
	int size = pend - pstart + 1;
	num--;
	if (num > size)
	{
		num	= size;
	}
	
	char* pdst = dst;
	while (num > 0)
	{
		*pdst++	= *pstart++;
		num--;
	}
	*pdst	= '\0';
	return pdst;
}


char * STD::GetValue(const char *src,char *dest , int size, int num,char ch)
{
	char *s0=dest;

    memset(dest, 0, size);

	if (ch == '\n')
	{
		while (*src!=0 && num!=1)
		{
			if (*src==ch)
				num--;
			src++;
		}
		
		while (*src!=0 && *src!=ch && size > 1)
		{
			size--;
			*s0++=*src++;
		}
	}
	else
	{
		while (*src!=0 && *src!='\n' && num!=1)
		{
			if (*src==ch)
				num--;
			src++;
		}
		
		while (*src!=0 && *src!='\n' && *src!=ch && size > 1)
		{
			size--;
			*s0++=*src++;
		}
	}

	alltrim(dest);
	
	return dest;
}

int STD::GetValue(const char *src,int num,char ch)
{
	char dest[30];

	return atoi(STD::GetValue(src, dest, sizeof(dest), num, ch) );
}

long STD::my_fatol(const char* str)	// 浮点型字符串转换成整形，乘了10000
{
	int value = STD::GetValue(str, 1, '.') * 10000;

	char buff[10];

	STD::GetValue(str, buff, sizeof(buff), 2, '.');

	int len = strlen(buff);

	if (len > 4)
	{
		len = 4;
		buff[4] = 0;
	}

	if (len < 4)
	{
		memset(buff+len, '0', 4-len);
		buff[4] = 0;
	}
	
	value += atoi(buff);

	return value;
}

long STD::my_ftol(const double f)	// 浮点型转换成整形，乘了10000
{
	return (long)(f*10000+0.5);
}

char* STD::my_ltoa(const long l, char* buff, int size)		// 整形转换成字符串，整形＊10000
{
	if (buff == NULL)
		return NULL;

	*buff	= 0;

	if (size < 1)
		return buff;

	memset(buff, 0, size);

	char	buffer[100];
	sprintf(buffer, "%d.%04d", l/10000, l%10000);
	
	lstrcpyn(buff, buffer, size);

	return buff;
}

long STD::GetDate(int& year, int& month, int& day, long timevalue)
{
	if (timevalue == 0)
		timevalue	= time(NULL);

	struct tm *newtime = localtime( &timevalue );
	
	year	= newtime->tm_year+1900;
	month	= newtime->tm_mon+1;
	day		= newtime->tm_mday;
	return 10000L*year + 100L*month + day;
}

long STD::GetDate(long timevalue)
{
	int year, month, day;

	return GetDate(year, month, day, timevalue);
}

long STD::GetTime(int& hour, int& minute, int& second, long timevalue)
{
	if (timevalue == 0)
		timevalue	= time(NULL);

	struct tm *newtime = localtime( &timevalue );
	
	if (newtime != NULL)
	{
		hour	= newtime->tm_hour;
		minute	= newtime->tm_min;
		second	= newtime->tm_sec;
	}
	else
	{
		hour	= 0;
		minute	= 0;
		second	= 0;
	}

	return 10000L*hour + 100L*minute + second;
}

long STD::GetTime(long timevalue)
{
	int hour, minute, second;

	return GetTime(hour, minute, second, timevalue);
}

void STD::GetTime(int& year, int& month, int& day, int& hour, int& minute, int& second, long timevalue)
{
	if (timevalue == 0)
		timevalue	= time(NULL);

	struct tm *newtime = localtime( &timevalue );
	
	if (newtime != NULL)
	{
		year	= newtime->tm_year+1900;
		month	= newtime->tm_mon+1;
		day		= newtime->tm_mday;
		hour	= newtime->tm_hour;
		minute	= newtime->tm_min;
		second	= newtime->tm_sec;
	}
	else
	{
		year	= 0;
		month	= 0;
		day		= 0;
		hour	= 0;
		minute	= 0;
		second	= 0;
	}
}

void	STD::GetTime(int& p_date, int& p_time, long timevalue)
{
	if (timevalue == 0)
		timevalue	= time(NULL);
	
	struct tm *newtime = localtime( &timevalue );
	
	if (newtime != NULL)
	{
		p_date	= (newtime->tm_year+1900)*10000 + (newtime->tm_mon+1)*100 + newtime->tm_mday;
		p_time	= newtime->tm_hour*10000 + newtime->tm_min*100 + newtime->tm_sec;
	}
	else
	{
		p_date	= 0;
		p_time	= 0;
	}
}

bool STD::IsRunnian(long date)		//date YYYYMMDD 判断是不是润年
{
	if( ( (date%4==0) && (date%100!=0) )||(date%400==0) )
	{
		return true;
		
	}
	else
	{
		return false;
	}
}
static int months[] = {31,28,31,30,31,30,31,31,30,31,30,31};
static int months1[] = {31,29,31,30,31,30,31,31,30,31,30,31};
int STD::GetOtherDay(int date,int nSpan)
{
	int bRunnian = false;
	int nPyear	= date/10000;
 	int nPmonth = (date - nPyear*10000) / 100;
 	int nPday	= date-nPyear*10000-nPmonth*100;

	int nDyear = 0;
	int nDmonth = 0;
	int nDday = 0;

	int nflag = 0;

	int nstartdays = 0;
	int nenddays = 0;
	int i = 0;
	
	int nTemp = 0;

	if(IsRunnian(nPyear))
	{
		for (i=0;i<nPmonth-1;++i)
		{
			nTemp += months1[i];
		}
	}
	else
	{
		for (i=0;i<nPmonth-1;++i)
		{
			nTemp += months[i];
		}
	}

	//这一年中此时的总天数
	nstartdays = nTemp + nPday;
	//跨度后的天数
	nenddays = nstartdays + nSpan;

	i = 0;
	nTemp = 0;
	int ntempyear = nPyear;
	int *p = NULL;

	if (nenddays >= 0)
	{
		do 
		{
			if(IsRunnian(ntempyear))
			{
				p = months1;
			}
			else
			{
				p = months;
			}
			if (nenddays < 31 )
			{
				ntempyear -= 1;
				i = 11;
				break;
			}
			nTemp += *(p+i);
			i++;
			if(i==12)
			{
				i=0;
				ntempyear++;
			}
			
			
		} while (nenddays - nTemp > *(p+i) );
		
		nDyear = ntempyear;
		nDmonth = i+1;
		nDday = nenddays - nTemp;
	}
	else
	{
		ntempyear -= 1;
		i = 12;

		do 
		{
			if(IsRunnian(ntempyear))
			{
				p = months1;
			}
			else
			{
				p = months;
			}
			i--;
			nTemp += *(p+i);
			if (i==0)
			{
				i = 12;
				ntempyear -= 1;
			}

		} while (nTemp + nenddays < 0);

		

				
		
		nDyear = ntempyear;
		nDmonth = i+1;
		nDday = nenddays + nTemp;
	}


	return nDyear*10000+nDmonth*100+nDday;

}

static int days_of_mon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
long getpreviousrq(long rq)         //取前一日期
{
         short year= (short)(rq/10000l);
         short mon = (short)(rq%10000);
         short day = (short)(mon%100);
         mon=mon/100;
         if ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) )
          days_of_mon[1]=29;
     else days_of_mon[1]=28;
     if (day>1)  return rq-1l;
     if (mon>1)  return (long)year*10000l+100l*(mon-1)+days_of_mon[mon-2];
     return  (long)(year-1l)*10000l+1231l;
}

short  days_of_year ( short year, short mon, short day ) // 某日是该年的第几天
{
         short  i, days;
         if ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) )
          days_of_mon[1]=29;
         else days_of_mon[1]=28;
         days = 0;
         for ( i = 0; i < mon - 1; i++ )        days += days_of_mon[i];
         days += day;
         return days;
}

long  days_num(long startrq,long endrq)
{
         int sign = 1;
         if(startrq > endrq)
         {
            sign = -1;
            long temp = startrq;
            startrq = endrq;
            endrq = temp;
         }
         short  startyear = (short)(startrq/10000);
         short  endyear   = (short)(endrq/10000);
         short  mon, day;
         long days = 0L;
         mon = (short)((endrq%10000)/100);
         day = (short)((endrq%10000)%100);
         days += days_of_year(endyear,mon,day );

         mon = (short)((startrq%10000)/100);
         day = (short)((startrq%10000)%100);
         if(startyear == endyear)
         {
                        days -= days_of_year(startyear,mon,day);
                        days += 1;
                        return days*sign;
         }
         else
         {
                if (startyear % 400 == 0 || ( startyear % 4 == 0 && startyear % 100 != 0 ) )
                        days += 366-days_of_year(startyear,mon,day);
                else
                        days += 365-days_of_year(startyear,mon,day);
                days += 1;
         }

         for(short i = startyear+1; i< endyear; i++)
         {
                if ( i % 400 == 0 || ( i % 4 == 0 && i % 100 != 0 ) )
                        days += 366;
                else days += 365;
         }

         return days*sign;
}
long STD::DateMinus(long date1, long	date2)	//date1-date2，两个日期相减，格式为20061103，date2为0时取当前日期
{
	if (date2 == 0)
		date2	= GetDate(time(NULL) );

	return days_num(date1, date2);
}

long STD::rand(int max_rand)
{
	static	long s = time(NULL);

	s = abs(s*29+37 + clock())%max_rand;

	return s;
}

long STD::GetCount(const char *src, char ch)
{
	long count = 0;
	while (*src!=0 && *src!='\n')
	{
		if (*src==ch)
			count++;
		src++;
	}
	
	return count;
}

//step 为天，可以为正也可以为负，为负则是减
long STD::DateAdd(long date, long step)
{
	struct tm tmtime;
	long ttime = 0;
		
	memset(&tmtime,0,sizeof(tmtime));
	tmtime.tm_year	= date/10000 -1900;
	if(tmtime.tm_year<=0)
		tmtime.tm_year = 0;
	tmtime.tm_mon	= (date/100)%100 -1;
	tmtime.tm_mday	= date%100;
	ttime = mktime(&tmtime);
	ttime = ttime + 86400*step;
	if(ttime<=0)
		ttime = 1;
	return STD::GetDate(ttime);
}

char*	STD::GetTimeString(time_t timevalue, char* out, int len)
{
	if (!out || len <= 0)
	{
		return NULL;
	}

	memset(out, 0, len);

	char	buffer[100];

	if (timevalue > 0)
	{
		int	year, month, day, hour, minute, second;

		STD::GetTime(year, month, day, hour, minute, second, timevalue);
		sprintf(buffer, "%4d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);

		strncpy(out, buffer, len-1);
	}

	return out;
}

// 计算两数值，oper可为'+','-','/'，分别代表加、减、除，结果存放在Result中，Dec是存放的小数点位数，为0时没有小数点，否则可为1,2,3
int  STD::Float(char *Float1,char *Float2,char oper,char *Result, int Dec)
{
    double f1, f2;
    double f3;
	
    f1 = atof(Float1);
    f2 = atof(Float2);
	
    switch(oper)
    {
	case '+':
		f3 = f1 + f2;
		break;
	case '-':
		f3 = f1 - f2;
		break;
	case '/':
		if (f2 < 0.0005)
			f3 = 0;
		else
			f3 = f1 / f2;
		break;
	case '*':
		f3 = f1 * f2;
		break;
	default:
		f3 = 0;
		break;
    }
	
    sprintf(Result, "%.*f", Dec, f3);
	
	
    if (f3 < -0.0005)
        return -1;
    else if (f3 > 0.0005)
        return 1;
    else
        return 0;
}

char* STD::FormatMoneyString(char* out, const char* money, int dec)
{
	const char* in = money;
	char* result = out;

	while (1)
	{
		char c = *in;

		if (c == 0)
		{
			*result = 0;
			break;
		}
		else if (c != ',')
		{
			if (result != in)
			{
				*result	= c;
			}

			result++;
		}

		in++;
	}

	double f = atof(out);
	sprintf(out, "%.*f", dec, f);
	
	return out;
}

double STD::FormatMoneyDouble(const char* money)
{
	char	out[100];
	const char* in = money;
	char* result = out;
	
	while (1)
	{
		char c = *in;
		
		if (c == 0)
		{
			*result = 0;
			break;
		}
		else if (c != ',')
		{
			if (result != in)
			{
				*result	= c;
			}
			
			result++;
		}
		
		in++;
	}
	
	return atof(out);
}

int STD::FormatInt(const char* money)
{
	char	out[100];
	const char* in = money;
	char* result = out;
	
	while (1)
	{
		char c = *in;
		
		if (c == 0)
		{
			*result = 0;
			break;
		}
		else if (c != ',')
		{
			if (result != in)
			{
				*result	= c;
			}
			
			result++;
		}
		
		in++;
	}
	
	return atoi(out);
}

INT64		STD::FloatToInt(double f)
{
	if (f < -0.00001)
	{
		return f - 0.5;
	}
	else
	{
		return f + 0.5;
	}
}




#ifndef	WINDOWSCODE
char* lstrcpyn(char* dest,char* src, long len)
{
	if (!dest || !src || len <= 0)
		return dest;

	memset(dest, 0, len);
	strncpy(dest, src, len-1);

	return dest;
}
char* lstrcpyn(char* dest, const char* src, long len)
{
	if (!dest || !src || len <= 0)
		return dest;

	memset(dest, 0, len);
	strncpy(dest, src, len-1);

	return dest;
}

char	toupper(char c)
{
	if (c >= 'a' && c<= 'z')
		return (c-('a'-'A'));
	return c;
}

char*	strupr(char* str)
{
	for (char* p = str; *p != 0; p++)
		*p	= toupper(*p);

	return str;
}

char	tolower(char c)
{
	if (c >= 'A' && c<= 'Z')
		return (c+('a'-'A'));
	return c;
}

char*	strlwr(char* str)
{
	for (char* p = str; *p != 0; p++)
		*p	= tolower(*p);

	return str;
}
#endif



int STD::Utf8ToAnsi(char* out, int len, const char *szUTF8, int utf_len/*, int nCodePage = 936*/)
{
	WCHAR	temp[32768+1];
	memset(temp, 0, sizeof(temp));
	int size = MultiByteToWideChar(CP_UTF8, 0, szUTF8, utf_len, temp, sizeof(temp)/sizeof(temp[0]));

	memset(out, 0, len);
	size = WideCharToMultiByte(CP_ACP, 0, temp, size, out, len, NULL, NULL);

	return size;
}

int STD::AnsiToUtf8(char* out, int len, const char *src)
{
	WCHAR buffer[32768+1];
	WCHAR* p = buffer;
	int srclen = strlen(src);
	
	if (srclen < dim(buffer))
	{
		srclen = ConvertToUnicode(buffer, dim(buffer), src, srclen);
	}
	else
	{
		p = new WCHAR[srclen+1];
		
		if (p)
		{
			srclen = ConvertToUnicode(p, srclen+1, src, srclen);
		}
	}

	memset(out, 0, len);
	int size = WideCharToMultiByte(CP_UTF8, 0, p, srclen, out, len, NULL, NULL);
	
	if (p != buffer)
	{
		delete[] p;
	}
	return size;
}

int STD::UnicodeToUtf8(TListBuffer& unicode, TListBuffer& utf8)
{
	int size = unicode.size() * 3 / 2 + 100;
	utf8.SetBufferSize(size);
	size = WideCharToMultiByte(CP_UTF8, 0, (WCHAR*)unicode.GetPtr(), unicode.size()/2, utf8.GetPtr(), size, NULL, NULL);
	utf8.Add(size);
	return size;
}

int STD::UnicodeToUtf8(const WCHAR* unicode, char* utf8, int size)
{
	memset(utf8, 0, size);
	return WideCharToMultiByte(CP_UTF8, 0, unicode, wcslen(unicode), utf8, size, NULL, NULL);
}

long STD::getpreviousrq(long rq)         //取前一日期
{
	char days_of_mon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int	year= (rq/10000l);
	int	mon = (rq/100%100);
	int day = (rq%100);

	if ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) )
	{
		days_of_mon[1]=29;
	}
	else 
	{
		days_of_mon[1]=28;
	}

	if (day>1)  
	{
		return rq-1l;
	}
	
	if (mon>1)  
	{
		return (long)year*10000l+100l*(mon-1)+days_of_mon[mon-2];
	}

	return  (long)(year-1l)*10000l+1231l;
}

long STD::getnextdate(long rq)         //取后一日期
{
	char days_of_mon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int	year= (rq/10000l);
	int	mon = (rq/100%100);
	int day = (rq%100);

	if ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) )
	{
		days_of_mon[1]=29;
	}
	else 
	{
		days_of_mon[1]=28;
	}

	if (day < days_of_mon[mon-1])  
	{
		return rq+1l;
	}
	else if (mon < 12)
	{
		return (long)year*10000l + 100l*(mon+1) + 1;
	}

	return  (long)(year+1)*10000l + 101;
}

int  STD::days_of_year ( int year, int mon, int day ) // 某日是该年的第几天
{
	char days_of_mon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
         int  i, days;
         if ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) )
          days_of_mon[1]=29;
         else days_of_mon[1]=28;
         days = 0;
         for ( i = 0; i < mon - 1; i++ )        days += days_of_mon[i];
         days += day;
         return days;
}

long  STD::days_num(long startrq,long endrq)
{
         int sign = 1;
         if(startrq > endrq)
         {
            sign = -1;
            long temp = startrq;
            startrq = endrq;
            endrq = temp;
         }
         short  startyear = (short)(startrq/10000);
         short  endyear   = (short)(endrq/10000);
         short  mon, day;
         long days = 0L;
         mon = (short)((endrq%10000)/100);
         day = (short)((endrq%10000)%100);
         days += days_of_year(endyear,mon,day );

         mon = (short)((startrq%10000)/100);
         day = (short)((startrq%10000)%100);
         if(startyear == endyear)
         {
                        days -= days_of_year(startyear,mon,day);
                        days += 1;
                        return days*sign;
         }
         else
         {
                if (startyear % 400 == 0 || ( startyear % 4 == 0 && startyear % 100 != 0 ) )
                        days += 366-days_of_year(startyear,mon,day);
                else
                        days += 365-days_of_year(startyear,mon,day);
                days += 1;
         }

         for(short i = startyear+1; i< endyear; i++)
         {
                if ( i % 400 == 0 || ( i % 4 == 0 && i % 100 != 0 ) )
                        days += 366;
                else days += 365;
         }

         return days*sign;
}

long STD::getweek(long date ) // 得到某日是第几个星期week*10+weekday(0-6)
{
    if(date<19800101l)	return 2;	//不能在1980年之前
    return days_num(19800101l, date) + 1;
}
//格式yyyymmdd-hhmmss
time_t	STD::GetSeconds(const char* times)
{
	int days = GetValue(times, 1, '-');
	int seconds = GetValue(times, 2, '-');
	struct tm t = {0};
	t.tm_year	= days / 10000 - 1900;
	if (t.tm_year < 0)
	{
		t.tm_year	= 0;
	}
	t.tm_mon	= (days/100)%100 - 1;
	t.tm_mday	= days%100;

	t.tm_hour	= seconds/10000;
	t.tm_min	= (seconds/100)%100;
	t.tm_sec	= seconds%100;

	time_t	ttime = mktime(&t);
	return ttime;
}


//---------------------------------------------------------------------------------------
BOOL STD::MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize,DWORD &dwReturn)
{	
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);
	
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}		
	// Convert headers from ASCII to Unicode.
	dwReturn = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwSize); 
	dwReturn = dwReturn * sizeof(WCHAR);
	return TRUE;
}


//区分大小写，假设在'|'或'='之前没有空格，如“JY = fjjj | ”是错误的
//从一个字符串中查找参数，格式：name1=1111|name2=2222|...
char*	STD::GetPara(const char* src, char* dst, int size, const char* para)
{
	return GetPara(src, dst, size, para, '|', '=');
	/*memset(dst, 0,size);
	//char *s0=dest;
	
	//const char* p = strchr(src, '|');
	
	int len = strlen(para);
	const char* p = strstr(src, para);	//假设全为小写
	
	if (p)
	{
		if (p > src)	//非第一个字符
		{
			if (*(p-1) != '|')	//假设没有空格
			{
				return dst;
			}
		}
		
		p += len;
		if (*p != '=')	//不是属性名
		{
			return dst;
		}
		
		p++;
		char* pdst = dst;
		size--;
		while(size > 0)
		{
			char c = *p;
			if (c == 0 || c == '|')
			{
				break;
			}
			*pdst	= *p;
			pdst++;
			p++;
			size--;
		}
	}
	
	//alltrim(dst);
	
	return dst;*/
}

int		STD::GetPara(const char* src, const char* para)
{
	char data[50];
	GetPara(src, data, sizeof(data), para);
	return atoi(data);
}

//first = '\1' second = '='
//从一个字符串中查找参数，格式：name1=1111|name2=2222|...
char*	STD::GetPara(const char* src, char* dst, int size, const char* para, char first, char second)
{
	memset(dst, 0,size);
	//char *s0=dest;
	
	//const char* p = strchr(src, '|');
	
	int len = strlen(para);
	
	//const char* result = NULL;
	const char* p		= NULL;
	if (strncmp(src, para, len) == 0 && src[len] == second)
	{
		p	= src;
		len++;
	}
	else
	{
		char key[100];
		len	= _snprintf(key, sizeof(key), "%c%s%c", first, para, second);

		p = strstr(src, key);	//假设全为小写
	}
	if (p)
	{
		p += len;
		
		char* pdst = dst;
		size--;
		while(size > 0)
		{
			char c = *p;
			if (c == 0 || c == first)
			{
				break;
			}
			*pdst	= *p;
			pdst++;
			p++;
			size--;
		}
	}
	
	//alltrim(dst);
	
	return dst;
}

int		STD::GetPara(const char* src, const char* para, char first, char second)
{
	char data[50];
	GetPara(src, data, sizeof(data), para, first, second);
	return atoi(data);
}

char*   STD::TrimEnter(char  *str)
{
    if(str==NULL)
        return NULL;
    char   *ptr=strchr(str,'\xa');
    if(ptr)
        *ptr=0;
    ptr = strchr(str, '\xd');
    if(ptr)
        *ptr=0;
    return ptr;
}

char*	STD::TrimChar(char* str, char c)
{
	char* dst = str;
	
	for (char*p = str; *p; p++)
	{
		if (*p == c)
		{
			continue;
		}
		
		if (dst != p)
		{
			*dst = *p;
		}
		
		dst++;
	}
	
	*dst = 0;
	
	return str;
}

int		STD::ConvertToUnicode(WCHAR* dst, int size, const char* src, int len)
{
	memset(dst, 0, sizeof(WCHAR)*size );
	if (len == -1)
	{
		len	= strlen(src);
	}
	return MultiByteToWideChar(CP_ACP, 0, src, len, dst, size-1);
}

//将ANSI转换成UNICODE，返回转换的字符个数，包含结束符，但目标数据类别为char,注意对齐方式,size指明dst能存储的字节个数+1
int		STD::ConvertToUnicode(char* dst, int size, const char* src, int len)
{
	memset(dst, 0, size );
	
	WCHAR buffer[2048+1];
	
	if (len == -1)
	{
		len	= strlen(src);
	}
	
	//len	= min(len, size/sizeof(TCHAR)-1);
	if (len < dim(buffer))
	{
		int ret = ConvertToUnicode(buffer, dim(buffer), src, len);
		ret		= min(ret, size/sizeof(WCHAR)-1);
		memcpy(dst, buffer, ret*sizeof(WCHAR));
		return ret;
	}
	else
	{
		WCHAR* p = new WCHAR[len+1];
		
		if (p)
		{
			int ret = ConvertToUnicode(p, len+1, src, len);
			ret		= min(ret, size/sizeof(WCHAR)-1);
			memcpy(dst, p, ret*sizeof(WCHAR));
			
			delete[] p;
			return ret;
		}
		return 0;
	}
}

//将UNICODE转换成ANSI，返回转换的字符个数，包含结束符
int		STD::ConvertToAnsi(char* dst, int size, const WCHAR* src, int len)
{
	memset(dst, 0, size);
	if (len == -1)
	{
		len	= wcslen(src);
	}
	return WideCharToMultiByte(CP_ACP, 0, src, len, dst, size-1, 0, 0);
}

//将UNICODE转换成ANSI，返回转换的字符个数，包含结束符，但原数据类别为char，注意对齐方式,len指明src的字节个数
int		STD::ConvertToAnsi(char* dst, int size, const char* src, int len)
{
	memset(dst, 0, size);
	
	WCHAR	buffer[2048+1];
	
	memset(buffer, 0, sizeof(buffer));
	
	if (len < (INT)sizeof(buffer) && len > 0)
	{
		memcpy(buffer, src, len);
		len		/= sizeof(WCHAR);
		return WideCharToMultiByte(CP_ACP, 0, buffer, len, dst, size, 0, 0);
	}
	else
	{
		WCHAR* p = (WCHAR*)calloc(len+2, 1);
		//WCHAR* p = new WCHAR[len/sizeof(WCHAR)+1];
		if (p)
		{
			//memset(p, 0, len+sizeof(WCHAR));
			memcpy(p, src, len);
			len		/= sizeof(WCHAR);
			int ret = WideCharToMultiByte(CP_ACP, 0, p, len, dst, size-1, 0, 0);
			
			//delete[] p;
			free(p);
			
			return ret;
		}
		
		return 0;
	}
}

char*	STD::strreplace(char *src, int len, const char *oldstr, const char *newstr)
{
	if (!oldstr || *oldstr == 0)
	{
		return src;
	}

	TListBuffer	listbuff(1024, sizeof(char), 1024);

	int oldstrlen = strlen(oldstr);
	int newstrlen = strlen(newstr);
	char* start = src;
	char* p = src;
	while (1)
	{
		p	= strstr(start, oldstr);
		if (p)
		{
			//*p = 0;
			listbuff.Add(start, p-start);
			listbuff.Add(newstr, newstrlen);
			start	= p + oldstrlen;
		}
		else
		{
			listbuff.Add(start, strlen(start));
			break;
		}
	}
	
	listbuff.GetItem(0, src, len);
	return src;
}

long STD::GZCompress(char * src, int srclen, vector<char>& dst)
{
	int ret = 0;
	float len_f = (srclen + 12.0f) * 1.001f;
	int dstlen = (int)(len_f + 0.500001f);
	dst.resize(dstlen);
	while(true)
	{
		ret = compress2((Bytef*)&dst[0], (uLongf*)&dstlen, (Bytef*)src, srclen, 9);
		if(ret == Z_OK)
		{
			dst.resize(dstlen);
			break;
		}
		
		if (ret == Z_BUF_ERROR)
		{
			dstlen = 2 * dst.size();
			dst.resize(dstlen);
		}
		else
		{
			break;
		}
	}
	
	if (ret == Z_OK)
	{
		return 0;
	}
	
	return -1;
}

long STD::GZUncompress(char * src, int srclen, TListBuffer& dst)	//char
{
	dst.Clear();
	int ret = 0;
	int dstlen = srclen * 3;
	dst.SetBufferSize(dstlen+1);
	
	while(true)
	{
		ret = uncompress((BYTE*)dst.GetPtr(), (uLongf*)&dstlen, (Bytef*)src, srclen);
		if (ret == Z_OK)
		{
			dst.Add(dstlen);
			break;
		}
		
		if (ret == Z_BUF_ERROR)
		{
			dst.Clear();
			dstlen *= 2;
			if (dstlen > 1024*1024*10)
			{
				break;
			}
			dst.SetBufferSize(dstlen+1);
		}
		else
		{
			break;
		}
	}
	
	if (ret == Z_OK)
	{
		return 0;
	}
	
	return -1;
}

long	STD::GetVersionLong(const char* ver)
{
	//ver:V1.23 B456()
	int high = atof(ver+1) * 100 + 0.5;
	const char* p = strchr(ver, 'B');
	int low = 0;
	if (p)
	{
		low = atoi(p+1);
	}
	return high << 16 | low;
}

char*	STD::MobilePhoneCharReplace(const char* src, char* dst)
{
	strcpy(dst, src);
	if (*src == '1' && strlen(src) >= 7)
	{
		memset(dst + 3, '*', 4);
	}
	return dst;
}


//检测string字符串是否在list中，假设string、list已经都是小写了
bool	STD::isInList(const char* string, const char* list, char ch)
{
	int string_len = strlen(string);
	int list_len = strlen(list);
	if (string_len > list_len)
	{
		return false;
	}

	if (string_len == list_len)
	{
		return strcmp(string, list) == 0;
	}

	if (strncmp(string, list, string_len) == 0 && list[string_len] == ch)
	{
		return true;
	}
	char temp[1024];
	_snprintf(temp, sizeof(temp), "%c%s%c", ch, string, ch);
	string_len++;
	if (strncmp(temp, list + list_len - string_len, string_len) == 0)
	{
		return true;
	}
	string_len++;
	if (strstr(list, temp))
	{
		return true;
	}
	return false;
}

//将ANSI转换成UNICODE，返回转换后的字符串，注意使用者需调用delete[]
WCHAR*	STD::ConvertToUnicode(char* src)
{
	int len = strlen(src);
	int needsize = MultiByteToWideChar (CP_ACP, 0, src, len, NULL, 0);
	if (needsize == 0)
	{
		return NULL;
	}
	WCHAR* pdata = new WCHAR[needsize+1];
	memset(pdata, 0, sizeof(WCHAR)*(needsize+1));
	MultiByteToWideChar (CP_ACP, 0, src, len, pdata, needsize);
	return pdata;
}

WCHAR*	STD::Utf8ToUnicode(const char* src)	//返回new
{
	int len = strlen(src);
	int needsize = MultiByteToWideChar (CP_UTF8, 0, src, len, NULL, 0);
	if (needsize == 0)
	{
		return NULL;
	}
	WCHAR* pdata = new WCHAR[needsize+1];
	memset(pdata, 0, sizeof(WCHAR)*(needsize+1));
	MultiByteToWideChar (CP_UTF8, 0, src, len, pdata, needsize);
	return pdata;
}

WCHAR*	STD::ReadFile(const char* file)
{
	FILE* fp = fopen(file, "rb");
	if (fp)
	{
		int filesize = filelength(fileno(fp));
		if (filesize > 0 && filesize < 1024*1024)
		{
			int newsize = filesize + (filesize&1) + 100;
			char* pdata = new char[newsize];
			if (pdata)
			{
				memset(pdata, 0, newsize);
				fseek(fp, 0, SEEK_SET);
				fread(pdata, 1, filesize, fp);

				fclose(fp);
				
				int sign = *(int*)pdata;
				if ((sign&0xffffff) == 0xbfbbef)	//UTF-8
				{
					WCHAR* ret = Utf8ToUnicode(pdata);
					delete[] pdata;
					return ret;
				}
				else if ((sign&0xffff) == 0xfeff)	//unicode
				{
					return (WCHAR*)pdata;
				}
				else	//ansi
				{
					WCHAR* ret = ConvertToUnicode(pdata);
					delete[] pdata;
					return ret;
				}
			}
		}
		fclose(fp);
	}

	return NULL;
}

