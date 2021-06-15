#ifndef CSIMPLELIST_H_H_
#define CSIMPLELIST_H_H_
#include "CNoTrackObject.h"
template<class DTYPE>  class CSimpleList;
template<class DTYPE> class CListIterator;
template<class DTYPE>  class CNode//:public CNoTrackObject
{
	friend class CSimpleList<DTYPE>;
	friend class CListIterator<DTYPE>;
	CNode* pNext;
	DTYPE data;
};
template<class DTYPE>
class CSimpleList
{
	friend class CListIterator<DTYPE>;
public:
	CSimpleList();
	CSimpleList(CNode<DTYPE>* phead);
	CSimpleList(const CSimpleList& csl);
	void AttachList(CNode<DTYPE>* phead);
	CNode<DTYPE>* DetachList();

	bool DeleteNode(DTYPE& value,const int& pos);
	bool AddNode(const DTYPE& value,const int& pos);
	CSimpleList& operator =(const DTYPE& value);
	void AddNodeToEnd(const DTYPE& value);
	void AddNodeToBegin(const DTYPE& value);
	int GetNodeNum() const;
	bool IsEmpty() const;
	bool EditNode(DTYPE& value,const int& pos);
	void DeleteAll();
	bool GetNodeValue(DTYPE& value,const int& pos) const;
	virtual ~CSimpleList()
	{
		delete this->m_pHead;
	}
private:
	CNode<DTYPE>* m_pHead;
};


template<class DTYPE>
CSimpleList<DTYPE>::CSimpleList()
{
	this->m_pHead = new CNode<DTYPE>;
	this->m_pHead->pNext = 0;
}
template<class DTYPE>
CSimpleList<DTYPE>::CSimpleList(const CSimpleList& csl)
{
	this->m_pHead = new CNode<DTYPE>;
	this->m_pHead->pNext = csl.m_pHead->pNext;

}
template<class DTYPE>
CSimpleList<DTYPE>::CSimpleList(CNode<DTYPE>* phead)
{
	this->m_pHead = new CNode<DTYPE>;
	this->m_pHead->pNext = phead;
}
template<class DTYPE>
void CSimpleList<DTYPE>::AttachList(CNode<DTYPE>* phead)
{
	this->m_pHead->pNext = phead;
}
template<class DTYPE>
CNode<DTYPE>* CSimpleList<DTYPE>::DetachList()
{
	CNode<DTYPE>* pTmp = this->m_pHead->pNext;
	this->m_pHead->pNext = 0;
	return pTmp;
}
template<class DTYPE>
bool CSimpleList<DTYPE>::AddNode(const DTYPE &value, const int &pos)
{
	if(pos <= 0)
	{
		return false;
	}
	int nTmp = 1;
	CNode<DTYPE>* pTmp = this->m_pHead;
	while(nTmp != pos && 0 != pTmp) ////////////////find pos
	{
		pTmp = pTmp->pNext;
		nTmp++;
	}
	if(pos == nTmp)///////////is right pos:yes
	{
		CNode<DTYPE>* pNewNode = new CNode<DTYPE>;////////add node
		pNewNode->data = value; 
		pNewNode->pNext = pTmp->pNext;
		pTmp->pNext = pNewNode;
		return true;
	}
	else///////////is right pos:yes
	{
		return false;
	}

}
template<class DTYPE>
void CSimpleList<DTYPE>::AddNodeToBegin(const DTYPE &value)
{
	CNode<DTYPE>* pNewNode = new CNode<DTYPE>;////////add node
	pNewNode->data = value; 
	pNewNode->pNext = this->m_pHead->pNext;
	this->m_pHead->pNext = pNewNode;
}
template<class DTYPE>
CSimpleList<DTYPE>& CSimpleList<DTYPE>::operator =(const DTYPE& value)
{
	CNode<DTYPE>* pNewNode = new CNode<DTYPE>;////////add node
	pNewNode->data = value; 
	pNewNode->pNext = this->m_pHead->pNext;
	this->m_pHead->pNext = pNewNode;
	return *this;
}
template<class DTYPE>
void CSimpleList<DTYPE>::AddNodeToEnd(const DTYPE &value)
{
	CNode<DTYPE>* pTmp = this->m_pHead;
	while(0 != pTmp->pNext)
	{
		pTmp = pTmp->pNext;
	}
	CNode<DTYPE>* pNewNode = new CNode<DTYPE>;////////add node
	pNewNode->data = value; 
	pNewNode->pNext = 0;
	pTmp->pNext = pNewNode;
}
template<class DTYPE>
bool CSimpleList<DTYPE>::DeleteNode(DTYPE& value,const int& pos)
{
	if(pos <= 0)
	{
		return false;
	}

	int nTmp = 1;
	CNode<DTYPE>* pTmp = this->m_pHead;
	while(nTmp != pos && 0 != pTmp->pNext) ////////////////find pos
	{
		pTmp = pTmp->pNext;
		nTmp++;
	}
	DTYPE oldValue;
	CNode<DTYPE>* pDel;
	if(pos == nTmp && 0 != pTmp->pNext)///////////is right pos:yes
	{

		oldValue = pTmp->pNext->data;
		pDel = pTmp->pNext;
		pTmp->pNext = pDel->pNext;
		delete pDel;
	}
	return false;
}
template<class DTYPE>
void CSimpleList<DTYPE>::DeleteAll()
{
	CNode<DTYPE>* pTmp = this->m_pHead->pNext; //////////Hold First Node
	this->m_pHead->pNext = 0;

	CNode<DTYPE>* pDel;
	while(0 != pTmp)
	{
		pDel = pTmp;
		pTmp = pTmp->pNext;
		delete pDel;
	}

}
template<class DTYPE>
bool CSimpleList<DTYPE>::IsEmpty() const
{
	if(0 != this->m_pHead->pNext)
		return false;
	else
		return true;
}
template<class DTYPE>
int CSimpleList<DTYPE>::GetNodeNum() const
{
	int nNum = 0;
	CNode<DTYPE>* pTmp = this->m_pHead->pNext;
	while(0 != pTmp)
	{
		nNum++;
		pTmp = pTmp->pNext;
	}
	return nNum;
}
template<class DTYPE>
bool CSimpleList<DTYPE>::GetNodeValue(DTYPE& value,const int &pos) const
{
	if(pos <= 0)
	{
		return false;
	}

	int nTmp = 0;
	CNode<DTYPE>* pTmp = this->m_pHead;
	while(nTmp != pos && 0 != pTmp) ////////////////find pos
	{
		pTmp = pTmp->pNext;
		nTmp++;
	}
	if(pos == nTmp && 0 != pTmp)///////////is right pos:yes
	{
		value = pTmp->data;
		return true;
	}
	return false;
}
template<class DTYPE>
bool CSimpleList<DTYPE>::EditNode(DTYPE &value, const int &pos)
{
	if(pos <= 0)
	{
		return false;
	}

	int nTmp = 0;
	DTYPE oldValue;
	CNode<DTYPE>* pTmp = this->m_pHead;
	while(nTmp != pos && 0 != pTmp) ////////////////find pos
	{
		pTmp = pTmp->pNext;
		nTmp++;
	}
	if(pos == nTmp && 0 != pTmp)///////////is right pos:yes
	{
		oldValue = pTmp->data;
		pTmp->data = value;
		value = oldValue;
		return true;
	}
	return false;
}










