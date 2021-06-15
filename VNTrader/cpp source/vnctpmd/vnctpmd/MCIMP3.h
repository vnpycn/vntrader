// MCIMP3.h: interface for the CMCIMP3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCIMP3_H__76FDC07C_8F2D_48F7_9C9D_72CAF40A68F1__INCLUDED_)
#define AFX_MCIMP3_H__76FDC07C_8F2D_48F7_9C9D_72CAF40A68F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

class CMCIMP3  
{
public:
	CMCIMP3();
	virtual ~CMCIMP3();
	DWORD MCIOpen(const CString &strPath);
	DWORD MCIClose();
	DWORD MCIPlay();     //播放1次
	DWORD MCIPlayLOOP();  //循环播放
	DWORD MCIPause();
	DWORD MCIResume();
	DWORD MCIStop();
protected:
	MCIDEVICEID m_wID;
};

#endif // !defined(AFX_MCIMP3_H__76FDC07C_8F2D_48F7_9C9D_72CAF40A68F1__INCLUDED_)
