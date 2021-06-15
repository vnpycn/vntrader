#ifndef CIPADDRESSINFO_H_H_
#define CIPADDRESSINFO_H_H_
#include "inc.h"
#include "CIpNetTypeConvert.h"
namespace HongLib
{
	class CIpAddressInfo:private CIpNetTypeConvert
	{
	private:
		PMIB_IPADDRROW m_pIpAddr;
	public:
		CIpAddressInfo(PMIB_IPADDRROW pAddRow)
		{
			ASSERT(NULL != pAddRow)
				this->m_pIpAddr = pAddRow;
		}
		~CIpAddressInfo()
		{
		}
	inline DWORD GetIpAddress() const;
	inline TCHAR* GetIpAddressT(DWORD* pdwSize = NULL) const;
	inline DWORD GetMaskD() const;
	inline TCHAR* GetMaskT(DWORD* pdwSize = NULL) const;
	inline DWORD GetAdapterIndex() const;
	inline unsigned short GetIpAddrTypeUS() const;
	inline TCHAR* GetIpAddrTypeT(DWORD* pdwSize = NULL) const;
	inline DWORD GetBCastAddrD() const;
	inline TCHAR* GetBCastAddrT(DWORD* pdwSize = NULL) const;
	inline DWORD GetReasmSize() const;
	};
	DWORD CIpAddressInfo::GetIpAddress() const
	{
		return this->m_pIpAddr->dwAddr;
	}
	TCHAR* CIpAddressInfo::GetIpAddressT(DWORD* pdwSize) const
	{
		return this->DWORDToAddr(this->m_pIpAddr->dwAddr,pdwSize);
	}
	DWORD CIpAddressInfo::GetMaskD() const
	{
		return this->m_pIpAddr->dwMask;
	}
	TCHAR* CIpAddressInfo::GetMaskT(DWORD* pdwSize) const
	{
		return this->DWORDToAddr(this->m_pIpAddr->dwMask,pdwSize);
	}
	DWORD CIpAddressInfo::GetAdapterIndex() const
	{
		return this->m_pIpAddr->dwIndex;
	}
	unsigned short CIpAddressInfo::GetIpAddrTypeUS() const
	{
		return this->m_pIpAddr->wType;
	}
	TCHAR* CIpAddressInfo::GetIpAddrTypeT(DWORD* pdwSize) const
	{
		TCHAR* tcTmpType;
		DWORD dwTmpSize;
		switch(this->m_pIpAddr->wType)
		{
		case MIB_IPADDR_PRIMARY:
			dwTmpSize = sizeof(TEXT("MIB_IPADDR_PRIMARY"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IPADDR_PRIMARY"),dwTmpSize);
			break;
		case MIB_IPADDR_DYNAMIC:
			dwTmpSize = sizeof(TEXT("MIB_IPADDR_PRIMARY"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IPADDR_PRIMARY"),dwTmpSize);
			break;
		case MIB_IPADDR_DISCONNECTED:
			dwTmpSize = sizeof(TEXT("MIB_IPADDR_DISCONNECTED"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IPADDR_DISCONNECTED"),dwTmpSize);
			break;
		case MIB_IPADDR_DELETED:
			dwTmpSize = sizeof(TEXT("MIB_IPADDR_DELETED"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IPADDR_DELETED"),dwTmpSize);
			break;
		case MIB_IPADDR_TRANSIENT:
			dwTmpSize = sizeof(TEXT("MIB_IPADDR_TRANSIENT"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IPADDR_TRANSIENT"),dwTmpSize);
			break;
		default:
			dwTmpSize = sizeof(TEXT("UNKNOWN_TYPE"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("UNKNOWN_TYPE"),dwTmpSize);
			break;
		}
		if(NULL != pdwSize)
			*pdwSize = dwTmpSize;
		return tcTmpType;
	}
	DWORD CIpAddressInfo::GetBCastAddrD() const
	{
		return this->m_pIpAddr->dwBCastAddr;
	}
	TCHAR* CIpAddressInfo::GetBCastAddrT(DWORD* pdwSize) const
	{
		return this->DWORDToAddr(this->m_pIpAddr->dwBCastAddr,pdwSize);
	}
	DWORD CIpAddressInfo::GetReasmSize() const
	{
		return this->m_pIpAddr->dwReasmSize;
	}
}
#endif