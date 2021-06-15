#ifndef CCHANGEMAC_H_H_
#define CCHANGEMAC_H_H_

class CChangeMac
{
public:
	CChangeMac(){}
	CChangeMac(tstring& currentpath);
	void SetCurrentPath(const tstring& currentpath);
	unsigned char IsValidMAC(const tstring& mac) const;/////0为无效地址。1为12位地址。2为17位地址
	void SetMac(const tstring& mac) const;
	tstring GetMacFromRemote(TCHAR* tcIp) const;//////////格式为xxxxxxxxxxxx
	tstring CovertMACCanRead(const tstring& mac) const;
	void ReSetMac() const;
	bool IsValidCurrPath() const;
	tstring GetCurrentDesc() const;
	tstring GetCurrentMAC() const;
	tstring ParseMACAddr(const tstring& mac) const;/////将xx-xx-xx-xx-xx-xx转换为xxxxxxxxxxxx格式(智能判断)
	bool ReStartAdp() const;

private:
	CChangeMac(CChangeMac& object){}//////////不允许使用复制构造函数
	tstring m_sCurrentPath;
};
CChangeMac::CChangeMac(tstring& currentpath)
{
	this->m_sCurrentPath = currentpath;
}
void CChangeMac::SetCurrentPath(const tstring& currentpath)
{
	this->m_sCurrentPath = currentpath;
}
void CChangeMac::SetMac(const tstring& mac) const
{
	LONG lRet;
	ASSERT(true == IsValidCurrPath());

		tstring strTmpMac = this->ParseMACAddr(mac);
	ASSERT(strTmpMac.empty() == false);


	CRegKey Reg;
	lRet = Reg.Open(HKEY_LOCAL_MACHINE,this->m_sCurrentPath.c_str(),KEY_SET_VALUE);
	if(ERROR_SUCCESS != lRet)
	{
		CWindowsException exp(lRet);
		throw exp;
	}
	lRet = Reg.SetStringValue(TEXT("NetworkAddress"),strTmpMac.c_str());
	if(ERROR_SUCCESS != lRet)
	{
		Reg.Close();
		CWindowsException exp(lRet);
		throw exp;
	}
	Reg.Close();

}
void CChangeMac::ReSetMac() const
{
	LONG lRet;
	ASSERT(true == IsValidCurrPath());

	CRegKey Reg;
	lRet = Reg.Open(HKEY_LOCAL_MACHINE,this->m_sCurrentPath.c_str(),KEY_SET_VALUE);
	if(ERROR_SUCCESS != lRet)
	{
		CWindowsException exp(lRet,TEXT("ReSetMac:Reg.Open"));
		throw exp;
	}
	lRet = Reg.DeleteValue(TEXT("NetworkAddress"));
	if((ERROR_SUCCESS != lRet) && (lRet != 2))
	{
		Reg.Close();
		CWindowsException exp(lRet,TEXT("ReSetMac:Reg.DeleteValue"));
		throw exp;
	}
	Reg.Close();
}
bool CChangeMac::IsValidCurrPath() const
{
	if(this->m_sCurrentPath.empty())
		return false;
	return true;
}
tstring CChangeMac::GetCurrentDesc() const
{
	LONG lRet;
	ASSERT(true == IsValidCurrPath());

		CRegKey Reg;
	lRet = Reg.Open(HKEY_LOCAL_MACHINE,this->m_sCurrentPath.c_str(),KEY_QUERY_VALUE);
	if(ERROR_SUCCESS != lRet)
	{
		CWindowsException exp(lRet,TEXT("GetCurrentDesc:Reg.Open error"));
		throw exp;
	}
	DWORD dwDescCharCout = 0;
	lRet = Reg.QueryStringValue(TEXT("DriverDesc"),NULL,&dwDescCharCout);
	if(ERROR_SUCCESS != lRet)
	{
		Reg.Close();
		CWindowsException exp(lRet,TEXT("GetCurrentDesc:QueryStringValue1 error"));
		throw exp;
	}
	TCHAR* tcDescBuffer = new TCHAR[dwDescCharCout];

	lRet = Reg.QueryStringValue(TEXT("DriverDesc"),tcDescBuffer,&dwDescCharCout);
	if(ERROR_SUCCESS != lRet)
	{
		Reg.Close();
		delete[] tcDescBuffer;
		CWindowsException exp(lRet,TEXT("GetCurrentDesc:QueryStringValue2 error"));
		throw exp;
	}
	tstring sTmpDesc = tcDescBuffer;
	delete[] tcDescBuffer;
	Reg.Close();
	return sTmpDesc;


}
tstring CChangeMac::GetCurrentMAC() const
{
	LONG lRet;
	ASSERT(true == IsValidCurrPath());

	CRegKey Reg;
	lRet = Reg.Open(HKEY_LOCAL_MACHINE,this->m_sCurrentPath.c_str(),KEY_QUERY_VALUE);
	if(ERROR_SUCCESS != lRet)
	{
		CWindowsException exp(lRet);
		throw exp;
	}
	DWORD dwMacCharCout = 0;

	lRet = Reg.QueryStringValue(TEXT("NetworkAddress"),NULL,&dwMacCharCout);

	if(ERROR_SUCCESS != lRet)
	{
		Reg.Close();
		CWindowsException exp(lRet,TEXT("GetCurrentMAC:QueryStringValue1 error"));
		throw exp;
	}

	TCHAR* tcMacBuffer = new TCHAR[dwMacCharCout];
	lRet = Reg.QueryStringValue(TEXT("NetworkAddress"),tcMacBuffer,&dwMacCharCout);

	if(ERROR_SUCCESS != lRet)
	{
		delete[] tcMacBuffer;
		Reg.Close();
		CWindowsException exp(lRet,TEXT("GetCurrentMAC:QueryStringValue2 error"));
		throw exp;
	}
	tstring sTmpMac = tcMacBuffer;
	delete[] tcMacBuffer;
	Reg.Close();
	return sTmpMac;


}
unsigned char CChangeMac::IsValidMAC(const tstring& mac) const
{
	if(mac.size() == 17)
	{
		tstring::size_type i;
		unsigned char ucFlagNum = 0;
		for(i = 2;i<mac.size();i+=3)
		{
			if((TEXT('-') == mac[i]) || (TEXT(' ') == mac[i]))
			{
				ucFlagNum++;
			}
		}
		if(5 == ucFlagNum)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}
	else if(mac.size() == 12)
	{
		return 1;
	}
	return 0;
}
tstring CChangeMac::ParseMACAddr(const tstring& mac) const
{
	unsigned char unValidMAC = this->IsValidMAC(mac);

	if(1 == unValidMAC)
	{
		return mac;
	}
	else if(2 == unValidMAC)
	{
		tstring sRetMAC;
		TCHAR tcTmpBuffer[3] = {TEXT('\0')};
		tstring::size_type i;
		for(i = 0;i<mac.size();i+=3)
		{
			tcTmpBuffer[0] = mac[i];
			tcTmpBuffer[1] = mac[i+1];
			sRetMAC += tcTmpBuffer;
		}
		return sRetMAC;
	}
	else
	{

		CWindowsException exp(TEXT("MAC地址格式错误"));
		throw exp;
	}


}