template<class DTYPE>
class CListIterator
{
public:
	CListIterator()
	{
		this->m_List = 0;
		this->m_Current = 0;
	};
	CListIterator(CSimpleList<DTYPE>* list);
	void AttachList(CSimpleList<DTYPE>* list);
	CSimpleList<DTYPE>*DetachList();
	bool IsEnd() const;
	bool FindFirst();
	DTYPE FindNext();
	DTYPE GetCurrent() const;
private:
	CSimpleList<DTYPE>* m_List;
	CNode<DTYPE>* m_Current;
};
template<class DTYPE>
CListIterator<DTYPE>::CListIterator(CSimpleList<DTYPE>* list):m_List(list),m_Current(list->m_pHead)
{

}
template<class DTYPE>
void CListIterator<DTYPE>::AttachList(CSimpleList<DTYPE>* list)
{
	this->m_List = list;
	this->m_Current = list->m_pHead;
}
template<class DTYPE>
CSimpleList<DTYPE>* CListIterator<DTYPE>::DetachList()
{
	CSimpleList<DTYPE>* pTmpList;
	pTmpList = this->m_List;
	this->m_List = 0;
	this->m_Current = 0;
	return pTmpList;
}
template<class DTYPE>
bool CListIterator<DTYPE>::IsEnd() const
{
	if(0 == this->m_Current)
	{
		return true;
	}
	else
	{
		return false;
	}
}
template<class DTYPE>
bool CListIterator<DTYPE>::FindFirst()
{
	if(0 == m_List->m_pHead)
	{
		return false;
	}
	else
	{
		this->m_Current = m_List->m_pHead->pNext;
		return true;
	}

}
template<class DTYPE>
DTYPE CListIterator<DTYPE>::FindNext()
{
	CNode<DTYPE>* pTmp;
	pTmp = this->m_Current;
	this->m_Current = this->m_Current->pNext;
	return pTmp->data;
}
template<class DTYPE>
DTYPE CListIterator<DTYPE>::GetCurrent() const
{
	return this->m_Current->data;
}

#endif