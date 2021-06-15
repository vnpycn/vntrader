#ifndef CSETUPCOMDI_H_H_
#define CSETUPCOMDI_H_H_
#include <setupapi.h>
#include <Cfgmgr32.h>
#pragma comment(lib,"Setupapi.lib")
#pragma comment(lib,"Rpcrt4.lib")

#ifdef _DEBUG

#ifndef ASSERT
#define ASSERT(f)\
	if(!(f)) _asm{int 3}
#endif

#else

#ifndef ASSERT
#define ASSERT(f) 
#endif

#endif


#ifdef UNICODE///////////为了兼容UNICODE
#define TRPC_CSTR  RPC_WSTR
#else 
#define TRPC_CSTR RPC_CSTR
#endif


namespace HongLib
{
	class CSetupComDi
	{
	public:
		inline	CSetupComDi();

		inline ~CSetupComDi();
		bool QueryFromDatabase(TCHAR* DataSource);
		PBYTE EnumSetupDi(const DWORD& dwdwPropFliter = SPDRP_DEVICEDESC);
		inline DWORD ResetEnum(const DWORD& index = 0);

		inline bool IsDisable();////////是否被禁用
		inline GUID GetDevGUID() const;///////////获取该设备的GUID
		inline bool IsCanBeDisable();////////是否可以禁用该设备
		inline bool EnableDev();////////启用设备
		inline bool DisableDev();////////禁用设备
	private:
		HDEVINFO m_hDevInfo;//////查询信息
		SP_DEVINFO_DATA m_sDeviceInfoData;//////设备数据
		DWORD m_dwIndex;////////索引
		PBYTE m_pbBuffer;
		inline void FreeBuffer();
		inline bool SetDevState(const bool& bEnable);/////改变设备状态

	protected:
		inline	CSetupComDi(CSetupComDi& object){}/////////不允许使用复制构造函函数
	};

	inline bool CSetupComDi::EnableDev()
	{
		return this->SetDevState(true);
	}
	inline bool CSetupComDi::DisableDev()
	{
		return this->SetDevState(false);
	}
	inline	CSetupComDi::CSetupComDi()
	{
		this->m_hDevInfo = INVALID_HANDLE_VALUE;
		this->m_dwIndex = 0;
		this->m_pbBuffer = NULL;
	};

	inline CSetupComDi::~CSetupComDi()
	{
		FreeBuffer();/////////防止最后一次没有释放资源
	};
	////////重置枚举
	DWORD CSetupComDi::ResetEnum(const DWORD& index)
	{
		DWORD dwOld = this->m_dwIndex;
		this->m_dwIndex = dwOld;
		return dwOld;
	}

	void CSetupComDi::FreeBuffer()
	{
		::GlobalFree(this->m_pbBuffer);
		this->m_pbBuffer = NULL;
	}

	bool CSetupComDi::SetDevState(const bool& bEnable)
	{
		ASSERT(INVALID_HANDLE_VALUE != this->m_hDevInfo);

			BOOL bRet = FALSE;
		SP_PROPCHANGE_PARAMS propChange = { sizeof ( SP_CLASSINSTALL_HEADER ) };
		propChange.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		propChange.Scope = DICS_FLAG_GLOBAL;
		if(bEnable)
		{
			propChange.StateChange = DICS_ENABLE;   // 启用，或是停用，请使用DICS_DISABLE
		}
		else
		{
			propChange.StateChange = DICS_DISABLE;
		}


		bRet = SetupDiSetClassInstallParams
			(      
			this->m_hDevInfo, 
			&this->m_sDeviceInfoData, 
			(SP_CLASSINSTALL_HEADER*) &propChange, 
			sizeof(propChange) 
			);

		if(TRUE != bRet)
			return FALSE;

		bRet = SetupDiCallClassInstaller(
			DIF_PROPERTYCHANGE,
			this->m_hDevInfo,
			&this->m_sDeviceInfoData);

		if(bRet)
		{
			return true;
		}
		else
		{
			return false;
		}
	}



	GUID CSetupComDi::GetDevGUID() const
	{
		ASSERT(INVALID_HANDLE_VALUE != this->m_hDevInfo);
			return this->m_sDeviceInfoData.ClassGuid;
	}


	bool CSetupComDi::IsDisable()
	{
		ASSERT(INVALID_HANDLE_VALUE != this->m_hDevInfo);

			ULONG ulProb;
		ULONG uState;
		if(CR_SUCCESS != CM_Get_DevNode_Status(&uState,&ulProb,this->m_sDeviceInfoData.DevInst,0))
		{
			return false;
		}
		if(CM_PROB_HARDWARE_DISABLED == ulProb || CM_PROB_DISABLED == ulProb)
			return true;
		return false;

	}