tstring CChangeMac::GetMacFromRemote(TCHAR* tcIp) const//////////格式为xxxxxxxxxxxx
{
	IPAddr ipAddr;
#ifdef UNICODE
	char *tmpIp;
	HongLib::CConvertString cov;
	cov.WideCharToANSIAutoAlloc(tcIp,tmpIp);
	ipAddr = inet_addr(tmpIp);
	delete[] tmpIp;
#else
	ipAddr = inet_addr(tcIp);
#endif
	ULONG   pulMac[2];
	ULONG ulLen = 6;
	LONG lRet = ::SendARP(ipAddr,0,pulMac,&ulLen);
	if( (NO_ERROR != lRet) || (0 == ulLen) )
	{
		CWindowsException exp(lRet,TEXT("GetMacFromRemote SendARP error"));
		throw exp;
	}
	PBYTE pbHexMac = (PBYTE) pulMac;
	TCHAR* tmpMACBuffer = new TCHAR[13];
	lsprintf_s(tmpMACBuffer,13,TEXT("%02X%02X%02X%02X%02X%02X"),pbHexMac[0],pbHexMac[1],pbHexMac[2],pbHexMac[3],pbHexMac[4],pbHexMac[5]);
	tstring sTmpMac = tmpMACBuffer;
	delete tmpMACBuffer;
	return sTmpMac;

}
tstring CChangeMac::CovertMACCanRead(const tstring& mac) const
{

	if(mac.size() == 12)
	{
		tstring strTmp;
		tstring::size_type i;
		for(i = 0;i<mac.size();i+=2)
		{
			strTmp += mac.substr(i,2);

			if(i !=  (mac.size() - 2) )
			{
				strTmp += TEXT('-');
			}
		}
		return strTmp;
	}
	else
	{
		return mac;
	}
}
bool CChangeMac::ReStartAdp() const
{
	ASSERT(true == this->IsValidCurrPath());
		bool bRet = false;
	HongLib::CSetupComDi adp;
	adp.QueryFromDatabase(TEXT("4D36E972-E325-11CE-BFC1-08002bE10318"));//////////网卡的GUID值，所有WINDOWS系统上面是一样的
	for(PBYTE pb = adp.EnumSetupDi();pb != NULL;pb = adp.EnumSetupDi())
	{
		if(lstrcmp((TCHAR*)pb,(TCHAR*)this->GetCurrentDesc().c_str()) == 0)///////我网卡的名字（指定自己需要操作的网卡即可）
		{
			if(adp.DisableDev())
			{
				if(adp.EnableDev())
				{
					bRet = true;
				}
			}
		}
	}
	return bRet;
}
#endif