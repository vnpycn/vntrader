// MCIMP3.cpp: implementation of the CMCIMP3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Test.h"
#include "MCIMP3.h"


#include <digitalv.h>   //循环播放需要的 dwFlags|=MCI_DGV_PLAY_REPEAT;//循环播放标志/自己添加


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMCIMP3::CMCIMP3()
{
	m_wID=NULL;
}

CMCIMP3::~CMCIMP3()
{
	MCIClose();
}

//打开MCI设备  参数为文件地址
DWORD CMCIMP3::MCIOpen(const CString &strPath)
{
  MCI_OPEN_PARMS mciOP;
  DWORD dwReturn;

  ASSERT(m_wID == NULL);
  mciOP.lpstrDeviceType = NULL;
  mciOP.lpstrElementName = strPath;
  dwReturn=mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_WAIT|MCI_OPEN_SHAREABLE,
	  (DWORD)(LPVOID)&mciOP);
  if (dwReturn==0)
  {
	  m_wID=mciOP.wDeviceID;
  }
  else
	  m_wID=NULL;
  return dwReturn;
}

//关闭MCI设备
DWORD CMCIMP3::MCIClose()
{
  MCI_GENERIC_PARMS mciGP;
  DWORD dwReturn;
  dwReturn=mciSendCommand(m_wID,MCI_CLOSE,MCI_NOTIFY|MCI_WAIT,
	  (DWORD)(LPVOID)&mciGP);
  m_wID=NULL;
  return dwReturn;
}

//播放
DWORD CMCIMP3::MCIPlay()
{
	MCI_PLAY_PARMS mciPP;
    return mciSendCommand(m_wID,MCI_PLAY,MCI_NOTIFY,(DWORD)(LPVOID)&mciPP);
}


DWORD CMCIMP3::MCIPlayLOOP()  //循环播放
{
	MCI_PLAY_PARMS mciPP;
	return mciSendCommand(m_wID,MCI_PLAY,MCI_DGV_PLAY_REPEAT,(DWORD)(LPVOID)&mciPP);
}


//暂停
DWORD CMCIMP3::MCIPause()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID,MCI_PAUSE,MCI_NOTIFY|MCI_WAIT,(DWORD)(LPVOID)&mciGP);
}

//继续 （从暂停恢复）
DWORD CMCIMP3::MCIResume()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID,MCI_RESUME,MCI_NOTIFY|MCI_WAIT,(DWORD)(LPVOID)&mciGP);
}

//停止播放
DWORD CMCIMP3::MCIStop()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID,MCI_STOP,MCI_NOTIFY|MCI_WAIT,(DWORD)(LPVOID)&mciGP);
}
