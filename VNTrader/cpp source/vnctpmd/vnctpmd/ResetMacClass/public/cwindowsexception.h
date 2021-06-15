#ifndef CWINDOWSEXCEPTION_H_H_
#define CWINDOWSEXCEPTION_H_H_
/****************************************************
this is windows exception class 
author:BigHead
date:2010.12.21
***************************************************/
class CWindowsException
{
public:
	CWindowsException()
	{
		m_strFormatMessage = NULL;
		m_sUserMessage = NULL;
		m_dwWindowsExpId = 0;
	}

	CWindowsException(TCHAR* message)
	{
		m_strFormatMessage = NULL;
		m_dwWindowsExpId = 0;
		m_sUserMessage = message;
	}
    CWindowsException(CWindowsException& obj)
	{
		m_strFormatMessage = obj.m_strFormatMessage;
		m_dwWindowsExpId = obj.m_dwWindowsExpId;
		m_sUserMessage = obj.m_sUserMessage;
	}
	CWindowsException(const DWORD& id)
	{
		m_strFormatMessage = NULL;
		m_dwWindowsExpId = id;
		m_sUserMessage =NULL;
	}
	CWindowsException(const DWORD& id,TCHAR* message)
	{
		m_strFormatMessage = NULL;
		m_dwWindowsExpId = id;
		m_sUserMessage = message;
	}
	void SetWindowsExpId(const DWORD& id)
	{
		this->m_dwWindowsExpId = id;
	}
	DWORD GetWindowsExpId() const
	{
		return this->m_dwWindowsExpId;
	}


    void FormatMessage();


	virtual ~CWindowsException()
	{
		::GlobalFree(this->m_strFormatMessage);
		this->m_strFormatMessage = NULL;
	}
	TCHAR* What()
	{
		if( (0 != this->m_dwWindowsExpId) && (NULL == this->m_sUserMessage) )
		{
			this->FormatMessage();
			return this->m_strFormatMessage;
		}
		else if( (0 == this->m_dwWindowsExpId) && (NULL != this->m_sUserMessage))
		{
			return (TCHAR*)this->m_sUserMessage;
		}
		else if( (0 != this->m_dwWindowsExpId) && (NULL != this->m_sUserMessage) )
		{
			this->FormatMessage();
			TCHAR* tmpFormatBuffer = this->m_strFormatMessage;
			DWORD dwMsgSize = lstrlen((TCHAR*)this->m_strFormatMessage);
			DWORD dwUsrMsgSize = lstrlen(this->m_sUserMessage);
			dwMsgSize = (dwUsrMsgSize+dwMsgSize+2+sizeof(",Error:") )*sizeof(TCHAR);
			this->m_strFormatMessage = (TCHAR*)::GlobalAlloc(GPTR,dwMsgSize);
			lstrcpy(this->m_strFormatMessage,this->m_sUserMessage);
			lstrcat(this->m_strFormatMessage,_T(",Error:"));
			lstrcat(this->m_strFormatMessage,tmpFormatBuffer);
			::GlobalFree(tmpFormatBuffer);
			return this->m_strFormatMessage;
        }
		else
		{
			return NULL;
		}
	}
private:
	TCHAR* m_sUserMessage;
	TCHAR* m_strFormatMessage;
	DWORD m_dwWindowsExpId;
};

void CWindowsException::FormatMessage()
{
	if(NULL != this->m_strFormatMessage)
	{
		::GlobalFree(this->m_strFormatMessage);
		this->m_strFormatMessage = NULL;
	}
    
	::FormatMessage(\
		FORMAT_MESSAGE_ALLOCATE_BUFFER | \
		FORMAT_MESSAGE_FROM_SYSTEM,\
		NULL,\
		this->m_dwWindowsExpId,\
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
		(LPTSTR)&this->m_strFormatMessage,\
		0, NULL );
}
#endif