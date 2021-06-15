#ifndef CREGBASE_H_H_
#define CREGBASE_H_H_
#include <windows.h>
#include "mydebug.h"
class CRegBase
{
public:
	CRegBase();
	inline LONG CreateKey(HKEY hKeyParent,LPCTSTR lpsSubKey,LPTSTR lpsClass = REG_NONE,DWORD dwOptions = REG_OPTION_NON_VOLATILE,REGSAM samDesired = KEY_ALL_ACCESS,LPDWORD lpdwDisposition = NULL);
	inline LONG OpenKey(HKEY hKeyParent,LPCTSTR lpsSubKey,REGSAM samDesired = KEY_ALL_ACCESS);
	inline LONG QueryValueSize(LPCTSTR lpsValueName,DWORD* pdwSize) const;
	inline LONG QueryValueType(LPCTSTR lpsValueName,DWORD* pdwType) const;
	inline LONG QueryValue(LPCTSTR pszValueName, GUID& guidValue) const;
	inline LONG QueryValue(LPCTSTR lpsValueName,DWORD* pdwValue) const;
	inline LONG QueryValue(LPCTSTR lpsValueName,LPTSTR pszValue,DWORD* pdwSize) const;
	inline LONG QueryValue(LPCTSTR lpsValueName,DWORD* pdwType,void* pvValue,ULONG* pdwSize) const;
	inline LONG QueryKeyProperty(LPTSTR lpsClass,DWORD* pdwClassSize,DWORD* lpcbSecurityDescriptor = NULL,SYSTEMTIME* pstKeyChangeTime = NULL) const;
	inline LONG SetValue(LPCTSTR lpsValueName,const DWORD& dwValue) const;
	SetValue(LPCTSTR lpsValueName,LPCTSTR szValue,const DWORD& dwSize,const DWORD& dwType = REG_SZ) const
	inline LONG SetValue(LPCTSTR lpsValueName,const DWORD& pdwType,const void* pvValue,const DWORD& dwSize) const;
	static inline bool IsExistKey(HKEY hKeyParent,LPCTSTR lpsSubKey);////////////静态函数
	inline bool IsExistValue(LPCTSTR lpsValueNalue) const;
	inline bool IsValidHandle() const;
	inline LONG GetEnumKeyInfo(DWORD *pdwRegCouts,DWORD* lpcMaxSubKeyLen) const;
	inline LONG GetEnumValueInfo(DWORD *pdwValueCouts,DWORD* lpcValueNameSize,DWORD* lpcbMaxValueLen = NULL) const;
	inline LONG DeleteValue(LPCTSTR lpValueName) const;
	static inline LONG DeleteKey(HKEY hKeyParent,LPCTSTR lpsSubKey);////////////静态函数
	inline LONG NotifyChangeKeyValue() const;
	inline LONG FlushKey() const;
	inline LONG EnumKey(const DWORD& dwIndex,LPTSTR lpsSubKey,DWORD* pdwSubKeySize,SYSTEMTIME* pstKeyChangeTime = NULL) const;
	inline LONG EnumValue(const DWORD& dwIndex,LPTSTR lpsValueName ,DWORD* pdwValueNameSize,DWORD* pdwType = NULL,void* pvValue = NULL,DWORD* pdwValueSize = NULL) const;
	inline LONG CloseKey();
	inline ~CRegBase();
private:
	HKEY m_hKey;
};

