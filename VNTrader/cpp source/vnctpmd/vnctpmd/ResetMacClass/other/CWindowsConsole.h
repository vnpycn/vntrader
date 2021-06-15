#ifndef CWINDOWSCONSOLE_H_H_
#define CWINDOWSCONSOLE_H_H_
#include "windows.h"

class CWindowsConsole
{
public:
	CWindowsConsole()
	{
		this->m_hRead = NULL;
		this->m_hWrite = NULL;
		this->m_vBuffer = NULL;
		this->m_nBufferSize = 32;
	}
	~CWindowsConsole()
	{
	}
	inline bool Close();
	inline bool CreateConsole(const unsigned int& unBuffersize);
	inline HANDLE GetWriteHandle() const;
	inline HANDLE GetReadHandle() const;
	inline TCHAR* GetBufferPointer() const;
	inline unsigned int GetBufferSize() const;
	inline TCHAR* Read(DWORD* nNumberOfBytesRead=NULL);
	inline bool Write(TCHAR* lpBuffer,DWORD* nNumberOfBytesWrite=NULL);
protected:
	HANDLE m_hWrite;
	HANDLE m_hRead;
	TCHAR* m_vBuffer;
	unsigned int m_nBufferSize;
};
bool CWindowsConsole::CreateConsole(const unsigned int& unBuffersize)
{
	this->m_nBufferSize = unBuffersize;//////////////创建缓冲区
	if(NULL != this->m_vBuffer)
	{   
		delete[] this->m_vBuffer;

		this->m_vBuffer = new TCHAR[unBuffersize/sizeof(TCHAR)];
		::ZeroMemory(this->m_vBuffer,this->m_nBufferSize);
	}



	SECURITY_ATTRIBUTES sa;////////////////////////重定位管道
	HANDLE hCmdRead = NULL;
	HANDLE hCmdWrite = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;


	if(!::CreatePipe(&hCmdRead, &this->m_hWrite, &sa, 0))
	{
		return false;
	}

	if(!::CreatePipe(&this->m_hRead, &hCmdWrite, &sa, 0))
	{
		return false;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hCmdWrite;
	si.hStdOutput = hCmdWrite;
	si.hStdInput = hCmdRead;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	TCHAR strCmdLine[] = TEXT("cmd.exe");

	if(!::CreateProcess(NULL,strCmdLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))/////////创建CMD进程
	{
		return false;
	}
	if(NULL != this->m_vBuffer)
		delete[] this->m_vBuffer;
	this->m_vBuffer = new TCHAR[this->m_nBufferSize/sizeof(TCHAR)];
	::CloseHandle(hCmdWrite);
	::CloseHandle(hCmdRead);
	::CloseHandle(pi.hProcess);
	::CloseHandle(pi.hThread);
	return true;
}
HANDLE CWindowsConsole::GetReadHandle() const
{
	return this->m_hRead;
}
HANDLE CWindowsConsole::GetWriteHandle() const
{
	return this->m_hWrite;
}
TCHAR* CWindowsConsole::GetBufferPointer() const
{
	return this->m_vBuffer;
}
unsigned int CWindowsConsole::GetBufferSize() const
{
	return this->m_nBufferSize;
}

bool CWindowsConsole::Write(TCHAR* lpBuffer,DWORD* nNumberOfBytesWrite)
{
	DWORD dwTmpWriteedComm = 0;
	DWORD dwTmpWriteedEnter = 0;
	DWORD nNumberOfBytesToWrite;
#ifdef UNICODE
	nNumberOfBytesToWrite = WideCharToMultiByte(CP_OEMCP,NULL,lpBuffer,-1,NULL,0,NULL,FALSE);
	if(0 == nNumberOfBytesToWrite)
	{
		return FALSE;
	}
	char *tmpBuffer = new char[nNumberOfBytesToWrite];
	WideCharToMultiByte(CP_OEMCP,NULL,lpBuffer,-1,tmpBuffer,nNumberOfBytesToWrite*sizeof(char),NULL,FALSE);
	BOOL nRetComm = ::WriteFile(this->m_hWrite,(LPVOID)tmpBuffer,static_cast<DWORD>(strlen(tmpBuffer)*sizeof(char)),&dwTmpWriteedComm,NULL);
	delete[] tmpBuffer;
#else
	nNumberOfBytesToWrite = lstrlen(lpBuffer)*sizeof(TCHAR);
	BOOL nRetComm = ::WriteFile(this->m_hWrite,(LPVOID)lpBuffer,nNumberOfBytesToWrite,&dwTmpWriteedComm,NULL);
#endif

	BOOL nRetEnterEnter = ::WriteFile(this->m_hWrite,(LPVOID)"\n",sizeof("\n")-sizeof(char),&dwTmpWriteedEnter,NULL);

	if(NULL != nNumberOfBytesWrite)
		*nNumberOfBytesWrite = dwTmpWriteedComm;
	if(0 == nRetComm || 0 == nRetEnterEnter)
		return false;
	return true;
}
inline TCHAR* CWindowsConsole::Read(DWORD* nNumberOfBytesRead)
{
	DWORD dwTmpReaded = 0;
	if(NULL == this->m_vBuffer)
		return NULL;

	::ZeroMemory(this->m_vBuffer,this->m_nBufferSize);

	DWORD nRet = ::ReadFile(this->m_hRead,(LPVOID)this->m_vBuffer,(this->m_nBufferSize-1)/sizeof(TCHAR),&dwTmpReaded,NULL);


#ifdef UNICODE
	if(0 == nRet && 0 == dwTmpReaded)
	{
		return NULL;
	}
	int dwMinSize = MultiByteToWideChar (CP_ACP, 0,(LPCSTR)this->m_vBuffer,-1, NULL, 0);
	dwMinSize *= sizeof(TCHAR);
	if(static_cast<unsigned int>(dwMinSize) > this->m_nBufferSize)
		return false;
	WCHAR* tmpBuffer = new WCHAR[dwMinSize/sizeof(WCHAR)];
	MultiByteToWideChar (CP_ACP, 0,(LPCSTR)this->m_vBuffer,-1,tmpBuffer,dwMinSize);  
	memcpy(this->m_vBuffer,tmpBuffer,dwMinSize);
	delete[] tmpBuffer;

	return this->m_vBuffer;
#else
	if(NULL != nNumberOfBytesRead)
		*nNumberOfBytesRead = dwTmpReaded;
	if(0 == nRet && 0 == dwTmpReaded)
	{
		return NULL;
	}
	else
	{
		return this->m_vBuffer;
	}
#endif
}
bool CWindowsConsole::Close()
{
	if(NULL != this->m_hRead)
	{
		::CloseHandle(this->m_hRead);
	}
	if(NULL != this->m_vBuffer)
	{
		delete[] this->m_vBuffer;
		this->m_vBuffer = NULL;
		this->m_nBufferSize = 0;
	}
	if(NULL != this->m_hWrite)
	{
		if(false == this->Write(TEXT("exit")))
		{
			::CloseHandle(this->m_hWrite);
			return false;
		}
		::CloseHandle(this->m_hWrite);

	}
	return true;
}

#endif

/*
unsigned int CWindowsConsole::SetBufferSize(const unsigned int& nSize)
{
	unsigned int nTmpSize = this->m_nBufferSize;
	this->m_nBufferSize = nSize;
	if(NULL != this->m_vBuffer)
	{   
		delete[] this->m_vBuffer;

		this->m_vBuffer = new TCHAR[nSize/sizeof(TCHAR)];
		::ZeroMemory(this->m_vBuffer,this->m_nBufferSize);
	}

	return nTmpSize;
}
*/






/*
#define WM_WINDWOSCONSOLEREAD WM_USER+100
class CWindowsConsoleAsyn:public CWindowsConsole
{
public:
	CWindowsConsoleAsyn():CWindowsConsole()
	{
		m_bIsMessage = false;
		m_hReadThread = NULL;
		m_hWindowsHwnd = NULL;
		m_hThreadExitFlag = NULL;
		InitializeCriticalSection(&this->m_crBufferFlag);

	}
	~CWindowsConsoleAsyn()
	{
		::DeleteCriticalSection(&this->m_crBufferFlag);
	}
	inline bool CreateConsole();
	inline HWND SetMessage(const HWND& hwnd);
	inline unsigned int SetBufferSize(const unsigned int& nSize);
	inline bool StartRead();
	inline bool Close();
	static DWORD WINAPI ReadThreadProc(LPVOID lpParameter);
private:
	CRITICAL_SECTION m_crBufferFlag;
	bool m_bIsMessage;
	HANDLE m_hReadThread;
	HWND m_hWindowsHwnd;
	HANDLE m_hThreadExitFlag;
};

bool CWindowsConsoleAsyn::CreateConsole()
{

	if(!CWindowsConsole::CreateConsole())
		return false;
	return true;
}

HWND CWindowsConsoleAsyn::SetMessage(const HWND& hwnd)
{
	this->m_hWindowsHwnd = hwnd;
	this->m_bIsMessage = true;
	return this->m_hWindowsHwnd;
}
bool CWindowsConsoleAsyn::StartRead()
{
	this->m_hReadThread = ::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ReadThreadProc,(LPVOID)this,0,NULL);
	this->m_hThreadExitFlag = ::CreateEvent(NULL,TRUE,FALSE,NULL); 
	if(NULL == this->m_hReadThread || NULL == this->m_hThreadExitFlag )
		return false;
	return true;
}


unsigned int CWindowsConsoleAsyn::SetBufferSize(const unsigned int& nSize)
{
	unsigned int unRet = 0;
		::EnterCriticalSection(&this->m_crBufferFlag);
		unRet = CWindowsConsole::SetBufferSize(nSize);
		::LeaveCriticalSection(&this->m_crBufferFlag);
	return unRet;

}
bool CWindowsConsoleAsyn::Close()
{
	bool bRet = true;;
	::SetEvent(this->m_hThreadExitFlag);/////////////////////////设置“读线程”退出消息
	this->m_bIsMessage = false;///////////////必须设置，不然会让子线程和主线程都阻塞
	if(NULL != this->m_hWrite)/////////////////////发送“控制台进程”退出消息
	{
		bRet = this->Write(TEXT("exit"));
		::CloseHandle(this->m_hWrite);
	}
	::WaitForSingleObject(this->m_hReadThread,INFINITE);/////////等待“读线程”退出消息
	if(NULL != this->m_hRead)/////////////关闭读消息
	{
		::CloseHandle(this->m_hRead);	
		this->m_hRead = NULL;
	}


	if(NULL != this->m_vBuffer)/////////////删除Buffer
	{
		delete[] this->m_vBuffer;
		this->m_vBuffer = NULL;
		this->m_nBufferSize = 0;
	}


	::CloseHandle(this->m_hReadThread);
	return bRet;
}

#endif
*/