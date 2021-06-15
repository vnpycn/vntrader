#ifndef CNOTRACKOBJECT_H_H_
#define CNOTRACKOBJECT_H_H_
#include "CWindowsException.h"
class CNoTrackObject
{
public:
	void* operator new(size_t nSize)
	{
        
		void* p = ::GlobalAlloc(GPTR,nSize);
		if(NULL == p)
		{
			DWORD nError = ::GetLastError();
			CWindowsException(nError,"GlobalAlloc error");
		}
	    return p;
	}
	void operator delete(void*p)
	{
			if(NULL != p)
		if(NULL != ::GlobalFree(p))
		{
			DWORD nError = ::GetLastError();
			CWindowsException(nError,"GlobalFree error");
		}
	}
	virtual ~CNoTrackObject(){};
};
#endif