#ifndef CBASEEXCEPTION_H_H_
#define CBASEEXCEPTION_H_H_
/****************************************************
this is exception class 
author:BigHead
date:2010.12.21
***************************************************/
class CBaseException
{
public:
	CBaseException()
	{
		this->m_nExpId = 0;
	}
	CBaseException(const long& id)
	{
		this->m_nExpId = id;
	}
	CBaseException(char* message)
	{
		this->m_strExpMsg = message;
		this->m_nExpId = 0;
	}
	CBaseException(const long& id,char* message)
	{
		this->m_nExpId = id;
        this->m_strExpMsg = message;
	}
	virtual ~CBaseException()
	{
	}
	char* GetExpMsg() const
	{
		return this->m_strExpMsg;
	}
	long GetExpId() const
	{
		return this->m_nExpId;
	}
private:
	int m_nExpId;
	char* m_strExpMsg;
};
#endif