	bool CSetupComDi::IsCanBeDisable()
	{
		ASSERT(INVALID_HANDLE_VALUE != this->m_hDevInfo);

			ULONG ulProb;
		ULONG uState;
		if(CR_SUCCESS != CM_Get_DevNode_Status(&uState,&ulProb,this->m_sDeviceInfoData.DevInst,0))
		{
			return false;
		}
		if(DN_DISABLEABLE & uState)
			return true;
		return false;

	}



	bool CSetupComDi::QueryFromDatabase(TCHAR* DataSource)//////////////该参数为GUID字符串，不包括括号
	{

		ASSERT(NULL != DataSource);

			GUID gDataSrc;
		if(RPC_S_OK != UuidFromString((TRPC_CSTR)DataSource, &gDataSrc))////////////设备的GUID比如网络适配器的是{4D36E972-E325-11CE-BFC1-08002bE10318};
			return false;


		RtlZeroMemory(&this->m_sDeviceInfoData, sizeof(SP_DEVINFO_DATA));                  
		this->m_sDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);                   

		this->m_hDevInfo = SetupDiGetClassDevs(&gDataSrc,0,0,DIGCF_PRESENT );
		if(INVALID_HANDLE_VALUE == this->m_hDevInfo)
			return false;
		return true;
	}


	PBYTE CSetupComDi::EnumSetupDi(const DWORD& dwPropFliter)
	{
		PBYTE pbRet = NULL;
		ASSERT(
			(SPDRP_CHARACTERISTICS  == dwPropFliter) ||
			(SPDRP_CLASS == dwPropFliter) || 
			(SPDRP_CLASSGUID  == dwPropFliter) ||  
			(SPDRP_COMPATIBLEIDS  == dwPropFliter) ||
			(SPDRP_CONFIGFLAGS  == dwPropFliter) ||
			(SPDRP_DEVICE_POWER_DATA  == dwPropFliter) ||  
			(SPDRP_DEVICEDESC  == dwPropFliter) || 
			(SPDRP_DEVTYPE  == dwPropFliter) ||
			(SPDRP_DRIVER  == dwPropFliter) ||
			(SPDRP_ENUMERATOR_NAME  == dwPropFliter) ||
			(SPDRP_EXCLUSIVE == dwPropFliter) ||
			(SPDRP_FRIENDLYNAME  == dwPropFliter) ||
			(SPDRP_HARDWAREID  == dwPropFliter) ||
			(SPDRP_INSTALL_STATE  == dwPropFliter) ||
			(SPDRP_LEGACYBUSTYPE == dwPropFliter) ||
			(SPDRP_LOCATION_INFORMATION  == dwPropFliter)||
			(SPDRP_LOCATION_PATHS == dwPropFliter) ||
			(SPDRP_LOWERFILTERS  == dwPropFliter) ||
			(SPDRP_MFG  == dwPropFliter) ||
			(SPDRP_PHYSICAL_DEVICE_OBJECT_NAME == dwPropFliter) ||
			(SPDRP_REMOVAL_POLICY  ==  dwPropFliter) ||
			(SPDRP_REMOVAL_POLICY_HW_DEFAULT  == dwPropFliter) ||
			(SPDRP_REMOVAL_POLICY_OVERRIDE == dwPropFliter) ||
			(SPDRP_SECURITY == dwPropFliter) ||
			(SPDRP_SECURITY_SDS  == dwPropFliter) ||
			(SPDRP_SERVICE == dwPropFliter) ||
			(SPDRP_UI_NUMBER  == dwPropFliter) ||
			(SPDRP_UI_NUMBER_DESC_FORMAT  == dwPropFliter) ||
			(SPDRP_UPPERFILTERS == dwPropFliter)
			);

			FreeBuffer();//////////////释放上次的资源

		if( SetupDiEnumDeviceInfo(this->m_hDevInfo,this->m_dwIndex,&this->m_sDeviceInfoData) )
		{
			DWORD dwData;
			DWORD buffersize = 0;
			while(
				!SetupDiGetDeviceRegistryProperty(
				this->m_hDevInfo,
				&this->m_sDeviceInfoData,
				dwPropFliter,
				&dwData,
				(PBYTE)this->m_pbBuffer,
				buffersize,
				&buffersize) 
				)
			{
				if (ERROR_INSUFFICIENT_BUFFER == GetLastError())// Change the buffer size.
				{
					if (this->m_pbBuffer) 
					{
						GlobalFree(this->m_pbBuffer);
					}
					this->m_pbBuffer = (PBYTE)GlobalAlloc(GPTR,buffersize);
				}
				else
				{
					break;
				}
			}

			m_dwIndex++;
			return this->m_pbBuffer;
		}

		return this->m_pbBuffer;
	}


};
#endif