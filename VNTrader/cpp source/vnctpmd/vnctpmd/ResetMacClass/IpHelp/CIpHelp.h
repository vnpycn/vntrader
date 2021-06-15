#ifndef CIPHELP_H_H_
#define CIPHELP_H_H_
#include "CNetworkParam.h"
#include "CIpForwardTable.h"
#include "CIpAddressTable.h"
#include "CAdapterTable.h"
namespace HongLib
{
	class CIpHelp
	{
	public:
		inline CIpHelp()
		{
		}
		virtual ~CIpHelp()
		{
		}

		CIpForwardTable* CreateIpForwardTable()
		{
			return new CIpForwardTable();
		}
		CAdapterTable* CreateGetAdapterTable()
		{
			return new CAdapterTable();
		}
		CIpAddressTable* CreateIpAddressTable()
		{
			return new CIpAddressTable();
		}
		CNetworkParam* CreateNetWorkParam()
		{
			return new CNetworkParam();
		}
	};


}
#endif