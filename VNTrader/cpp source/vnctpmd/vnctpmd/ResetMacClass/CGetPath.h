#ifndef CGETADPPATH_H_H_
#define CGETADPPATH_H_H_
class CGetAdpPath
{
public:
	CGetAdpPath(){};
	int GetComEthnetAdapterPath();
	tstring GetCurrentPath() const;
	DWORD GetEthnetCout();
	tstring EnumEthnet(const DWORD& index);
	bool SetCurrentPath(const DWORD& index);
	void SetCurrentPath(const tstring& path);
    operator tstring();
	void ClearVector();
	~ CGetAdpPath();


private:
	vector<tstring> m_sEthnetAdpaterPath;/////////当前机器的所有以太网在注册表中的路径
	tstring m_sCurrentPath;////////////当前操作的一台网卡路径
	CGetAdpPath(CGetAdpPath& mac);////////不允许使用复制构造函数

};
void CGetAdpPath::ClearVector()
{
	this->m_sEthnetAdpaterPath.clear();
}
CGetAdpPath::operator tstring()
{
	return m_sCurrentPath;
}
int CGetAdpPath::GetComEthnetAdapterPath()
{
	CRegKey Reg;
	LONG lRet;

	TCHAR tcRoot[] = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002bE10318}");


	//printf("B1\n");
	//Sleep(5000);


lRet = Reg.Open(HKEY_LOCAL_MACHINE,tcRoot,KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE);
	if(ERROR_SUCCESS != lRet)
	{
		CWindowsException exp(lRet,TEXT("GetComEthnetAdapterPath:Reg.Open error"));
		throw exp;
	}

	//printf("B2\n");
	//Sleep(5000);


	HKEY hTmpKey = Reg;
	DWORD dwSubKeyNum = 0;
	DWORD dwSubKeyMaxLen = 0;
	lRet = ::RegQueryInfoKey(hTmpKey,NULL,NULL,0,&dwSubKeyNum,&dwSubKeyMaxLen,NULL,NULL,NULL,NULL,NULL,NULL);
	if(ERROR_SUCCESS != lRet)
		{
		   Reg.Close();
		   CWindowsException exp(lRet,TEXT("GetComEthnetAdapterPath:RegQueryInfoKey error"));
			throw exp;
		}

 

	DWORD dwSizeLen;
	DWORD dwEtnCout = 0;
	for(DWORD i=0;i<dwSubKeyNum;i++)
	{
		dwSizeLen = dwSubKeyMaxLen + 1;
		TCHAR *tcSubKeyBuffer = new TCHAR[dwSizeLen];
		CRegKey SubKey;
		lRet = Reg.EnumKey(i,tcSubKeyBuffer,&dwSizeLen);
		if(ERROR_SUCCESS != lRet)
		{
           delete[] tcSubKeyBuffer;
		   Reg.Close();
		   CWindowsException exp(lRet,TEXT("GetComEthnetAdapterPath:Reg.EnumKey error"));
			throw exp;
		}

 

		lRet = SubKey.Open(Reg,tcSubKeyBuffer,KEY_QUERY_VALUE);
 

		if(ERROR_SUCCESS != lRet)
		{
 
            delete[] tcSubKeyBuffer;
			SubKey.Close();
 
			CWindowsException exp(lRet,TEXT("GetComEthnetAdapterPath:SubKey.Open error"));
 
			Reg.Close();

			//throw exp;  ////////////////////////////////////////////////////导致退出
		}
 

		CRegKey Ndi;
		lRet = Ndi.Open(SubKey,TEXT("Ndi\\Interfaces"),KEY_QUERY_VALUE);

		SubKey.Close();////////////关闭SubKey句柄

 
		if(ERROR_SUCCESS != lRet)//////////有些键没有这个键所以打开失败，直接跳过即可
		{
			delete[] tcSubKeyBuffer;
			tcSubKeyBuffer = NULL;
			continue;
		}

 


		DWORD dwTmpValueSize = 0;
		Ndi.QueryStringValue(TEXT("LowerRange"),NULL,&dwTmpValueSize);
		if(ERROR_SUCCESS != lRet)
		{
			Reg.Close();
			CWindowsException exp(lRet,TEXT("GetComEthnetAdapterPath:Ndi.QueryStringValue1 error"));
			throw exp;
		}
		//printf("B4\n");
		//Sleep(3000);

		//printf("B7\n");
		//Sleep(5000);

		TCHAR *dwTmpValueBuffer = new TCHAR[dwTmpValueSize];
		lRet = Ndi.QueryStringValue(TEXT("LowerRange"),dwTmpValueBuffer,&dwTmpValueSize);

		Ndi.Close();/////////关闭Ndi句柄

		if(ERROR_SUCCESS != lRet)
		{
			Reg.Close();
			CWindowsException exp(lRet,TEXT("GetComEthnetAdapterPath:Ndi.QueryStringValue2 error"));
			throw exp;
		}


		//printf("B8\n");
		//Sleep(2000);


		if( !lstrcmp(dwTmpValueBuffer,TEXT("ethernet")) )
		{
			tstring strTmp = tcRoot;
			strTmp += TEXT("\\");
			strTmp += tcSubKeyBuffer;
			this->m_sEthnetAdpaterPath.push_back(strTmp);
			dwEtnCout++;
		}


		//printf("B9\n");
		//Sleep(3000);


		delete[] dwTmpValueBuffer;
        dwTmpValueBuffer = NULL;
        delete[] tcSubKeyBuffer;
		tcSubKeyBuffer = NULL;


		//printf("B10\n");
		//Sleep(3000);

	}
	Reg.Close();
      return dwEtnCout;

}
tstring CGetAdpPath::GetCurrentPath() const
{
    return this->m_sCurrentPath;
}
DWORD CGetAdpPath::GetEthnetCout()
{
	return (DWORD)(this->m_sEthnetAdpaterPath.size());/////////////返回容器中元素的个数
}
tstring CGetAdpPath::EnumEthnet(const DWORD& index)
{
	if((this->m_sEthnetAdpaterPath.size()) > index)//////////检查是否越界
	{
		return this->m_sEthnetAdpaterPath[index];
	}
	else
	{
		return tstring();
	}
}
bool CGetAdpPath::SetCurrentPath(const DWORD& index)
{
	if( this->m_sEthnetAdpaterPath.size() > index)//////////检查是否越界
	{
	this->m_sCurrentPath = this->m_sEthnetAdpaterPath[index];
	return true;
	}
	return false;
}
void CGetAdpPath::SetCurrentPath(const tstring& path)
{
   this->m_sCurrentPath = path;
}

CGetAdpPath::~ CGetAdpPath()
{
	this->m_sEthnetAdpaterPath.clear();
}

#endif