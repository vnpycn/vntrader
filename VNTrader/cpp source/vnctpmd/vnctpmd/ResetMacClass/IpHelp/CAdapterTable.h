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
		���ܣ��õ�����������
		ǰ�����������º������� Update(PIP_INTERFACE_INFO pAdapterTable = NULL);
		��������
		����ֵ����������
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
		���ܣ������������б�

		������ppAdapterTable �������б�ͷָ��
		����ֵ�����½��
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
		���ܣ��õ�һ���������ľ�����Ϣ
		ǰ�����������º������� Update(PIP_INTERFACE_INFO pAdapterTable = NULL);
		���߽�bIsUpdate����Ϊtrue
		������
		dwAdapterIndex ����������
		bIsUpdate �Ƿ������Ϣ
		nRet ���µķ���ֵ��NO_ERROR���³ɹ���
		����ֵ��CAdapterInfo һ����������������
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