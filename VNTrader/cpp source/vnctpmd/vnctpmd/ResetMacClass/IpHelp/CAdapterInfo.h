#ifndef CADAPTERINFO_H_H_
#define CADAPTERINFO_H_H_
#include "inc.h"
#include "CIpNetTypeConvert.h"
namespace HongLib
{
	class CAdapterInfo:private CIpNetTypeConvert
	{
	private:
		PMIB_IFROW m_pIfRow;

	public:
		CAdapterInfo(PMIB_IFROW pIfRow)
		{
			ASSERT(NULL != pIfRow)
				this->m_pIfRow = pIfRow;
		}
		CAdapterInfo(const CAdapterInfo& other)
		{
			this->m_pIfRow = other.m_pIfRow;
		}
		~CAdapterInfo()
		{
		}

		inline TCHAR* GetAdpNameT(DWORD* pdwSize = NULL) const;
		inline DWORD GetAdapterIndex() const;
		inline DWORD GetTypeD() const;
		inline TCHAR* GetTypeT(DWORD* pdwSize = NULL) const;
		inline DWORD GetSpeed() const;
		inline BYTE* GetPhyAddressB(DWORD* pdwSize = NULL) const;
		inline TCHAR* GetPhyAddressT(DWORD* pdwSize = NULL) const;
		inline DWORD GetAdminStatue() const;
		inline DWORD GetOperateStateD() const;
		inline TCHAR* GetOperateStateT(DWORD* pdwSize = NULL) const;
		inline TCHAR* GetDescriptionT(DWORD* pdwSize = NULL) const;
		inline BYTE* GetDescriptionB(DWORD* pdwSize = NULL) const;


	};




	/********************
	功能：得到适配器名称
	前置条件：无
	参数：
	pdwSize 适配器名字大小（字符数）

	返回值：
	成功：返回适配器名称内存指针
	失败：NULL
	说明：需要自己释放内存释放内存

	********************/
	TCHAR* CAdapterInfo::GetAdpNameT(DWORD* pdwSize) const
	{
		return this->WideCharToTCHAR(this->m_pIfRow->wszName,pdwSize);
	}
	/*********************
	功能：获取网卡索引
	参数：无
	返回值：网卡索引
	*********************/
	DWORD CAdapterInfo::GetAdapterIndex() const
	{
		return this->m_pIfRow->dwIndex;
	}
	/*********************
	功能：获取网卡类型
	参数：无
	返回值：网卡类型
	MIB_IF_TYPE_OTHER
	MIB_IF_TYPE_ETHERNET
	MIB_IF_TYPE_TOKENRING
	MIB_IF_TYPE_FDDI
	MIB_IF_TYPE_PPP
	MIB_IF_TYPE_LOOPBACK
	MIB_IF_TYPE_SLIP
	*********************/
	DWORD CAdapterInfo::GetTypeD() const
	{
		return this->m_pIfRow->dwType;
	}