CRegBase::CRegBase() 
{

	this->m_hKey = NULL;
}
LONG CRegBase::CreateKey(HKEY hKeyParent, LPCTSTR lpsSubKey, LPTSTR lpsClass, DWORD dwOptions, REGSAM samDesired , LPDWORD lpdwDisposition )
{
	ASSERT( (NULL == this->m_hKey) && (NULL != hKeyParent || NULL !=lpsSubKey) )
		return::RegCreateKeyEx(hKeyParent,lpsSubKey,0,lpsClass,dwOptions,samDesired,NULL,&this->m_hKey,lpdwDisposition);
}
LONG CRegBase::OpenKey(HKEY hKeyParent, LPCTSTR lpsSubKey, REGSAM samDesired)
{
	ASSERT( (NULL == this->m_hKey) && (NULL != hKeyParent || NULL !=lpsSubKey) )
		return ::RegOpenKeyEx(hKeyParent,lpsSubKey,0,samDesired,&this->m_hKey);
}
///////获取值的大小（字节数）
LONG CRegBase::QueryValueSize(LPCTSTR lpsValueName,DWORD *pdwSize) const
{
	ASSERT(NULL != this->m_hKey)
		return ::RegQueryValueEx(this->m_hKey,lpsValueName,0,NULL,NULL,pdwSize);
}
/////////获取值的类型
LONG CRegBase::QueryValueType(LPCTSTR lpsValueName, DWORD *pdwType) const
{
	ASSERT( (NULL != this->m_hKey) && (NULL != lpsValueName) && NULL != pdwType)
		return ::RegQueryValueEx(this->m_hKey,lpsValueName,0,pdwType,NULL,NULL);
}
///////////获取DWORD类型值
LONG CRegBase::QueryValue(LPCTSTR lpsValueName,DWORD* pdwValue) const
{
	ASSERT( (NULL != this->m_hKey) && (NULL != lpsValueName) && (NULL != pdwValue) )
	DWORD dwSize = sizeof(DWORD);
	DWORD dwType = 0;
	LONG lRet = ::RegQueryValueEx(this->m_hKey,lpsValueName,0,&dwType,(LPBYTE)pdwValue,&dwSize);
    ASSERT((lRes != ERROR_SUCCESS) || (dwType == REG_DWORD));
	ASSERT((lRes != ERROR_SUCCESS) || (dwSize == sizeof(DWORD)));
}
////////////获取字符型的值
LONG CRegBase::QueryValue(LPCTSTR lpsValueName,LPTSTR pszValue,DWORD* pdwSize) const
{
	ASSERT(pdwSiz != NULL);
	DWORD dwType = 0;
	LONG lRes = RegQueryValueEx(this->m_hKey,lpsValueName, NULL, &dwType, (LPBYTE)pszValue, pdwSize);
	ASSERT((lRes != ERROR_SUCCESS) || (dwType == REG_SZ) ||  (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));
	if (pszValue != NULL)
	{
		if(*pdwSize>0)
		{
			switch(dwType)
			{
				case REG_SZ:
				case REG_EXPAND_SZ:
					if ((*pdwSize) % sizeof(TCHAR) != 0 || pszValue[(*pdwSize) / sizeof(TCHAR) - 1] != 0)
					{
						pszValue[0]=_T('\0');
		 				return ERROR_INVALID_DATA;
					}
					break;
				case REG_MULTI_SZ:
					if ((*pdwSize) % sizeof(TCHAR) != 0 || (*pdwSize) / sizeof(TCHAR) < 1 || pszValue[(*pdwSize) / sizeof(TCHAR) -1] != 0 || (((*pdwSize) / sizeof(TCHAR))>1 && pszValue[(*pdwSize) / sizeof(TCHAR) - 2] != 0) )
					{
						pszValue[0]=_T('\0');
						return ERROR_INVALID_DATA;
					}
					break;
				default:
					// Ensure termination
					pszValue[0]=_T('\0');
					return ERROR_INVALID_DATA;
			}
		}
		else
		{
			// this is a blank one with no data yet
			// Ensure termination
			pszValue[0]=_T('\0');
		}
	}
	return lRes;
	

}
//////////////获取GUID类型的值
LONG CRegBase::QueryValue(LPCTSTR pszValueName, GUID& guidValue)
{
	/*
	ASSERT(m_hKey != NULL);
	TCHAR szGUID[64];
	LONG lRes;
	ULONG nCount;
	HRESULT hr;

	guidValue = GUID_NULL;

	nCount = 64;
	lRes = QueryStringValue(pszValueName, szGUID, &nCount);

	if (lRes != ERROR_SUCCESS)
		return lRes;

	if(szGUID[0] != _T('{'))
		return ERROR_INVALID_DATA;

	USES_CONVERSION_EX;
	LPOLESTR lpstr = T2OLE_EX(szGUID, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
#ifndef _UNICODE
	if(lpstr == NULL) 
		return E_OUTOFMEMORY;
#endif	
		
	hr = ::CLSIDFromString(lpstr, &guidValue);
	if (FAILED(hr))
		return ERROR_INVALID_DATA;

	return ERROR_SUCCESS;
	*/
}
///////////获取任意类型的值
LONG CRegBase::QueryValue(LPCTSTR lpsValueName, DWORD *pdwType, void* pvValue,ULONG* pdwSize) const
{
	ASSERT((NULL != this->m_hKey) && (NULL != lpsValueName) && (NULL != pdwSize) && (NULL != pdwType))
		return ::RegQueryValueEx(this->m_hKey,lpsValueName,0,pdwType,static_cast<LPBYTE>(pvValue),pdwSize);
}
///////////写字整型的值
LONG CRegBase::SetValue(LPCTSTR lpsValueName,const DWORD &dwValue) const
{
		ASSERT( (NULL != this->m_hKey) && (NULLL != lpsValueName) )
		return ::RegSetValueEx(this->m_hKey,lpsValueName,0,REG_DWORD,reinterpret_cast<const BYTE*>(&dwValue),sizeof(DWORD));
}
/////////////写字符串的值
LONG CRegBase::SetValue(LPCTSTR lpsValueName,LPCTSTR szValue,const DWORD& dwSize,const DWORD& dwType) const
{
	ASSERT( (NULL != this->m_hKey) && (NULLL != lpsValueName) || (NULL != szValue) ) 
   ASSERT((dwType == REG_SZ) ||  (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));

	return ::RegSetValueEx(this->m_hKey,lpsValueName,0,dwType,reinterpret_cast<const BYTE*>szValue,dwSize);
}
////////////写入任意类型数据
LONG CRegBase::SetValue(LPCTSTR lpsValueName, const DWORD &pdwType, const void *pvValue,const DWORD& dwSize) const
{
	ASSERT( (NULL != this->m_hKey) && (NULLL != lpsValueName) || (NULL != pvValue) ) 
		return ::RegSetValueEx(this->m_hKey,lpsValueName,0,pdwType,reinterpret_cast<const BYTE*>pvValue,dwSize);

}

