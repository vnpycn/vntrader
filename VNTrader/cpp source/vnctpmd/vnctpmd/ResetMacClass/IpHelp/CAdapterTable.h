#ifndef CADAPTERTABLE_H_H_
#define CADAPTERTABLE_H_H_
#include "CAdapterInfo.h"
namespace HongLib
{
	class CAdapterTable
	{
	private:
		PMIB_IFTABLE m_IfTable;
		DWORD m_TableBufferSize;
	public:
		CAdapterTable()
		{
			this->m_IfTable = NULL;
			this->m_TableBufferSize = 0;
		}
		CAdapterTable(const CAdapterTable& other)
		{
			this->m_IfTable = (PMIB_IFTABLE)::GlobalAlloc(GPTR,other.m_TableBufferSize);
			memcpy(this->m_IfTable,other.m_IfTable,other.m_TableBufferSize);
			this->m_TableBufferSize = other.m_TableBufferSize;
		}
		virtual ~CAdapterTable()
		{
			this->Clear();
		}

		/**************
		功能：得到适配器数量
		前置条件：更新函数调用 Update(PIP_INTERFACE_INFO pAdapterTable = NULL);
		参数：无
		返回值：网卡数量
		**************/
		DWORD GetNumber() const
		{
			ASSERT(NULL != this->m_IfTable)

				return this->m_IfTable->dwNumEntries;
		}

		void Clear()
		{

			::GlobalFree(this->m_IfTable);
			this->m_IfTable = NULL;
			this->m_TableBufferSize = 0;
		}
		CAdapterInfo* CAdapterTable::EnumAdapterInfoObject(const DWORD& dwEnumIndex) const
		{
			if(dwEnumIndex < this->m_IfTable->dwNumEntries)
			{
				return new CAdapterInfo(&this->m_IfTable->table[dwEnumIndex]);
			}
			return NULL;
		}
		/***************
		功能：更新适配器列表

		参数：ppAdapterTable 适配器列表头指针
		返回值：更新结果
		***************/
		DWORD CAdapterTable::Update(PMIB_IFTABLE* ppAdapterTable = NULL)
		{
			DWORD dwSize = 0;

			if(ERROR_INSUFFICIENT_BUFFER == ::GetIfTable(this->m_IfTable,&this->m_TableBufferSize,FALSE))
			{
				::GlobalFree(this->m_IfTable);
				this->m_IfTable = (PMIB_IFTABLE)::GlobalAlloc(GPTR,this->m_TableBufferSize);
			}
			DWORD nRet;
			nRet = ::GetIfTable(this->m_IfTable,&this->m_TableBufferSize,FALSE);
			if(NULL != ppAdapterTable)
			{
				*ppAdapterTable = this->m_IfTable;
			}
			return nRet;
		}
		/****************
		功能：得到一个适配器的具体信息
		前置条件：更新函数调用 Update(PIP_INTERFACE_INFO pAdapterTable = NULL);
		或者将bIsUpdate设置为true
		参数：
		dwAdapterIndex 适配器索引
		bIsUpdate 是否更新信息
		nRet 更新的返回值（NO_ERROR更新成功）
		返回值：CAdapterInfo 一个具体适配器对象
		****************/
		CAdapterInfo* CAdapterTable::GetAdapterInfoObject(const DWORD& dwAdapterIndex,const bool& bIsUpdate = false)
		{
               DWORD nRet = 0L;
			if(true == bIsUpdate)
			{
					nRet = this->Update();
			}
			if(NO_ERROR != nRet)
                return NULL;

			for(DWORD i=0; i<this->m_IfTable->dwNumEntries;i++)
			{
				if(dwAdapterIndex == this->m_IfTable->table[i].dwIndex)
				{
					return new CAdapterInfo(&this->m_IfTable->table[i]);
				}
			}
			return NULL;
		}

	};
}
#endif