	TCHAR* CAdapterInfo::GetTypeT(DWORD* pdwSize) const
	{
		TCHAR* tcTmpType;
		DWORD dwTmpSize;
		switch(this->m_pIfRow->dwType)
		{
		case MIB_IF_TYPE_OTHER:
			dwTmpSize = sizeof(TEXT("MIB_IF_TYPE_OTHER"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_TYPE_OTHER"),dwTmpSize);
			break;
		case MIB_IF_TYPE_ETHERNET:
			dwTmpSize = sizeof(TEXT("MIB_IF_TYPE_ETHERNET"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_TYPE_ETHERNET"),dwTmpSize);
			break;
		case MIB_IF_TYPE_TOKENRING:
			dwTmpSize = sizeof(TEXT("MIB_IF_TYPE_TOKENRING"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_TYPE_TOKENRING"),dwTmpSize);
			break;
		case MIB_IF_TYPE_FDDI:
			dwTmpSize = sizeof(TEXT("MIB_IF_TYPE_FDDI"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_TYPE_FDDI"),dwTmpSize);
			break;
		case MIB_IF_TYPE_PPP:
			dwTmpSize = sizeof(TEXT("MIB_IF_TYPE_PPP"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_TYPE_PPP"),dwTmpSize);
			break;
		case MIB_IF_TYPE_LOOPBACK:
			dwTmpSize = sizeof(TEXT("MIB_IF_TYPE_LOOPBACK"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_TYPE_LOOPBACK"),dwTmpSize);
			break;
		case MIB_IF_TYPE_SLIP:
			dwTmpSize = sizeof(TEXT("MIB_IF_TYPE_SLIP"));
			tcTmpType = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_TYPE_SLIP"),dwTmpSize);
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
	/*************
	function:Get Specifies the speed of the interface in bits per second
	parameter:none
	return:the speed of the interface in bits per second
	*************/
	DWORD CAdapterInfo::GetSpeed() const
	{
		return this->m_pIfRow->dwSpeed;
	}
	/***************
	function:Get Specifies the physical address of the adapter for this interface
	other:the same as above
	***************/
	BYTE* CAdapterInfo::GetPhyAddressB(DWORD* pdwSize) const
	{
		if(NULL != pdwSize)
		{
			*pdwSize = this->m_pIfRow->dwPhysAddrLen;
		}
		return (BYTE*)(this->IPHpCopyMemory(this->m_pIfRow->bPhysAddr,this->m_pIfRow->dwPhysAddrLen));
	}
	/***************
	function:Get Specifies the physical address of the adapter for this interface
	other:the same as above
	***************/
	TCHAR* CAdapterInfo::GetPhyAddressT(DWORD* pdwSize) const
	{
		return this->PhyNameBeToTCHAR(this->m_pIfRow->bPhysAddr,this->m_pIfRow->dwPhysAddrLen,pdwSize);
	}
	/*****************
	function:Get Specifies the interface is administratively enabled or disabled.
	parameter:No
	return:the interface is administratively enabled or disabled.
	******************/

	DWORD CAdapterInfo::GetAdminStatue() const
	{
		return this->m_pIfRow->dwAdminStatus;
	}
	/*******************
	function:Get Specifies the operational status of the interface. This member can be one of the following values.
	other:the same as above
	return :
	MIB_IF_OPER_STATUS_NON_OPERATIONAL LAN adapter has been disabled, for example because of an address conflict. 
	MIB_IF_OPER_STATUS_UNREACHABLE WAN adapter that is not connected. 
	MIB_IF_OPER_STATUS_DISCONNECTED For LAN adapters: network cable disconnected. For WAN adapters: no carrier. 
	MIB_IF_OPER_STATUS_CONNECTING WAN adapter that is in the process of connecting. 
	MIB_IF_OPER_STATUS_CONNECTED WAN adapter that is connected to a remote peer. 
	MIB_IF_OPER_STATUS_OPERATIONAL Default status for LAN adapters 

	*******************/
	DWORD CAdapterInfo::GetOperateStateD() const
	{
		return this->m_pIfRow->dwOperStatus;
	}
	TCHAR* CAdapterInfo::GetOperateStateT(DWORD* pdwSize) const
	{
		TCHAR* tcTmpOper;
		DWORD dwTmpSize;
		switch(this->m_pIfRow->dwOperStatus)
		{
		case MIB_IF_OPER_STATUS_NON_OPERATIONAL:
			dwTmpSize = sizeof(TEXT("MIB_IF_OPER_STATUS_NON_OPERATIONAL"));
			tcTmpOper = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_OPER_STATUS_NON_OPERATIONAL"),dwTmpSize);
			break;
		case MIB_IF_OPER_STATUS_UNREACHABLE:
			dwTmpSize = sizeof(TEXT("MIB_IF_OPER_STATUS_UNREACHABLE"));
			tcTmpOper = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_OPER_STATUS_UNREACHABLE"),dwTmpSize);
			break;
		case MIB_IF_OPER_STATUS_DISCONNECTED:
			dwTmpSize = sizeof(TEXT("MIB_IF_OPER_STATUS_DISCONNECTED"));
			tcTmpOper = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_OPER_STATUS_DISCONNECTED"),dwTmpSize);
			break;
		case MIB_IF_OPER_STATUS_CONNECTING:
			dwTmpSize = sizeof(TEXT("MIB_IF_OPER_STATUS_CONNECTING"));
			tcTmpOper = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_OPER_STATUS_CONNECTING"),dwTmpSize);
			break;
		case MIB_IF_OPER_STATUS_CONNECTED:
			dwTmpSize = sizeof(TEXT("MIB_IF_OPER_STATUS_CONNECTED"));
			tcTmpOper = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_OPER_STATUS_CONNECTED"),dwTmpSize);
			break;
		case MIB_IF_OPER_STATUS_OPERATIONAL:
			dwTmpSize = sizeof(TEXT("MIB_IF_OPER_STATUS_OPERATIONAL"));
			tcTmpOper = (TCHAR*)this->IPHpCopyMemory(TEXT("MIB_IF_OPER_STATUS_OPERATIONAL"),dwTmpSize);
			break;
		default:
			dwTmpSize = sizeof(TEXT("UNKNOWN_OPER_TYPE"));
			tcTmpOper = (TCHAR*)this->IPHpCopyMemory(TEXT("UNKNOWN_OPER_TYPE"),dwTmpSize);
			break;
		}
		if(NULL != pdwSize)
			*pdwSize = dwTmpSize;
		return tcTmpOper;
	}
	/*****************
	function:Get Contains a description of the interface（TCHAR*)
	other:the same as above
	****************/
	TCHAR* CAdapterInfo::GetDescriptionT(DWORD* pdwSize) const
	{
		return this->ByteToTCHAR(this->m_pIfRow->bDescr,this->m_pIfRow->dwDescrLen,pdwSize);
	}
	/*****************
	function:Get Contains a description of the interface(BYTE)
	other:the same as above
	****************/
	BYTE* CAdapterInfo::GetDescriptionB(DWORD* pdwSize) const
	{
		if(NULL != pdwSize)
		{
			*pdwSize = this->m_pIfRow->dwDescrLen;
		}
		return (BYTE*)this->IPHpCopyMemory((void*)this->m_pIfRow->bDescr,this->m_pIfRow->dwDescrLen);
	}
}
#endif