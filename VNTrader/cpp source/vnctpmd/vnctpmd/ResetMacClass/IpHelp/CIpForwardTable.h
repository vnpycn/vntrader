#ifndef CIPFORWARDTABLE_H_H_
#define CIPFORWARDTABLE_H_H_
#include "CIpForwardInfo.h"
namespace HongLib
{
	class CIpForwardTable
	{
	private:
		PMIB_IPFORWARDTABLE m_pIpForwardTable;
		DWORD m_dwIpForwardBufferSize;
	public:
		CIpForwardTable()
		{
			this->m_pIpForwardTable = NULL;
			this->m_dwIpForwardBufferSize = 0;
		}
		CIpForwardTable(const CIpForwardTable& other)
		{
			this->m_pIpForwardTable = (PMIB_IPFORWARDTABLE)::GlobalAlloc(GPTR,other.m_dwIpForwardBufferSize);
			memcpy(this->m_pIpForwardTable,other.m_pIpForwardTable,other.m_dwIpForwardBufferSize);
			this->m_dwIpForwardBufferSize = other.m_dwIpForwardBufferSize;
		}
		~CIpForwardTable()
		{
			this->Clear();
		}
		inline	DWORD Update(PMIB_IPFORWARDTABLE* ppIpForward = NULL);
		inline	DWORD GetNumber() const;
		inline	DWORD AddForwardEntry(PMIB_IPFORWARDROW& pRoute);
		inline	DWORD EditIpForwardEntry(PMIB_IPFORWARDROW& pRoute);
		inline 	DWORD DeleteForwardEntry(PMIB_IPFORWARDROW& pRoute);
		inline CIpForwardInfo* EnumIpAddressInfoObject(const DWORD& dwEnumIndex) const; 
		inline CIpForwardInfo* GetIpForwardInfoObject(const DWORD& dwAdapterIndex,const DWORD& dwCout) const;
		inline DWORD GetSpecIndexNum(const DWORD& dwAdapterIndex) const;
		void Clear()
		{
			::GlobalFree(this->m_pIpForwardTable);
			this->m_pIpForwardTable = NULL;
			this->m_dwIpForwardBufferSize = 0;
		}
	};

	DWORD CIpForwardTable::Update(PMIB_IPFORWARDTABLE* ppIpForward)
	{
		DWORD dwSize = 0;

		if(ERROR_INSUFFICIENT_BUFFER == ::GetIpForwardTable(this->m_pIpForwardTable,&this->m_dwIpForwardBufferSize,FALSE))
			{
				::GlobalFree(this->m_pIpForwardTable);
				this->m_pIpForwardTable = (PMIB_IPFORWARDTABLE)::GlobalAlloc(GPTR,this->m_dwIpForwardBufferSize);
			}
			DWORD dwRet;
			dwRet = ::GetIpForwardTable(this->m_pIpForwardTable,&this->m_dwIpForwardBufferSize,FALSE);
			if(NULL != ppIpForward)
			{
				*ppIpForward = this->m_pIpForwardTable;
			}
			return dwRet;
	}
	DWORD CIpForwardTable::GetNumber() const
	{
		ASSERT(NULL != this->m_pIpForwardTable)
			return this->m_pIpForwardTable->dwNumEntries;
	}
	DWORD CIpForwardTable::AddForwardEntry(PMIB_IPFORWARDROW& pRoute)
	{
		return ::CreateIpForwardEntry(pRoute);
	}
	DWORD CIpForwardTable::EditIpForwardEntry(PMIB_IPFORWARDROW& pRoute)
	{
		return ::SetIpForwardEntry(pRoute);
	}
	DWORD CIpForwardTable::DeleteForwardEntry(PMIB_IPFORWARDROW& pRoute)
	{
		return ::DeleteIpForwardEntry(pRoute);
	}
	CIpForwardInfo* CIpForwardTable::EnumIpAddressInfoObject(const DWORD& dwEnumIndex) const
	{
		if(dwEnumIndex < this->m_pIpForwardTable->dwNumEntries)
			{
				return new CIpForwardInfo(&this->m_pIpForwardTable->table[dwEnumIndex]);
			}
			return NULL;
	}
	CIpForwardInfo* CIpForwardTable::GetIpForwardInfoObject(const DWORD& dwAdapterIndex,const DWORD& dwCout) const
	{
		  DWORD nTmpCout = 0;
		  for(DWORD i=0; i<this->m_pIpForwardTable->dwNumEntries;i++)
		{
			if(dwAdapterIndex == this->m_pIpForwardTable->table[i].dwForwardIfIndex)
			{
                
				if(nTmpCout == dwCout)
					return new CIpForwardInfo(&this->m_pIpForwardTable->table[i]);

				nTmpCout++;
			}
		}
		return NULL;
	}
	DWORD CIpForwardTable::GetSpecIndexNum(const DWORD& dwAdapterIndex) const
	{
		 DWORD nTmpCout = 0;
		 for(DWORD i=0; i<this->m_pIpForwardTable->dwNumEntries;i++)
		{
			if(dwAdapterIndex == this->m_pIpForwardTable->table[i].dwForwardIfIndex)
			{
				nTmpCout++;
			}
		}
		return nTmpCout;
	}
}
#endif