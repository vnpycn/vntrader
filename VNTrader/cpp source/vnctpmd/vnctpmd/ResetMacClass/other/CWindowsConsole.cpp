#include "stdafx.h"
#include "CWindowsConsole.h"
DWORD WINAPI CWindowsConsoleAsyn::ReadThreadProc(LPVOID lpParameter)
{
	CWindowsConsoleAsyn* pWcaObject = static_cast<CWindowsConsoleAsyn*>(lpParameter);

	while(WAIT_OBJECT_0 != ::WaitForSingleObject(pWcaObject->m_hThreadExitFlag,0))
	{

		DWORD dwReadSize = 0;
		TCHAR *pReadData;

		if(NULL != (pReadData = pWcaObject->Read(&dwReadSize)))
		{
			//if(pWcaObject->m_bIsMessage)
			::SendMessage(pWcaObject->m_hWindowsHwnd,WM_WINDWOSCONSOLEREAD,(WPARAM)pReadData,(LPARAM)dwReadSize);///////必须使用SendMessage,不然Buffer出错
		}



	}

return 1;

}