////////////判断键是否存在
static bool CRegBase::IsExistKey(HKEY hKeyParent, LPCTSTR lpsSubKey) const
{
	ASSERT(NULL != hKeyParent || NULL != lpsSubKey)
		HKEY hTmpKey = NULL;
	LONG lRet = ::RegOpenKeyEx(hKeyParent,lpsSubKey,0,KEY_ALL_ACCESS,&hTmpKey);
	if(ERROR_SUCCESS == lRet)
	{
		::RegCloseKey(hTmpKey);
		return true;
	}
	if(0x02 == lRet)
	{
		return false;
	}
	else
	{
		return true;
	}
}
/////////////////判断值是否存在
bool CRegBase::IsExistValue(LPCTSTR lpsValueName) const
{
	ASSERT( (NULL != lpsValueName)  && (NULL != this->m_hKey) )
	LONG lRet = ::RegQueryValueEx(this->m_hKey,lpsValueName,0,NULL,NULL,NULL);
	if(0x02 == lRet)
	{
		return false;
	}
	return true;
}
////////////////判断当前打开键句柄是否有效
bool CRegBase::IsValidHandle() const
{
	if(NULL == this->m_hKey)
		return false;
	return true;
}
////////////////删除值
LONG CRegBase::DeleteValue(LPCTSTR lpValueName) const
{
	ASSERT( (NULL != lpsValueName)  && (NULL != this->m_hKey) )
	return ::RegDeleteValue(this->m_hKey,lpValueName);
}
/////////////////删除键
LONG CRegBase::DeleteKey(HKEY hKeyParent, LPCTSTR lpsSubKey)
{
	ASSERT( (NULL != hKeyParent) || (NULL != lpsSubKey))
	return ::RegDeleteKey(hKeyParent,lpsSubKey);
}
////////////刷新缓存
LONG CRegBase::FlushKey() const
{
	ASSERT(NULL != this->m_hKey)
	return ::RegFlushKey(this->m_hKey);
}
LONG CRegBase::CloseKey()
{
	ASSERT(NULL != this->m_hKey)
	LONG nRet = ::RegCloseKey(this->m_hKey);
	this->m_hKey = NULL;
	return nRet;
}
CRegBase::~CRegBase()
{
	ASSERT(NULL == this->m_hKey)
		this->m_hKey = NULL;
}
///////////////////获取枚举信息
LONG CRegBase::GetEnumKeyInfo(DWORD *pdwRegCouts,DWORD* lpcMaxSubKeySize) const
{
	ASSERT(NULL != this->m_hKey)
	LONG nRet = ::RegQueryInfoKey(this->m_hKey,NULL,NULL,0,pdwRegCouts,lpcMaxSubKeyLen,NULL,NULL,NULL,NULL,NULL,NULL);
	if(NULL != lpcMaxSubKeyLen)
		*lpcMaxSubKeyLen += 1;

	return nRet;
}
LONG CRegBase::GetEnumValueInfo(DWORD *pdwValueCouts, DWORD *lpcValueNameLen, DWORD *lpcbMaxValueLen) const
{
	ASSERT(NULL != this->m_hKey)
	LONG nRet = ::RegQueryInfoKey(this->m_hKey,NULL,NULL,0,NULL,NULL,NULL,pdwValueCouts,lpcValueNameLen,lpcbMaxValueLen,NULL,NULL);
	if(NULL != lpcValueNameLen)
		*lpcValueNameLen += 1;
	if(NULL != lpcbMaxValueLen)
		*lpcbMaxValueLen += 1;
	return nRet;

}
LONG CRegBase::EnumKey(const DWORD &dwIndex, LPTSTR lpsSubKey, DWORD *pdwSubKeySize, LPTSTR lpsClass, DWORD *pdwClassSize, SYSTEMTIME *pstKeyChangeTime) const
{
ASSERT(NULL != this->m_hKey || NULL !=lpsSubKey)
	if(NULL == )
		return 0x0E; ////////////Not enough storage is available to complete this operation.
	FILETIME ft;
	LONG nRet = ::RegEnumKeyEx(this->m_hKey,dwIndex,lpsSubKey,pdwSubKeySize,0,lpsClass,pdwClassSize,&ft);
	if(NULL != pstKeyChangeTime)
		::FileTimeToSystemTime(&ft,pstKeyChangeTime);
	return nRet;

}

LONG CRegBase::EnumValue(const DWORD &dwIndex, LPTSTR lpsValueName, DWORD *pdwValueNameSize, DWORD *pdwType, void *pvValue, DWORD *pdwValueSize) const
{
	if(NULL == lpsValueName)
		return 0x0E;////////////Not enough storage is available to complete this operation.
	return ::RegEnumValue(this->m_hKey,dwIndex,lpsValueName,pdwValueNameSize,0,pdwType,(LPBYTE)pvValue,pdwValueSize);

}

LONG CRegBase::QueryKeyProperty(LPTSTR lpsClass, DWORD *pdwClassSize, DWORD* lpcbSecurityDescriptor,SYSTEMTIME* pstKeyChangeTime) const
{
	FILETIME ft;
	LONG nRet = ::RegQueryInfoKey(this->m_hKey,lpsClass,pdwClassSize,0,NULL,NULL,NULL,NULL,NULL,NULL,lpcbSecurityDescriptor,&ft);
	if(NULL != pstKeyChangeTime)
		::FileTimeToSystemTime(&ft,pstKeyChangeTime);
	return nRet;

}
#endif