#ifndef CIPFORWARDINFO_H_H_
#define CIPFORWARDINFO_H_H_
#include "inc.h"
#include "CIpNetTypeConvert.h"
namespace HongLib
{
	class CIpForwardInfo:private CIpNetTypeConvert
	{
	private:
		PMIB_IPFORWARDROW m_pIpForwInfo;
	public:
		CIpForwardInfo(PMIB_IPFORWARDROW pMibForRow)
		{
			ASSERT(NULL != pMibForRow)
				this->m_pIpForwInfo = pMibForRow;
		}
		~CIpForwardInfo()
		{
		}
		inline DWORD GetForDestD() const;
		inline TCHAR* GetForDestT(DWORD* pdwSize = NULL) const;
		inline DWORD GetForMaskD() const;
		inline TCHAR* GetForMaskT(DWORD* pdwSize = NULL) const;
		inline DWORD GetForPolicy() const;
		inline DWORD GetForNextHopD() const;
		inline TCHAR* GetForNextHopT(DWORD* pdwSize = NULL) const;
		inline DWORD GetForIfIndex() const;
		inline DWORD GetForType() const;
		inline DWORD GetForProto() const;
		inline DWORD GetForAge() const;
		inline DWORD GetForNextHopAS() const;
		inline DWORD GetForMetric1() const;
		inline DWORD GetForMetric2() const;
		inline DWORD GetForMetric3() const;
		inline DWORD GetForMetric4() const;
		inline DWORD GetForMetric5() const;

	};

	DWORD CIpForwardInfo::GetForDestD() const
	{
		return this->m_pIpForwInfo->dwForwardDest;
	}


	TCHAR* CIpForwardInfo::GetForDestT(DWORD* pdwSize) const
	{
		return this->DWORDToAddr(this->m_pIpForwInfo->dwForwardDest,pdwSize);
	}



	DWORD CIpForwardInfo::GetForMaskD() const
	{
		return this->m_pIpForwInfo->dwForwardMask;
	}



	TCHAR* CIpForwardInfo::GetForMaskT(DWORD* pdwSize) const
	{
		return this->DWORDToAddr(this->m_pIpForwInfo->dwForwardMask,pdwSize);
	}



	DWORD CIpForwardInfo::GetForPolicy() const
	{
		return this->m_pIpForwInfo->dwForwardPolicy;
	}



	DWORD CIpForwardInfo::GetForNextHopD() const
	{
		return this->m_pIpForwInfo->dwForwardNextHop;
	}



	TCHAR* CIpForwardInfo::GetForNextHopT(DWORD* pdwSize) const
	{
		return this->DWORDToAddr(this->m_pIpForwInfo->dwForwardNextHop,pdwSize);
	}



	DWORD CIpForwardInfo::GetForIfIndex() const
	{
		return this->m_pIpForwInfo->dwForwardIfIndex;
	}



	DWORD CIpForwardInfo::GetForType() const
	{
		return this->m_pIpForwInfo->dwForwardType;
	}



	DWORD CIpForwardInfo::GetForProto() const
	{
		return this->m_pIpForwInfo->dwForwardProto;
	}



	DWORD CIpForwardInfo::GetForAge() const
	{
		return this->m_pIpForwInfo->dwForwardAge;
	}



	DWORD CIpForwardInfo::GetForNextHopAS() const
	{
		return this->m_pIpForwInfo->dwForwardNextHopAS;
	}



	DWORD CIpForwardInfo::GetForMetric1() const
	{
		return this->m_pIpForwInfo->dwForwardMetric1;
	}



	DWORD CIpForwardInfo::GetForMetric2() const
	{
		return this->m_pIpForwInfo->dwForwardMetric2;
	}



	DWORD CIpForwardInfo::GetForMetric3() const
	{
		return this->m_pIpForwInfo->dwForwardMetric3;
	}



	DWORD CIpForwardInfo::GetForMetric4() const
	{
		return this->m_pIpForwInfo->dwForwardMetric4;
	}



	DWORD CIpForwardInfo::GetForMetric5() const
	{
		return this->m_pIpForwInfo->dwForwardMetric5;
	}
}
#endif