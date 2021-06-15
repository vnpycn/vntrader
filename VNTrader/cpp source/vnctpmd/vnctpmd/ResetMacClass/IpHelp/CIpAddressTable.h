#ifndef CIPADDRESSTABLE_H_H_
#define CIPADDRESSTABLE_H_H_
#include "CIpAddressInfo.h"
namespace HongLib
{
	class CIpAddressTable:CIpNetTypeConvert
	{
	private:
		PMIB_IPADDRTABLE m_pIpAddrTable;
		DWORD m_dwIpAddrBufferSize;
	public:
		CIpAddressTable()
		{
			this->m_pIpAddrTable = NULL;
			this->m_dwIpAddrBufferSize = 0;
		}
		CIpAddressTable(const CIpAddressTable& other)
		{
			this->m_pIpAddrTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR,other.m_dwIpAddrBufferSize);
			memcpy(this->m_pIpAddrTable,other.m_pIpAddrTable,other.m_dwIpAddrBufferSize);
			this->m_dwIpAddrBufferSize = other.m_dwIpAddrBufferSize;

		}
		~CIpAddressTable()
		{
			this->Clear();
		}
		
		void Clear()
		{
			::GlobalFree(this->m_pIpAddrTable);
			this->m_dwIpAddrBufferSize = 0;
		}


		inline DWORD Update(PMIB_IPADDRTABLE* ppIpAddrTable = NULL);
		inline DWORD GetNumber() const;
		inline DWORD AddIpAddress(const DWORD& dwIndex,const TCHAR* ptcIp,const TCHAR* ptcMask,PULONG NTEContext = NULL,PULONG NTEInstance = NULL);
		inline DWORD DeleteIpAddress(ULONG NTEContext);
        inline CIpAddressInfo* EnumIpAddressInfoObject(const DWORD& dwEnumIndex) const; 
		inline CIpAddressInfo* GetIpAddressInfoObject(const DWORD& dwAdapterIndex,const DWORD& dwCout) const;
        inline DWORD GetSpecIndexNum(const DWORD& dwAdapterIndex) const;

	};
	DWORD CIpAddressTable::Update(PMIB_IPADDRTABLE* ppIpAddrTable)
	{

		if(ERROR_INSUFFICIENT_BUFFER == ::GetIpAddrTable(this->m_pIpAddrTable,&this->m_dwIpAddrBufferSize,FALSE))
		{
			::GlobalFree(this->m_pIpAddrTable);
			this->m_pIpAddrTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR,this->m_dwIpAddrBufferSize);
		}
		DWORD nRet;
		nRet = ::GetIpAddrTable(this->m_pIpAddrTable,&this->m_dwIpAddrBufferSize,FALSE);
		if(NULL != ppIpAddrTable)
		{
			*ppIpAddrTable = this->m_pIpAddrTable;
		}
		return nRet;
	}
	DWORD CIpAddressTable::GetNumber() const
	{
		ASSERT(NULL != this->m_pIpAddrTable)
			return this->m_pIpAddrTable->dwNumEntries;
	}
	DWORD CIpAddressTable::AddIpAddress(const DWORD& dwIndex,const TCHAR* ptcIp,const TCHAR* ptcMask,PULONG NTEContext,PULONG NTEInstance)
	{
		ULONG uiTmpIp = this->ADDRToULONG(ptcIp);
		ULONG uiTmpMask = this->ADDRToULONG(ptcMask);
		ULONG ulTmpCon = 0;
		ULONG ulTmpInt = 0;
		DWORD dwRet;
		dwRet = ::AddIPAddress(uiTmpIp,uiTmpMask,dwIndex,&ulTmpCon,&ulTmpInt);

		if(NULL != NTEContext)
			*NTEContext = ulTmpCon;

		if(NULL != NTEInstance)
			*NTEInstance = ulTmpInt;

		return dwRet;
	}

	DWORD CIpAddressTable::DeleteIpAddress(ULONG NTEContext)
	{
		return ::DeleteIPAddress(NTEContext);
	}
	CIpAddressInfo* CIpAddressTable::GetIpAddressInfoObject(const DWORD& dwAdapterIndex,const DWORD& dwCout) const
	{
       DWORD nTmpCout = 0;
		for(DWORD i=0; i<this->m_pIpAddrTable->dwNumEntries;i++)
		{
			if(dwAdapterIndex == this->m_pIpAddrTable->table[i].dwIndex)
			{
                
				if(nTmpCout == dwCout)
				return new CIpAddressInfo(&this->m_pIpAddrTable->table[i]);

				nTmpCout++;
			}
		}
		return NULL;
	}
	CIpAddressInfo* CIpAddressTable::EnumIpAddressInfoObject(const DWORD& dwEnumIndex) const
	{
		if(dwEnumIndex < this->m_pIpAddrTable->dwNumEntries)
			{
				return new CIpAddressInfo(&this->m_pIpAddrTable->table[dwEnumIndex]);
			}
			return NULL;
	}
	 DWORD CIpAddressTable::GetSpecIndexNum(const DWORD& dwAdapterIndex) const
	 {
        DWORD nTmpCout = 0;
		for(DWORD i=0; i<this->m_pIpAddrTable->dwNumEntries;i++)
		{
			if(dwAdapterIndex == this->m_pIpAddrTable->table[i].dwIndex)
			{
				nTmpCout++;
			}
		}
		return nTmpCout;
	 }
}
#endif